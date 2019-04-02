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
#include "board-UartSupport.h"

#define TOT_UART FSL_FEATURE_SOC_LPUART_COUNT

static osg_UartImpl _osg_board_Uart_devices[TOT_UART];
static osg_board_UartHandler _osg_board_Uart_handlers[TOT_UART];


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

uint8_t _osg_board_Uart_getUarts()
{
    return (uint8_t)TOT_UART;
}

uint8_t _osg_board_Uart_getDeviceIndex(const osg_UartId id)
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

osg_UartImpl * _osg_board_Uart_getUartByInstance(void * handler)
{
    for (int i = 0; i < TOT_UART; ++i)
    {
        if (_osg_board_Uart_devices[i].handler == handler) return &_osg_board_Uart_devices[i];
    }

    return NULL;
}

osg_UartImpl * _osg_board_Uart_getUartById(const osg_UartId id)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(id);
    return &_osg_board_Uart_devices[index];
}

osg_board_UartInstance * _osg_board_Uart_getUartInstance(const osg_UartId id)
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

osg_board_UartHandler * _osg_board_Uart_getUartHandler(const osg_UartId id)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(id);
    return &_osg_board_Uart_handlers[index];
}

osg_board_UartHandler * _osg_board_Uart_getUartHandlerFromInstance(const osg_board_UartInstance * instance)
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


// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////

bool _osg_board_Uart_checkUart(const osg_UartId id)
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
            return true;
        default:
            return false;
    }
}

bool _osg_board_Uart_validateTxPin(const osg_UartId id, const osg_GpioId tx)
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
                return true;
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
                return true;
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
                return true;
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
                return true;
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
                return true;
            }

            break;
        }
#endif
        default:
            return false;
    }

    return false;
}

bool _osg_board_Uart_validateRxPin(const osg_UartId id, const osg_GpioId rx)
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
                return true;
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
                return true;
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
                return true;
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
                return true;
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
                return true;
            }

            break;
        }
#endif
        default:
            return false;
    }

    return false;
}

bool _osg_board_Uart_validateCtsPin(const osg_UartId id, const osg_GpioId cts)
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
                return true;
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
                return true;
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
                return true;
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
                return true;
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
                return true;
            }

            break;
        }
#endif
        default:
            return false;
    }

    return false;
}

bool _osg_board_Uart_validateRtsPin(const osg_UartId id, const osg_GpioId rts)
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
                return true;
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
                return true;
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
                return true;
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
                return true;
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
                return true;
            }

            break;
        }
#endif
        default:
            return false;
    }

    return false;
}


// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////

uint32_t _osg_board_Uart_decodeUartBaudRate(const osg_UartBaudRate br)
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

lpuart_data_bits_t _osg_board_Uart_decodeUartBits(osg_UartBits b, const osg_UartParity p)
{
    if (p != OSG_UART_PARITY_NONE)
    {
        osg_assert(b != OSG_UART_BITS_5, "ASSERT FAILED: Invalid UART word bits and parity.");
        b--;
    }
    
    switch (b)
    {
        case OSG_UART_BITS_8: return kLPUART_EightDataBits;
        default: osg_error("ERROR: Invalid UART Word length.");
    }

    // suppress warning
    return kLPUART_EightDataBits;
}

lpuart_stop_bit_count_t _osg_board_Uart_decodeUartStopBits(const osg_UartStopBits sb)
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

lpuart_parity_mode_t _osg_board_Uart_decodeUartParity(const osg_UartParity p)
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

bool _osg_board_Uart_decodeUartCtsEnabled(const osg_UartFlowControl fc)
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

bool _osg_board_Uart_decodeUartRtsEnabled(const osg_UartFlowControl fc)
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

//port_mux_t _osg_board_Uart_decodePinMuxAlt(const osg_GpioId pin)
//{
//    switch (pin)
//    {
//        case OSG_GPIO_PA0:
//        case OSG_GPIO_PA1:
//        case OSG_GPIO_PA2:
//        case OSG_GPIO_PA3:
//        case OSG_GPIO_PB12:
//        case OSG_GPIO_PB13:
//        case OSG_GPIO_PB14:
//        case OSG_GPIO_PB15:
//            return kPORT_MuxAlt2;
//        case OSG_GPIO_PA14:
//        case OSG_GPIO_PA15:
//        case OSG_GPIO_PA16:
//        case OSG_GPIO_PA17:
//        case OSG_GPIO_PA20:
//        case OSG_GPIO_PA21:
//        case OSG_GPIO_PA22:
//        case OSG_GPIO_PA23:
//        case OSG_GPIO_PA24:
//        case OSG_GPIO_PA25:
//        case OSG_GPIO_PA26:
//        case OSG_GPIO_PA27:
//        case OSG_GPIO_PA28:
//        case OSG_GPIO_PA29:
//        case OSG_GPIO_PA30:
//        case OSG_GPIO_PA31:
//        case OSG_GPIO_PB2:
//        case OSG_GPIO_PB3:
//        case OSG_GPIO_PB8:
//        case OSG_GPIO_PB9:
//        case OSG_GPIO_PB10:
//        case OSG_GPIO_PB11:
//        case OSG_GPIO_PB16:
//        case OSG_GPIO_PB17:
//        case OSG_GPIO_PC1:
//        case OSG_GPIO_PC2:
//        case OSG_GPIO_PC3:
//        case OSG_GPIO_PC4:
//        case OSG_GPIO_PC12:
//        case OSG_GPIO_PC13:
//        case OSG_GPIO_PC14:
//        case OSG_GPIO_PC15:
//        case OSG_GPIO_PC16:
//        case OSG_GPIO_PC17:
//        case OSG_GPIO_PC18:
//        case OSG_GPIO_PC19:
//        case OSG_GPIO_PC24:
//        case OSG_GPIO_PC25:
//        case OSG_GPIO_PC26:
//        case OSG_GPIO_PC27:
//        case OSG_GPIO_PD0:
//        case OSG_GPIO_PD1:
//        case OSG_GPIO_PD2:
//        case OSG_GPIO_PD3:
//        case OSG_GPIO_PD4:
//        case OSG_GPIO_PD5:
//        case OSG_GPIO_PD6:
//        case OSG_GPIO_PD7:
//        case OSG_GPIO_PD8:
//        case OSG_GPIO_PD9:
//        case OSG_GPIO_PD10:
//        case OSG_GPIO_PD11:
//        case OSG_GPIO_PE0:
//        case OSG_GPIO_PE1:
//        case OSG_GPIO_PE2:
//        case OSG_GPIO_PE3:
//        case OSG_GPIO_PE4:
//        case OSG_GPIO_PE5:
//        case OSG_GPIO_PE6:
//        case OSG_GPIO_PE7:
//        case OSG_GPIO_PE12:
//        case OSG_GPIO_PE13:
//        case OSG_GPIO_PE16:
//        case OSG_GPIO_PE17:
//        case OSG_GPIO_PE18:
//        case OSG_GPIO_PE19:
//        case OSG_GPIO_PE20:
//        case OSG_GPIO_PE21:
//        case OSG_GPIO_PE22:
//        case OSG_GPIO_PE23:
//            return kPORT_MuxAlt3;
//        default:
//            osg_error("ERROR: PIN can't be used for UART.");
//    }
//
//    return kPORT_PinDisabledOrAnalog;
//}


// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////

//osg_GpioAlternateFunction _osg_board_Uart_encodeAlternateFunction(const osg_UartId id)
//{
//    switch (id)
//    {
//#ifdef LPUART0
//        case OSG_UART0:
//        {
//            return OSG_GPIO_ALTERNATE_UART0;
//        }
//#endif
//#ifdef LPUART1
//        case OSG_UART1:
//        {
//            return OSG_GPIO_ALTERNATE_UART1;
//        }
//#endif
//#ifdef LPUART2
//        case OSG_UART2:
//        {
//            return OSG_GPIO_ALTERNATE_UART2;
//        }
//#endif
//#ifdef LPUART3
//        case OSG_UART3:
//        {
//            return OSG_GPIO_ALTERNATE_UART3;
//        }
//#endif
//#ifdef LPUART4
//        case OSG_UART4:
//        {
//            return OSG_GPIO_ALTERNATE_UART4;
//        }
//#endif
//#ifdef LPUART5
//        case OSG_UART5:
//        {
//            return OSG_GPIO_ALTERNATE_UART5;
//        }
//#endif
//        default:
//            osg_error("ERROR: Impossible to set correct I2C Alternate Function to Pin.");
//    }
//
//    // suppress warning
//    return OSG_GPIO_ALTERNATE_DEFAULT;
//}

osg_UartType _osg_board_Uart_encodeUartType(const osg_UartId id)
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


// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////

void _osg_board_Uart_enableClock(const osg_UartId id)
{
    CLOCK_SetLpuartClock(2U);
}


// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////

void _osg_board_Uart_doBufferedSend(osg_UartImpl * uart)
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

void _osg_board_Uart_doBufferedReceive(osg_UartImpl * uart)
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
void _osg_board_Uart_sendReceiveCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
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
