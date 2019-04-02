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

void osg_board_Uart_ctor(osg_Uart * self, const osg_UartConfig * const config)
{
    if (_osg_board_Uart_checkUart(self->id) == false)
    {
        osg_error("ERROR: UART port NOT available on this device!");
    }

    // Rx and Tx PINs can be swapped
    if (_osg_board_Uart_validateTxPin(self->id, self->txPin.id) == false
        && _osg_board_Uart_validateRxPin(self->id, self->txPin.id) == false)
        osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");

    // Rx and Tx PINs can be swapped
    if (_osg_board_Uart_validateRxPin(self->id, self->rxPin.id) == false
        && _osg_board_Uart_validateTxPin(self->id, self->rxPin.id) == false)
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
    
    _osg_board_Uart_resetUart(self->id);
    _osg_board_Uart_enableClock(self->id);

    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->handler = _osg_board_Uart_getUartHandler(self->id);
    uart->type = _osg_board_Uart_encodeUartType(self->id);

    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;
    handler->Instance = _osg_board_Uart_getUartInstance(self->id);
    handler->Init.Mode       = UART_MODE_TX_RX;
    handler->Init.BaudRate   = _osg_board_Uart_decodeUartBaudRate(config->baud);
    handler->Init.Parity     = _osg_board_Uart_decodeUartParity(config->parity);
    handler->Init.WordLength = _osg_board_Uart_decodeUartBits(config->bits, config->parity);
    handler->Init.StopBits   = _osg_board_Uart_decodeUartStopBits(config->stopBits);
    handler->Init.OverSampling = UART_OVERSAMPLING_16;
    handler->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    handler->Init.HwFlowCtl  = _osg_board_Uart_decodeUartFlowControl(config->flow);
    handler->Init.ClockPrescaler = UART_PRESCALER_DIV1;
    handler->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    uart->flags = OSG_UART_FLAG_DEFAULT;

    _osg_board_Uart_enable(handler, uart->type);

    if (HAL_UART_DeInit(handler) != HAL_OK) osg_error("ERROR: UART deinit fails.");

    const HAL_StatusTypeDef initErr = HAL_UART_Init(handler);
    if (initErr != HAL_OK) osg_error("ERROR: UART init failed");
    
    if (HAL_UARTEx_SetTxFifoThreshold(handler, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        osg_error("ERROR: UART init failed");
    }

    if (HAL_UARTEx_SetRxFifoThreshold(handler, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        osg_error("ERROR: UART init failed");
    }

    if (HAL_UARTEx_DisableFifoMode(handler) != HAL_OK)
    {
        osg_error("ERROR: UART init failed");
    }
    
    _osg_board_Uart_enableInterrupt(self->id);
}


void osg_board_Uart_dtor(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    HAL_UART_DeInit(handler);
    
    _osg_board_Uart_resetUart(self->id);
    _osg_board_Uart_disableClock(self->id);
    _osg_board_Uart_disableInterrupt(self->id);

    uart->txCompleteNb = NULL;
    uart->rxCompleteNb = NULL;
    uart->txCompleteBuffered = NULL;
    uart->rxCompleteBuffered = NULL;
    uart->handler = NULL;
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
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    const HAL_StatusTypeDef ret = HAL_UART_Transmit(handler, (uint8_t *)buffer, (uint16_t)size, timeout);

    return ret == HAL_OK ? true : false;
}

bool osg_board_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    const osg_UartFlags restore = uart->flags;
    uart->flags |= OSG_UART_FLAG_IS_NB_SEND | OSG_UART_FLAG_IS_SEND;

    const HAL_StatusTypeDef status = HAL_UART_Transmit_IT(handler, (uint8_t *)buffer, (uint16_t)size);
    
    const bool ret = status == HAL_OK ? true : false;
    if (!ret)
    {
        uart->flags = restore;
    }

    return ret;
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

    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    const HAL_StatusTypeDef ret = HAL_UART_Receive(handler, (uint8_t *)buffer, (uint16_t)size, timeout);
    
    return ret == HAL_OK ? true : false;
}

bool osg_board_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED) return false;
    const osg_UartFlags restore = uart->flags;
    uart->flags |= OSG_UART_FLAG_IS_NB_RECEIVE | OSG_UART_FLAG_IS_RECEIVE;

    const HAL_StatusTypeDef status = HAL_UART_Receive_IT(handler, (uint8_t *)buffer, (uint16_t)size);
    
    const bool ret = status == HAL_OK ? true : false;
    if (!ret)
    {
        uart->flags = restore;
    }

    return ret;
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
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED)
    {
        const HAL_StatusTypeDef ret = HAL_UART_AbortReceive_IT(handler);
        
        if (ret == HAL_OK)
        {
            while (!(uart->flags & OSG_UART_FLAG_RX_ABORTED))
            {
                // waiting for abort
            }

            uart->flags &= ~OSG_UART_FLAG_IS_RECEIVE;
            uart->flags &= ~OSG_UART_FLAG_RX_BUFF_ENABLED;
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

Size osg_board_Uart_getReceivedCharsRxBuffer(osg_Uart * self)
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
