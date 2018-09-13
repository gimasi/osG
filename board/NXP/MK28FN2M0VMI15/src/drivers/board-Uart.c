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
#include "board/drivers/board-Uart.h"
#include <osg.h>
#include <fsl_lpuart.h>
#include <fsl_port.h>

#define TOT_UART FSL_FEATURE_SOC_LPUART_COUNT


static uint8_t _osg_board_Uart_getUarts()
{
    return (uint8_t)TOT_UART;
}

typedef lpuart_handle_t osg_board_UartHandler;
typedef LPUART_Type osg_board_UartInstance;

static osg_UartImpl _osg_board_Uart_devices[TOT_UART];
static osg_board_UartHandler _osg_board_Uart_handlers[TOT_UART];

static osg_UartImpl * _osg_board_Uart_getUartByInstance(void * handler)
{
    for (int i = 0; i < TOT_UART; ++i)
    {
        if (_osg_board_Uart_devices[i].handler == handler) return &_osg_board_Uart_devices[i];
    }

    return NULL;
}

static uint8_t _osg_board_Uart_getDeviceIndex(const osg_UartId id)
{
    uint8_t index = 0;

#ifdef LPUART0
    if (id == OSG_UART0)
        return index;
    ++index;
#endif
#ifdef LPUART1
    if (id == OSG_UART1)
        return index;
    ++index;
#endif
#ifdef LPUART2
    if (id == OSG_UART2)
        return index;
    ++index;
#endif
#ifdef LPUART3
    if (id == OSG_UART3)
        return index;
    ++index;
#endif
#ifdef LPUART4
    if (id == OSG_UART4)
        return index;
    ++index;
#endif
#ifdef LPUART5
    if (id == OSG_UART5)
        return index;
    ++index;
#endif

    return TOT_UART;
}

static osg_UartImpl * _osg_board_Uart_getUartById(const osg_UartId id)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(id);
    return &_osg_board_Uart_devices[index];
}

static osg_board_UartHandler * _osg_board_Uart_getUartHandler(const osg_UartId id)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(id);
    return &_osg_board_Uart_handlers[index];
}

static osg_board_UartHandler * _osg_board_Uart_getUartHandlerFromInstance(const osg_board_UartInstance * instance)
{
#ifdef LPUART0
    if (instance == LPUART0)
        return _osg_board_Uart_getUartHandler(OSG_UART0);
    else
#endif
#ifdef LPUART1
        if (instance == LPUART1)
            return _osg_board_Uart_getUartHandler(OSG_UART1);
    else
#endif
#ifdef LPUART2
        if (instance == LPUART2)
            return _osg_board_Uart_getUartHandler(OSG_UART2);
    else
#endif
#ifdef LPUART3
        if (instance == LPUART3)
            return _osg_board_Uart_getUartHandler(OSG_UART3);
    else
#endif
#ifdef LPUART4
        if (instance == LPUART4)
            return _osg_board_Uart_getUartHandler(OSG_UART4);
    else
#endif
#ifdef LPUART5
        if (instance == LPUART5)
            return _osg_board_Uart_getUartHandler(OSG_UART5);
    else
#endif
        osg_error("ERROR: Invalid UART peripheral selected.");

    return NULL;
}

static osg_board_UartInstance * _osg_board_Uart_getUartInstance(const osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0: return LPUART0;
#endif
#ifdef LPUART1
        case OSG_UART1: return LPUART1;
#endif
#ifdef LPUART2
        case OSG_UART2: return LPUART2;
#endif
#ifdef LPUART3
        case OSG_UART3: return LPUART3;
#endif
#ifdef LPUART4
        case OSG_UART4: return LPUART4;
#endif
#ifdef LPUART5
        case OSG_UART5:
            return LPUART5;
#endif
        default: osg_error("ERROR: Invalid UART peripheral selected..");
    }

    return NULL;
}

static osg_UartType _osg_board_Uart_getUartType(const osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0: return OSG_LPUART;
#endif
#ifdef LPUART1
        case OSG_UART1: return OSG_LPUART;
#endif
#ifdef LPUART2
        case OSG_UART2: return OSG_LPUART;
#endif
#ifdef LPUART3
        case OSG_UART3: return OSG_LPUART;
#endif
#ifdef LPUART4
        case OSG_UART4: return OSG_LPUART;
#endif
        default: osg_error("ERROR: Impossible to retrieve UART Type.");
    }

    // suppress warning
    return OSG_UART;
}

osg_GpioAlternateFunction _osg_board_Uart_getGpioAlternateFunction(const osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0:
        {
            return OSG_GPIO_ALTERNATE_UART0;
        }
#endif
#ifdef LPUART1
        case OSG_UART1:
        {
            return OSG_GPIO_ALTERNATE_UART1;
        }
#endif
#ifdef LPUART2
        case OSG_UART2:
        {
            return OSG_GPIO_ALTERNATE_UART2;
        }
#endif
#ifdef LPUART3
        case OSG_UART3:
        {
            return OSG_GPIO_ALTERNATE_UART3;
        }
#endif
#ifdef LPUART4
        case OSG_UART4:
        {
            return OSG_GPIO_ALTERNATE_UART4;
        }
#endif
#ifdef LPUART5
        case OSG_UART5:
        {
            return OSG_GPIO_ALTERNATE_UART5;
        }
#endif
        default:
            osg_error("ERROR: Impossible to set correct I2C Alternate Function to Pin.");
    }

    // suppress warning
    return OSG_GPIO_ALTERNATE_DEFAULT;
}

port_mux_t _osg_board_Uart_getPinPortMuxForUart(const osg_GpioId pin)
{
    switch (pin)
    {
        case OSG_GPIO_PA0:
        case OSG_GPIO_PA1:
        case OSG_GPIO_PA2:
        case OSG_GPIO_PA3:
        case OSG_GPIO_PB12:
        case OSG_GPIO_PB13:
        case OSG_GPIO_PB14:
        case OSG_GPIO_PB15:
            return kPORT_MuxAlt2;
        case OSG_GPIO_PA14:
        case OSG_GPIO_PA15:
        case OSG_GPIO_PA16:
        case OSG_GPIO_PA17:
        case OSG_GPIO_PA20:
        case OSG_GPIO_PA21:
        case OSG_GPIO_PA22:
        case OSG_GPIO_PA23:
        case OSG_GPIO_PA24:
        case OSG_GPIO_PA25:
        case OSG_GPIO_PA26:
        case OSG_GPIO_PA27:
        case OSG_GPIO_PA28:
        case OSG_GPIO_PA29:
        case OSG_GPIO_PA30:
        case OSG_GPIO_PA31:
        case OSG_GPIO_PB2:
        case OSG_GPIO_PB3:
        case OSG_GPIO_PB8:
        case OSG_GPIO_PB9:
        case OSG_GPIO_PB10:
        case OSG_GPIO_PB11:
        case OSG_GPIO_PB16:
        case OSG_GPIO_PB17:
        case OSG_GPIO_PC1:
        case OSG_GPIO_PC2:
        case OSG_GPIO_PC3:
        case OSG_GPIO_PC4:
        case OSG_GPIO_PC12:
        case OSG_GPIO_PC13:
        case OSG_GPIO_PC14:
        case OSG_GPIO_PC15:
        case OSG_GPIO_PC16:
        case OSG_GPIO_PC17:
        case OSG_GPIO_PC18:
        case OSG_GPIO_PC19:
        case OSG_GPIO_PC24:
        case OSG_GPIO_PC25:
        case OSG_GPIO_PC26:
        case OSG_GPIO_PC27:
        case OSG_GPIO_PD0:
        case OSG_GPIO_PD1:
        case OSG_GPIO_PD2:
        case OSG_GPIO_PD3:
        case OSG_GPIO_PD4:
        case OSG_GPIO_PD5:
        case OSG_GPIO_PD6:
        case OSG_GPIO_PD7:
        case OSG_GPIO_PD8:
        case OSG_GPIO_PD9:
        case OSG_GPIO_PD10:
        case OSG_GPIO_PD11:
        case OSG_GPIO_PE0:
        case OSG_GPIO_PE1:
        case OSG_GPIO_PE2:
        case OSG_GPIO_PE3:
        case OSG_GPIO_PE4:
        case OSG_GPIO_PE5:
        case OSG_GPIO_PE6:
        case OSG_GPIO_PE7:
        case OSG_GPIO_PE12:
        case OSG_GPIO_PE13:
        case OSG_GPIO_PE16:
        case OSG_GPIO_PE17:
        case OSG_GPIO_PE18:
        case OSG_GPIO_PE19:
        case OSG_GPIO_PE20:
        case OSG_GPIO_PE21:
        case OSG_GPIO_PE22:
        case OSG_GPIO_PE23:
            return kPORT_MuxAlt3;
        default:
            osg_error("ERROR: PIN can't be used for UART.");
    }

    return kPORT_PinDisabledOrAnalog;
}

static void _osg_board_Uart_sendReceiveCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);

static void _osg_board_Uart_doBufferedSend(osg_UartImpl * uart)
{
    if (osg_CircularFifo_isEmpty(&uart->txFifo)) return;
    const osg_UartFlags restore = uart->flags;
    uart->flags &= ~OSG_UART_FLAG_IS_NB_SEND;
    uart->flags |= OSG_UART_FLAG_IS_SEND;

    osg_board_UartHandler * handle = _osg_board_Uart_getUartHandlerFromInstance(uart->handler);

    if (uart->type == OSG_LPUART)
    {
        lpuart_transfer_t transfer;
        osg_CircularFifo_getFilledBuffer(&uart->txFifo, (void **)&transfer.data, &transfer.dataSize);

        if (!(uart->flags & OSG_UART_FLAG_HANDLE_CREATED))
        {
            LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, NULL);
            uart->flags |= OSG_UART_FLAG_HANDLE_CREATED;
        }
        const status_t ok = LPUART_TransferSendNonBlocking(uart->handler, handle, &transfer);
        if (ok != kStatus_Success) uart->flags = restore;
    }
    else
        osg_error("ERROR: Invalid UART type.");
}

static void _osg_board_Uart_doBufferedReceive(osg_UartImpl * uart)
{
    if (osg_CircularFifo_isNull(&uart->rxFifo)
            || !(uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED))
        return;

    const osg_UartFlags restore = uart->flags;
    uart->flags &= ~OSG_UART_FLAG_IS_NB_RECEIVE;
    uart->flags |= OSG_UART_FLAG_IS_RECEIVE;

    osg_board_UartHandler * handle = _osg_board_Uart_getUartHandlerFromInstance(uart->handler);

    if (uart->type == OSG_LPUART)
    {
        lpuart_transfer_t transfer;
        transfer.data = &uart->rxByte;
        transfer.dataSize = 1;
        Size s = 0;

        if (!(uart->flags & OSG_UART_FLAG_HANDLE_CREATED))
        {
            LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, NULL);
            uart->flags |= OSG_UART_FLAG_HANDLE_CREATED;
        }
        const status_t ok = LPUART_TransferReceiveNonBlocking(uart->handler, handle, &transfer, &s);
        if (ok != kStatus_Success) uart->flags = restore;
    }
    else
        osg_error("ERROR: Invalid UART type.");
}

// Not possible to split callbacks for supporting simultaneous send/receive (e.g, buffered functions)
static void _osg_board_Uart_sendReceiveCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if (userData != NULL)
    {
        int * pState = (int *)userData;
        *pState = (int)status;
        return;
    }

    osg_UartImpl * uart = _osg_board_Uart_getUartByInstance(base);
    
    if (uart->flags & OSG_UART_FLAG_IS_SEND
        && status == kStatus_LPUART_TxIdle)
    {
        if (uart->flags & OSG_UART_FLAG_IS_NB_SEND)
        {
            uart->flags &= ~OSG_UART_FLAG_IS_SEND;
            if (uart->txCompleteNb != NULL) uart->txCompleteNb();
        }
        else
        {
            // LPUART_TransferGetSendCount()
            osg_CircularFifo_popBytes(&uart->txFifo, handle->txDataSizeAll - handle->txDataSize);
            if (uart->txCompleteBuffered != NULL) uart->txCompleteBuffered();
        }
        uart->flags &= ~OSG_UART_FLAG_IS_NB_SEND;
        _osg_board_Uart_doBufferedSend(uart);
    }
    
    if (uart->flags & OSG_UART_FLAG_IS_RECEIVE
        && status == kStatus_LPUART_RxIdle)
    {
        if (uart->flags & OSG_UART_FLAG_IS_NB_RECEIVE)
        {
            uart->flags &= ~OSG_UART_FLAG_IS_RECEIVE;
            if (uart->rxCompleteNb != NULL) uart->rxCompleteNb();
        }
        else
        {
            // LPUART_TransferGetReceiveCount()
            osg_CircularFifo_pushBuffer(&uart->rxFifo, &uart->rxByte, 1);
            if (uart->rxCompleteBuffered != NULL) uart->rxCompleteBuffered();
        }
        uart->flags &= ~OSG_UART_FLAG_IS_NB_RECEIVE;
        _osg_board_Uart_doBufferedReceive(uart);
    }
}

void _osg_board_Uart_timeoutCallback(void * argument)
{
    int * pState = (int *)argument;
    *pState = kStatus_Timeout;
}

static uint32_t _osg_board_Uart_decodeUartBaudRate(osg_UartBaudRate br)
{
    switch (br)
    {
        case OSG_UART_BAUDRATE_300: return 300;
        case OSG_UART_BAUDRATE_600:  return 600;
        case OSG_UART_BAUDRATE_1200: return 1200;
        case OSG_UART_BAUDRATE_2400: return 2400;
        case OSG_UART_BAUDRATE_4800: return 4800;
        case OSG_UART_BAUDRATE_9600: return 9600;
        case OSG_UART_BAUDRATE_19200: return 19200;
        case OSG_UART_BAUDRATE_38400: return 38400;
        case OSG_UART_BAUDRATE_57600: return 57600;
        case OSG_UART_BAUDRATE_115200: return 115200;
        case OSG_UART_BAUDRATE_230400: return 230400;
        case OSG_UART_BAUDRATE_460800: return 460800;
        case OSG_UART_BAUDRATE_576000: return 576000;
        case OSG_UART_BAUDRATE_921600: return 921600;
        case OSG_UART_BAUDRATE_1000000: return 1000000;
        case OSG_UART_BAUDRATE_2000000: return 2000000;
        default: osg_error("ERROR: Invalid UART Baud Rate.");
    }

    // suppress warning
    return 0;
}

static lpuart_data_bits_t _osg_board_Uart_decodeUartBits(osg_UartBits b)
{
    switch (b)
    {
        case OSG_UART_BITS_8: return kLPUART_EightDataBits;
        default: osg_error("ERROR: Invalid UART Word length.");
    }

    // suppress warning
    return kLPUART_EightDataBits;
}

static lpuart_stop_bit_count_t _osg_board_Uart_decodeUartStopBits(osg_UartStopBits sb)
{
    switch (sb)
    {
        case OSG_UART_STOPBITS_1: return kLPUART_OneStopBit;
        case OSG_UART_STOPBITS_2: return kLPUART_TwoStopBit;
        default: osg_error("ERROR: Invalid UART Stop Bit.");
    }

    //suppress warning
    return kLPUART_OneStopBit;
}

static lpuart_parity_mode_t _osg_board_Uart_decodeUartParity(osg_UartParity p)
{
    switch (p)
    {
        case OSG_UART_PARITY_NONE: return kLPUART_ParityDisabled;
        case OSG_UART_PARITY_EVEN: return kLPUART_ParityEven;
        case OSG_UART_PARITY_ODD: return kLPUART_ParityOdd;
        default: osg_error("ERROR: Invalid UART Parity.");
    }

    // suppress warning
    return kLPUART_ParityDisabled;
}

static bool _osg_board_Uart_decodeUartRtsEnabled(osg_UartFlowControl fc)
{
    switch (fc)
    {
        case OSG_UART_FLOWCONTROL_RTS:
        case OSG_UART_FLOWCONTROL_RTS_CTS:
            return true;
        default:
            return false;
    }
}

static bool _osg_board_Uart_decodeUartCtsEnabled(osg_UartFlowControl fc)
{
    switch (fc)
    {
        case OSG_UART_FLOWCONTROL_CTS:
        case OSG_UART_FLOWCONTROL_RTS_CTS:
            return true;
        default:
            return false;
    }
}

static void _osg_board_Uart_enableClock(osg_UartId id)
{
    CLOCK_SetLpuartClock(2U);
}

Bool _osg_board_Uart_checkUart(osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0:
#endif
#ifdef LPUART1
        case OSG_UART1:
#endif
#ifdef LPUART2
        case OSG_UART2:
#endif
#ifdef LPUART3
        case OSG_UART3:
#endif
#ifdef LPUART4
        case OSG_UART4:
#endif
#ifdef LPUART5
        case OSG_UART5:
#endif
            return TRUE;
        default:
            return FALSE;
    }
}

Bool _osg_board_Uart_validateTxPin(const osg_UartId id, const osg_GpioId tx)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0:
        {
            if (tx == OSG_GPIO_PA2
                || tx == OSG_GPIO_PA14
                || tx == OSG_GPIO_PB15
                || tx == OSG_GPIO_PB17
                || tx == OSG_GPIO_PC24
                || tx == OSG_GPIO_PD7)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART1
        case OSG_UART1:
        {
            if (tx == OSG_GPIO_PE0
                || tx == OSG_GPIO_PC4
                || tx == OSG_GPIO_PD9)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART2
        case OSG_UART2:
        {
            if (tx == OSG_GPIO_PE12
                || tx == OSG_GPIO_PE16
                || tx == OSG_GPIO_PA24
                || tx == OSG_GPIO_PD3)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART3
        case OSG_UART3:
        {
            if (tx == OSG_GPIO_PE4
                || tx == OSG_GPIO_PA28
                || tx == OSG_GPIO_PB11
                || tx == OSG_GPIO_PC17)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART4
        case OSG_UART4:
        {
            if (tx == OSG_GPIO_PE20
                || tx == OSG_GPIO_PA20
                || tx == OSG_GPIO_PC15)
            {
                return TRUE;
            }

            break;
        }
#endif
        default:
            return FALSE;
    }

    return FALSE;
}

Bool _osg_board_Uart_validateRxPin(const osg_UartId id, const osg_GpioId rx)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0:
        {
            if (rx == OSG_GPIO_PA1
                || rx == OSG_GPIO_PA15
                || rx == OSG_GPIO_PB14
                || rx == OSG_GPIO_PB16
                || rx == OSG_GPIO_PC25
                || rx == OSG_GPIO_PD6)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART1
        case OSG_UART1:
        {
            if (rx == OSG_GPIO_PE1
                || rx == OSG_GPIO_PC3
                || rx == OSG_GPIO_PD8)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART2
        case OSG_UART2:
        {
            if (rx == OSG_GPIO_PE13
                || rx == OSG_GPIO_PE17
                || rx == OSG_GPIO_PA25
                || rx == OSG_GPIO_PD2)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART3
        case OSG_UART3:
        {
            if (rx == OSG_GPIO_PE5
                || rx == OSG_GPIO_PA29
                || rx == OSG_GPIO_PB10
                || rx == OSG_GPIO_PC16)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART4
        case OSG_UART4:
        {
            if (rx == OSG_GPIO_PE21
                || rx == OSG_GPIO_PA21
                || rx == OSG_GPIO_PC14)
            {
                return TRUE;
            }

            break;
        }
#endif
        default:
            return FALSE;
    }

    return FALSE;
}

Bool _osg_board_Uart_validateCtsPin(const osg_UartId id, const osg_GpioId cts)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0:
        {
            if (cts == OSG_GPIO_PA0
                || cts == OSG_GPIO_PA16
                || cts == OSG_GPIO_PB3
                || cts == OSG_GPIO_PB13
                || cts == OSG_GPIO_PC26
                || cts == OSG_GPIO_PD5)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART1
        case OSG_UART1:
        {
            if (cts == OSG_GPIO_PE2
                || cts == OSG_GPIO_PC2
                || cts == OSG_GPIO_PD11)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART2
        case OSG_UART2:
        {
            if (cts == OSG_GPIO_PE18
                || cts == OSG_GPIO_PA26
                || cts == OSG_GPIO_PD1)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART3
        case OSG_UART3:
        {
            if (cts == OSG_GPIO_PE6
                || cts == OSG_GPIO_PA30
                || cts == OSG_GPIO_PB9
                || cts == OSG_GPIO_PC19)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART4
        case OSG_UART4:
        {
            if (cts == OSG_GPIO_PE22
                || cts == OSG_GPIO_PA22
                || cts == OSG_GPIO_PC13)
            {
                return TRUE;
            }

            break;
        }
#endif
        default:
            return FALSE;
    }

    return FALSE;
}

Bool _osg_board_Uart_validateRtsPin(const osg_UartId id, const osg_GpioId rts)
{
    switch (id)
    {
#ifdef LPUART0
        case OSG_UART0:
        {
            if (rts == OSG_GPIO_PA3
                || rts == OSG_GPIO_PA17
                || rts == OSG_GPIO_PB2
                || rts == OSG_GPIO_PB12
                || rts == OSG_GPIO_PC27
                || rts == OSG_GPIO_PD4)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART1
        case OSG_UART1:
        {
            if (rts == OSG_GPIO_PE3
                || rts == OSG_GPIO_PC1
                || rts == OSG_GPIO_PD10)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART2
        case OSG_UART2:
        {
            if (rts == OSG_GPIO_PE19
                || rts == OSG_GPIO_PA27
                || rts == OSG_GPIO_PD0)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART3
        case OSG_UART3:
        {
            if (rts == OSG_GPIO_PE7
                || rts == OSG_GPIO_PA31
                || rts == OSG_GPIO_PB8
                || rts == OSG_GPIO_PC18)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef LPUART4
        case OSG_UART4:
        {
            if (rts == OSG_GPIO_PE23
                || rts == OSG_GPIO_PA23
                || rts == OSG_GPIO_PC12)
            {
                return TRUE;
            }

            break;
        }
#endif
        default:
            return FALSE;
    }

    return FALSE;
}

void osg_board_Uart_ctor(osg_Uart * self, const osg_UartBaudRate baud, const osg_UartParity parity, const osg_UartBits bits, const osg_UartStopBits stopBits, const osg_UartFlowControl flow, void * circularTxBuffer, Size txBufferSize, void * circularRxBuffer, Size rxBufferSize)
{
    if (_osg_board_Uart_checkUart(self->id) == FALSE)
    {
        osg_error("ERROR: UART port NOT available on this device!");
    }

    if (_osg_board_Uart_validateTxPin(self->id, self->txPin.id) == FALSE)
        osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");

    if (_osg_board_Uart_validateRxPin(self->id, self->rxPin.id) == FALSE)
        osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");

    if (flow == OSG_UART_FLOWCONTROL_CTS
        || flow == OSG_UART_FLOWCONTROL_RTS_CTS)
    {
        if (_osg_board_Uart_validateCtsPin(self->id, self->ctsPin.id) == FALSE)
            osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");
    }

    if (flow == OSG_UART_FLOWCONTROL_RTS
        || flow == OSG_UART_FLOWCONTROL_RTS_CTS)
    {
        if (_osg_board_Uart_validateRtsPin(self->id, self->rtsPin.id) == FALSE)
            osg_error("ERROR: Impossible to use these PINs for this UART port on this board!");
    }

    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->handler = _osg_board_Uart_getUartInstance(self->id);
    uart->type = _osg_board_Uart_getUartType(self->id);

    osg_CircularFifo_ctor(&uart->rxFifo, circularRxBuffer, rxBufferSize, OSG_FIFO_OVERWRITE_NO_ERROR);
    osg_CircularFifo_ctor(&uart->txFifo, circularTxBuffer, txBufferSize, OSG_FIFO_ERROR);
    uart->flags = OSG_UART_FLAG_DEFAULT;

    if (uart->type == OSG_LPUART)
    {
        lpuart_config_t config;
        LPUART_GetDefaultConfig(&config);
        config.baudRate_Bps = _osg_board_Uart_decodeUartBaudRate(baud);
        config.dataBitsCount = _osg_board_Uart_decodeUartBits(bits);
        config.stopBitCount = _osg_board_Uart_decodeUartStopBits(stopBits);
        config.parityMode = _osg_board_Uart_decodeUartParity(parity);
        config.enableRx = true;
        config.enableTx = true;
        config.enableRxRTS = _osg_board_Uart_decodeUartRtsEnabled(flow);
        config.enableTxCTS = _osg_board_Uart_decodeUartCtsEnabled(flow);

        const status_t initErr = LPUART_Init((osg_board_UartInstance *)uart->handler, &config, CLOCK_GetFreq(kCLOCK_Osc0ErClk));
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
    osg_CircularFifo_dtor(&uart->txFifo);
    osg_CircularFifo_dtor(&uart->rxFifo);
    uart->handler = NULL;
    uart->flags = OSG_UART_FLAG_DEFAULT;
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


Bool osg_board_Uart_sendBlocking(osg_Uart * self, const void * buffer, const Size size, const uint32_t timeout)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    int state = -1;
    osg_Timer timer;
    osg_Timer_ctor(&timer, OSG_TIMER_ONE_SHOT, _osg_board_Uart_timeoutCallback, &state);
    
    if (uart->type == OSG_LPUART)
    {
        lpuart_transfer_t xfer;
        xfer.data = (uint8_t *)buffer;
        xfer.dataSize = size;
        osg_board_UartHandler * handle = _osg_board_Uart_getUartHandler(self->id);
        /*LPUART_WriteBlocking((osg_board_UartInstance *)uart->handler, (uint8_t *)buffer, (uint16_t)size);
        while (1)
        {
            if (state == kStatus_Timeout)
            {
                osg_Timer_stop(&timer);
                osg_Timer_dtor(&timer);
                return FALSE;
            }

            if (LPUART_GetStatusFlags((osg_board_UartInstance *)uart->handler) & kLPUART_TransmissionCompleteFlag)
            {
                osg_Timer_stop(&timer);
                osg_Timer_dtor(&timer);
                return TRUE;
            }
        }*/

        uart->flags &= ~OSG_UART_FLAG_HANDLE_CREATED;
        LPUART_TransferCreateHandle((osg_board_UartInstance *)uart->handler, handle, _osg_board_Uart_sendReceiveCallback, &state);
        const status_t status = LPUART_TransferSendNonBlocking((osg_board_UartInstance *)uart->handler, handle, &xfer);

        if (status != kStatus_Success)
        {
            osg_Timer_dtor(&timer);
            return FALSE;
        }
        
        osg_Timer_start(&timer, timeout);
        while (1)
        {
            if (state == kStatus_LPUART_TxIdle)
            {
                osg_Timer_stop(&timer);
                osg_Timer_dtor(&timer);
                return TRUE;
            }

            if (state == kStatus_Timeout)
                break;
        }

        osg_Timer_stop(&timer);
        osg_Timer_dtor(&timer);
        return FALSE;
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    osg_Timer_dtor(&timer);
    return FALSE;
}

Bool osg_board_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size)
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
        const Bool ret = status == kStatus_Success ? TRUE : FALSE;

        if (!ret)
        {
            uart->flags = restore;
        }

        return ret;
    }
    else
        osg_error("ERROR: Invalid UART type.");

    return FALSE;
}

Bool osg_board_Uart_sendBuffered(osg_Uart * self, const void * buffer, const Size bufferSize)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);

    if (osg_CircularFifo_isNull(&uart->txFifo) == TRUE)
        return FALSE;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&uart->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return FALSE;

    _osg_board_Uart_doBufferedSend(uart);

    return TRUE;
}

Bool osg_board_Uart_receiveBlocking(osg_Uart * self, void * buffer, const Size size, const uint32_t timeout)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED) return FALSE;
    int state = -1;
    osg_Timer timer;
    osg_Timer_ctor(&timer, OSG_TIMER_ONE_SHOT, _osg_board_Uart_timeoutCallback, &state);

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
            return FALSE;
        }

        osg_Timer_start(&timer, timeout);
        while (1)
        {
            if (state == kStatus_LPUART_RxIdle)
            {
                osg_Timer_stop(&timer);
                osg_Timer_dtor(&timer);
                return TRUE;
            }

            if (state == kStatus_Timeout)
                break;
        }

        osg_Timer_stop(&timer);
        osg_Timer_dtor(&timer);
        return FALSE;
    }
    else
    {
        osg_error("ERROR: Invalid UART type.");
    }

    osg_Timer_dtor(&timer);
    return FALSE;
}

Bool osg_board_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);

    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED) return FALSE;
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
        const Bool ret = status == kStatus_Success ? TRUE : FALSE;

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

    return FALSE;
}

Size osg_board_Uart_receiveBuffered(osg_Uart * self, void * buffer, const Size bufferSize)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    return osg_CircularFifo_popBuffer(&uart->rxFifo, buffer, bufferSize);
}

void osg_board_Uart_startReceiveBuffered(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->flags |= OSG_UART_FLAG_RX_BUFF_ENABLED | OSG_UART_FLAG_IS_RECEIVE;
    _osg_board_Uart_doBufferedReceive(uart);
}

void osg_board_Uart_stopReceiveBuffered(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
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

Bool osg_board_Uart_isReceiveBufferedEnabled(osg_Uart * self)
{
    if (_osg_board_Uart_getUartById(self->id)->flags & OSG_UART_FLAG_RX_BUFF_ENABLED)
        return TRUE;

    return FALSE;
}

Size osg_board_Uart_getReceivedCharsRxBuffer(osg_Uart *self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (osg_CircularFifo_isNull(&uart->rxFifo) == FALSE)
        return osg_CircularFifo_getSize(&uart->rxFifo);
    
    return 0;
}

void HardFault_Handler(void)
{
    osg_error("HW ERROR.");
}
