// This file is part of osG.
//
// osG is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// osG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
//
// osG is also available under a commercial license.
// Please contact GIMASI at info@gimasi.ch for further information.
//
#include <string.h>
#include "../../../../include/board/drivers/board-Uart.h"
#include "board-UartSupport.h"
#include "../../../../../osg/include/osg.h"

void osg_board_Uart_ctor(osg_Uart * self, const osg_UartConfig * config)
{
    if (_osg_board_Uart_checkUart(self->id) == false)
    {
        osg_error("ERROR: UART port NOT available on this device!");
    }

    if (_osg_board_Uart_validateTxPin(self->id, self->txPin.id) == false)
        osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");

    if (_osg_board_Uart_validateRxPin(self->id, self->rxPin.id) == false)
        osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");

    if (config->flow == OSG_UART_FLOWCONTROL_CTS
        || config->flow == OSG_UART_FLOWCONTROL_RTS_CTS)
    {
        if (_osg_board_Uart_validateCtsPin(self->id, self->ctsPin.id) == false)
            osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");
    }

    if (config->flow == OSG_UART_FLOWCONTROL_RTS
        || config->flow == OSG_UART_FLOWCONTROL_RTS_CTS)
    {
        if (_osg_board_Uart_validateRtsPin(self->id, self->rtsPin.id) == false)
            osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");
    }

    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->handler = _osg_board_Uart_getUartInstance(self->id);
    uart->type = _osg_board_Uart_encodeUartType(self->id);

    uart->flags = OSG_UART_FLAG_DEFAULT;

    if (uart->type == OSG_LPUART)
    {
        lpuart_config_t conf;
        LPUART_GetDefaultConfig(&conf);
        conf.baudRate_Bps = _osg_board_Uart_decodeUartBaudRate(config->baud);
        conf.parityMode = _osg_board_Uart_decodeUartParity(config->parity);
        conf.dataBitsCount = _osg_board_Uart_decodeUartBits(config->bits, config->parity);
        conf.stopBitCount = _osg_board_Uart_decodeUartStopBits(config->stopBits);
        conf.enableRx = true;
        conf.enableTx = true;
        conf.enableRxRTS = _osg_board_Uart_decodeUartRtsEnabled(config->flow);
        conf.enableTxCTS = _osg_board_Uart_decodeUartCtsEnabled(config->flow);

        const status_t initErr = LPUART_Init((osg_board_UartInstance *)uart->handler, &conf, CLOCK_GetFreq(kCLOCK_Osc0ErClk));
        if (initErr != kStatus_Success) osg_error("UART init failed");
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    _osg_board_Uart_enableClock(self->id);
}


void osg_board_Uart_dtor(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (uart->type == OSG_LPUART)
    {
        LPUART_Deinit((osg_board_UartInstance *)uart->handler);
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    uart->txCompleteNb = NULL;
    uart->rxCompleteNb = NULL;
    uart->txCompleteBuffered = NULL;
    uart->rxCompleteBuffered = NULL;
    uart->handler = NULL;
    uart->flags = OSG_UART_FLAG_DEFAULT;
}

void osg_board_Uart_setBuffers(osg_Uart * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);

    osg_CircularFifoConfig circularConfig;
    memset(&circularConfig, 0, sizeof(circularConfig));

    circularConfig.buffer = circularRxBuffer;
    circularConfig.size = rxBufferSize;
    circularConfig.behavior = OSG_FIFO_OVERWRITE_NO_ERROR;
    osg_CircularFifo_ctor(&uart->rxFifo, &circularConfig);

    circularConfig.buffer = circularTxBuffer;
    circularConfig.size = txBufferSize;
    circularConfig.behavior = OSG_FIFO_ERROR;
    osg_CircularFifo_ctor(&uart->txFifo, &circularConfig);
}

void osg_board_Uart_setBufferedCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->txCompleteBuffered = tx;
    uart->rxCompleteBuffered = rx;
}

void osg_board_Uart_setNbCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->txCompleteNb = tx;
    uart->rxCompleteNb = rx;
}


bool osg_board_Uart_sendBlocking(osg_Uart * self, const void * buffer, const Size size, const uint32_t timeout)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    int state = -1;
    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_Uart_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);

    if (uart->type == OSG_LPUART)
    {
        lpuart_transfer_t xfer;
        xfer.data = (uint8_t *)buffer;
        xfer.dataSize = size;
        osg_board_UartHandler * handle = _osg_board_Uart_getUartHandler(self->id);

        uart->flags &= ~OSG_UART_FLAG_HANDLE_CREATED;
        LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, &state);
        const status_t status = LPUART_TransferSendNonBlocking((osg_board_UartInstance *)uart->handler, handle, &xfer);

        if (status != kStatus_Success)
        {
            osg_Timer_dtor(&timer);
            return false;
        }

        osg_Timer_start(&timer, timeout);
        while (1)
        {
            if (state == kStatus_LPUART_TxIdle)
            {
                osg_Timer_stop(&timer);
                osg_Timer_dtor(&timer);
                return true;
            }

            if (state == kStatus_Timeout)
                break;
        }

        osg_Timer_stop(&timer);
        osg_Timer_dtor(&timer);
        return false;
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    osg_Timer_dtor(&timer);
    return false;
}

bool osg_board_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);

    const osg_UartFlags restore = uart->flags;
    uart->flags |= OSG_UART_FLAG_IS_NB_SEND | OSG_UART_FLAG_IS_SEND;

    if (uart->type == OSG_LPUART)
    {
        lpuart_transfer_t xfer;
        xfer.data = (uint8_t *)buffer;
        xfer.dataSize = size;
        osg_board_UartHandler * handle = _osg_board_Uart_getUartHandler(self->id);
        if (!(uart->flags & OSG_UART_FLAG_HANDLE_CREATED))
        {
            LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, NULL);
            uart->flags |= OSG_UART_FLAG_HANDLE_CREATED;
        }
        const status_t status = LPUART_TransferSendNonBlocking((osg_board_UartInstance *)uart->handler, handle, &xfer);
        const bool ret = status == kStatus_Success ? true : false;

        if (!ret)
        {
            uart->flags = restore;
        }

        return ret;
    }
    else
        osg_error("ERROR: Invalid UART type.");

    return false;
}

bool osg_board_Uart_sendBuffered(osg_Uart * self, const void * buffer, const Size bufferSize)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);

    if (osg_CircularFifo_isNull(&uart->txFifo) == true)
        return false;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&uart->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return false;

    _osg_board_Uart_doBufferedSend(uart);

    return true;
}

bool osg_board_Uart_receiveBlocking(osg_Uart * self, void * buffer, const Size size, const uint32_t timeout)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED) return false;
    int state = -1;
    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_Uart_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);

    if (uart->type == OSG_LPUART)
    {
        Size received = 0;
        lpuart_transfer_t xfer;
        xfer.data = buffer;
        xfer.dataSize = size;
        osg_board_UartHandler * handle = _osg_board_Uart_getUartHandler(self->id);
        uart->flags &= ~OSG_UART_FLAG_HANDLE_CREATED;
        LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, &state);
        const status_t status = LPUART_TransferReceiveNonBlocking((osg_board_UartInstance *)uart->handler, handle, &xfer, &received);

        if (status != kStatus_Success)
        {
            osg_Timer_dtor(&timer);
            return false;
        }

        osg_Timer_start(&timer, timeout);
        while (1)
        {
            if (state == kStatus_LPUART_RxIdle)
            {
                osg_Timer_stop(&timer);
                osg_Timer_dtor(&timer);
                return true;
            }

            if (state == kStatus_Timeout)
                break;
        }

        osg_Timer_stop(&timer);
        osg_Timer_dtor(&timer);
        return false;
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    osg_Timer_dtor(&timer);
    return false;
}

bool osg_board_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);

    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED) return false;
    const osg_UartFlags restore = uart->flags;
    uart->flags |= OSG_UART_FLAG_IS_NB_RECEIVE | OSG_UART_FLAG_IS_RECEIVE;

    if (uart->type == OSG_LPUART)
    {
        Size received = 0;
        lpuart_transfer_t xfer;
        xfer.data = buffer;
        xfer.dataSize = size;
        osg_board_UartHandler * handle = _osg_board_Uart_getUartHandler(self->id);
        if (!(uart->flags & OSG_UART_FLAG_HANDLE_CREATED))
        {
            LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, NULL);
            uart->flags |= OSG_UART_FLAG_HANDLE_CREATED;
        }
        const status_t status = LPUART_TransferReceiveNonBlocking((osg_board_UartInstance *)uart->handler, handle, &xfer, &received);
        const bool ret = status == kStatus_Success ? true : false;

        if (!ret)
        {
            uart->flags = restore;
        }

        return ret;
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    return false;
}

Size osg_board_Uart_receiveBuffered(osg_Uart * self, void * buffer, const Size bufferSize)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (osg_CircularFifo_isNull(&uart->rxFifo) == true)
        return false;

    return osg_CircularFifo_popBuffer(&uart->rxFifo, buffer, bufferSize);
}

void osg_board_Uart_startReceiveBuffered(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_assert(osg_CircularFifo_isNull(&uart->rxFifo) == false, "ASSERT FAILED: UART Rx Buffer is NULL");
    uart->flags |= OSG_UART_FLAG_RX_BUFF_ENABLED | OSG_UART_FLAG_IS_RECEIVE;
    _osg_board_Uart_doBufferedReceive(uart);
}

void osg_board_Uart_stopReceiveBuffered(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_assert(osg_CircularFifo_isNull(&uart->rxFifo) == false, "ASSERT FAILED: UART Rx Buffer is NULL");
    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED)
    {
        if (uart->type == OSG_LPUART)
        {
            osg_board_UartHandler * handle = _osg_board_Uart_getUartHandler(self->id);
            LPUART_TransferAbortReceive((osg_board_UartInstance *)uart->handler, handle);

            uart->flags &= ~OSG_UART_FLAG_IS_RECEIVE;
            uart->flags &= ~OSG_UART_FLAG_RX_BUFF_ENABLED;
        }
        else
        {
            osg_error("ERROR: Inavlid UART type.");
        }
    }
    osg_CircularFifo_clear(&uart->rxFifo);
}

bool osg_board_Uart_isReceiveBufferedEnabled(osg_Uart * self)
{
    if (_osg_board_Uart_getUartById(self->id)->flags & OSG_UART_FLAG_RX_BUFF_ENABLED)
        return true;

    return false;
}

Size osg_board_Uart_getReceivedCharsRxBuffer(osg_Uart *self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (osg_CircularFifo_isNull(&uart->rxFifo) == false)
        return osg_CircularFifo_getSize(&uart->rxFifo);

    return 0;
}

uint8_t osg_board_Uart_countUarts()
{
    return _osg_board_Uart_getUarts();
}
