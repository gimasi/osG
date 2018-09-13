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
#include <stm32l4xx_hal.h>

#ifdef LPUART1
#define COUNT_UART0 1
#else
#define COUNT_UART0 0
#endif
#ifdef USART1
#define COUNT_UART1 1
#else
#define COUNT_UART1 0
#endif
#ifdef USART2
#define COUNT_UART2 1
#else
#define COUNT_UART2 0
#endif
#ifdef USART3
#define COUNT_UART3 1
#else
#define COUNT_UART3 0
#endif
#ifdef UART4
#define COUNT_UART4 1
#else
#define COUNT_UART4 0
#endif
#ifdef UART5
#define COUNT_UART5 1
#else
#define COUNT_UART5 0
#endif
#ifdef UART6
#define COUNT_UART6 1
#else
#define COUNT_UART6 0
#endif

#define TOT_UART (COUNT_UART0 + COUNT_UART1 + COUNT_UART2 + COUNT_UART3 + COUNT_UART4 + COUNT_UART5 + COUNT_UART6)


static uint8_t _osg_board_Uart_getUarts()
{
    return (uint8_t)TOT_UART;
}

typedef UART_HandleTypeDef osg_board_UartHandler;
typedef USART_TypeDef osg_board_UartInstance;

static osg_UartImpl _osg_board_Uart_devices[TOT_UART];
static osg_board_UartHandler _osg_board_Uart_handlers[TOT_UART];

static osg_UartImpl * _osg_board_Uart_getUartByHandler(void * handler)
{
    for (int i = 0; i < TOT_UART; ++i)
    {
        if (_osg_board_Uart_devices[i].handler == handler) return &_osg_board_Uart_devices[i];
    }

    return NULL;
}

static uint8_t _osg_board_Uart_getDeviceIndex(osg_UartId id)
{
    uint8_t index = 0;

#ifdef LPUART1
    if (id == OSG_UART0)
        return index;
    ++index;
#endif
#ifdef USART1
    if (id == OSG_UART1)
        return index;
    ++index;
#endif
#ifdef USART2
    if (id == OSG_UART2)
        return index;
    ++index;
#endif
#ifdef USART3
    if (id == OSG_UART3)
        return index;
    ++index;
#endif
#ifdef UART4
    if (id == OSG_UART4)
        return index;
    ++index;
#endif
#ifdef UART5
    if (id == OSG_UART5)
        return index;
    ++index;
#endif
#ifdef UART6
    if (id == OSG_UART6)
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

static osg_board_UartInstance * _osg_board_Uart_getUartInstance(const osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART1
        case OSG_UART0:
            return LPUART1;
#endif
#ifdef USART1
        case OSG_UART1:
            return USART1;
#endif
#ifdef USART2
        case OSG_UART2:
            return USART2;
#endif
#ifdef USART3
        case OSG_UART3:
            return USART3;
#endif
#ifdef UART4
        case OSG_UART4:
            return UART4;
#endif
#ifdef UART5
        case OSG_UART5:
            return UART5;
#endif
#ifdef UART6
        case OSG_UART6:
            return UART6;
#endif
        default:
            osg_error("ERROR: Impossible to retrieve UART instance.");
    }

    // suppress warning
    return NULL;
}

static void _osg_board_Uart_doBufferedSend(osg_UartImpl * uart)
{
    if (osg_CircularFifo_isEmpty(&uart->txFifo)) return;
    const osg_UartFlags restore = uart->flags;
    uart->flags &= ~OSG_UART_FLAG_IS_NB_SEND;
    uart->flags |= OSG_UART_FLAG_IS_SEND;
    void * buffer = NULL;
    Size size = 0;
    osg_CircularFifo_getFilledBuffer(&uart->txFifo, &buffer, &size);

    osg_board_UartHandler * handler = (osg_board_UartHandler *)uart->handler;
    const HAL_StatusTypeDef ok = HAL_UART_Transmit_IT(handler, (uint8_t *)buffer, (uint16_t)size);
    if (ok != HAL_OK) uart->flags = restore;
}

static void _osg_board_Uart_doBufferedReceive(osg_UartImpl * uart)
{
    if (osg_CircularFifo_isNull(&uart->rxFifo)
            || !(uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED))
        return;
    const osg_UartFlags restore = uart->flags;
    uart->flags &= ~OSG_UART_FLAG_IS_NB_RECEIVE;
    uart->flags |= OSG_UART_FLAG_IS_RECEIVE;

    osg_board_UartHandler * handler = (osg_board_UartHandler *)uart->handler;
    const HAL_StatusTypeDef ok = HAL_UART_Receive_IT(handler, &uart->rxByte, 1);
    if (ok != HAL_OK) uart->flags = restore;
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
 //   osg_board_Uart * uart = Uart__getUartByHandler(huart);
 //   if (uart->_error != NULL) uart->_error(uart);

    // @TODO: what to do in case of tx buffer error?
    // buffer never became empty...
    osg_error("ERROR: UART communication error.");
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartByHandler(huart);
    if (uart->flags & OSG_UART_FLAG_IS_NB_SEND)
    {
        uart->flags &= ~OSG_UART_FLAG_IS_SEND;
        if (uart->txCompleteNb != NULL) uart->txCompleteNb();
    }
    else
    {
        osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;
        osg_CircularFifo_popBytes(&uart->txFifo, handler->TxXferSize - handler->TxXferCount);
        if (uart->txCompleteBuffered != NULL) uart->txCompleteBuffered();
    }
    uart->flags &= ~OSG_UART_FLAG_IS_NB_SEND;
   _osg_board_Uart_doBufferedSend(uart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartByHandler(huart);
    if (uart->flags & OSG_UART_FLAG_IS_NB_RECEIVE)
    {
        uart->flags &= ~OSG_UART_FLAG_IS_RECEIVE;
        if (uart->rxCompleteNb != NULL) uart->rxCompleteNb();
    }
    else
    {
        osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;
        osg_CircularFifo_pushBuffer(&uart->rxFifo, &uart->rxByte, 1);
        if (uart->rxCompleteBuffered != NULL) uart->rxCompleteBuffered();
    }
    uart->flags &= ~OSG_UART_FLAG_IS_NB_RECEIVE;
    _osg_board_Uart_doBufferedReceive(uart);
}

void HAL_UART_AbortReceiveCpltCallback (UART_HandleTypeDef *huart)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartByHandler(huart);
    uart->flags |= OSG_UART_FLAG_RX_ABORTED;
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

static uint32_t _osg_board_Uart_decodeUartBits(osg_UartBits b)
{
    switch (b)
    {
        case OSG_UART_BITS_7: return UART_WORDLENGTH_7B;
        case OSG_UART_BITS_8: return UART_WORDLENGTH_8B;
        case OSG_UART_BITS_9: return UART_WORDLENGTH_9B;
        default: osg_error("ERROR: Invalid UART Word length.");
    }

    // suppress warning
    return 0;
}

static uint32_t _osg_board_Uart_decodeUartStopBits(osg_UartStopBits sb)
{
    switch (sb)
    {
        case OSG_UART_STOPBITS_0_5: return UART_STOPBITS_0_5;
        case OSG_UART_STOPBITS_1: return UART_STOPBITS_1;
        case OSG_UART_STOPBITS_1_5: return UART_STOPBITS_1_5;
        case OSG_UART_STOPBITS_2: return UART_STOPBITS_2;
        default: osg_error("ERROR: Invalid UART Stop Bit.");
    }

    //suppress warning
    return 0;
}

static uint32_t _osg_board_Uart_decodeUartParity(osg_UartParity p)
{
    switch (p)
    {
        case OSG_UART_PARITY_NONE: return UART_PARITY_NONE;
        case OSG_UART_PARITY_EVEN: return UART_PARITY_EVEN;
        case OSG_UART_PARITY_ODD: return UART_PARITY_ODD;
        default: osg_error("ERROR: Invalid UART Parity.");
    }

    // suppress warning
    return 0;
}

static uint32_t _osg_board_Uart_decodeUartFlowControl(osg_UartFlowControl fc)
{
    switch (fc)
    {
        case OSG_UART_FLOWCONTROL_NONE: return UART_HWCONTROL_NONE;
        case OSG_UART_FLOWCONTROL_RTS: return UART_HWCONTROL_RTS;
        case OSG_UART_FLOWCONTROL_CTS: return UART_HWCONTROL_CTS;
        case OSG_UART_FLOWCONTROL_RTS_CTS: return UART_HWCONTROL_RTS_CTS;
        default: osg_error("ERROR: Invalid UART Flow Control.");
    }

    // suppress warning
    return 0;
}

static void _osg_board_Uart_enableClock(osg_UartId id)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    
    switch(id)
    {
#ifdef LPUART1
        case OSG_UART0:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
            PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_LPUART1_CLK_ENABLE();
            break;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
            PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_USART1_CLK_ENABLE();
            break;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
            PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_USART2_CLK_ENABLE();
            break;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
            PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_USART3_CLK_ENABLE();
            break;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4;
            PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_UART4_CLK_ENABLE();
            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART5;
            PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_UART5_CLK_ENABLE();
            break;
        }
#endif
#ifdef UART6
        case OSG_UART6:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART6;
            PeriphClkInit.Uart6ClockSelection = RCC_UART6CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_UART6_CLK_ENABLE();
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid UART Port in UART initialization.");
    }
}

static void _osg_board_Uart_enable(osg_board_UartHandler * handler, const osg_UartType type)
{
    __HAL_UART_ENABLE(handler);
}

static void _osg_board_Uart_enableInterrupt(osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART1
        case OSG_UART0:
        {
            HAL_NVIC_SetPriority(LPUART1_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(LPUART1_IRQn);
            break;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
            break;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
            break;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(USART3_IRQn);
            break;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            HAL_NVIC_SetPriority(UART4_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(UART4_IRQn);
            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            HAL_NVIC_SetPriority(UART5_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(UART5_IRQn);
            break;
        }
#endif
#ifdef UART6
        case OSG_UART6:
        {
            HAL_NVIC_SetPriority(UART6_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(UART6_IRQn);
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid UART port; impossible to enable the correct interrupt.");
    }
}

static void _osg_board_Uart_resetUart(osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART1
        case OSG_UART0:
        {
            __HAL_RCC_LPUART1_FORCE_RESET();
            __HAL_RCC_LPUART1_RELEASE_RESET();
            __HAL_RCC_LPUART1_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(LPUART1_IRQn);
            break;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            __HAL_RCC_USART1_FORCE_RESET();
            __HAL_RCC_USART1_RELEASE_RESET();
            __HAL_RCC_USART1_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(USART1_IRQn);
            break;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            __HAL_RCC_USART2_FORCE_RESET();
            __HAL_RCC_USART2_RELEASE_RESET();
            __HAL_RCC_USART2_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(USART2_IRQn);
            break;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            __HAL_RCC_USART3_FORCE_RESET();
            __HAL_RCC_USART3_RELEASE_RESET();
            __HAL_RCC_USART3_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(USART3_IRQn);
            break;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            __HAL_RCC_UART4_FORCE_RESET();
            __HAL_RCC_UART4_RELEASE_RESET();
            __HAL_RCC_UART4_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(UART4_IRQn);
            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            __HAL_RCC_UART5_FORCE_RESET();
            __HAL_RCC_UART5_RELEASE_RESET();
            __HAL_RCC_UART5_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(UART5_IRQn);
            break;
        }
#endif
#ifdef UART6
        case OSG_UART6:
        {
            __HAL_RCC_UART6_FORCE_RESET();
            __HAL_RCC_UART6_RELEASE_RESET();
            __HAL_RCC_UART6_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(UART6_IRQn);
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid UART Port in UART reset.");
    }
}

static osg_UartType _osg_board_Uart_getUartType(const osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART1
        case OSG_UART0: return OSG_LPUART;
#endif
#ifdef USART1
        case OSG_UART1: return OSG_USART;
#endif
#ifdef USART2
        case OSG_UART2: return OSG_USART;
#endif
#ifdef USART3
        case OSG_UART3: return OSG_USART;
#endif
#ifdef UART4
        case OSG_UART4: return OSG_UART;
#endif
#ifdef UART5
        case OSG_UART5: return OSG_UART;
#endif
#ifdef UART6
        case OSG_UART6: return OSG_UART;
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
#ifdef LPUART1
        case OSG_UART0:
        {
            return OSG_GPIO_ALTERNATE_UART0;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            return OSG_GPIO_ALTERNATE_UART1;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            return OSG_GPIO_ALTERNATE_UART2;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            return OSG_GPIO_ALTERNATE_UART3;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            return OSG_GPIO_ALTERNATE_UART4;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            return OSG_GPIO_ALTERNATE_UART5;
        }
#endif
#ifdef UART6
        case OSG_UART6:
        {
            return OSG_GPIO_ALTERNATE_UART6;
        }
#endif
        default:
            osg_error("ERROR: Impossible to set correct UART Alternate Function to Pin.");
    }

    // suppress warning
    return OSG_GPIO_ALTERNATE_DEFAULT;
}

Bool _osg_board_Uart_checkUart(osg_UartId id)
{
    switch (id)
    {
#ifdef LPUART1
        case OSG_UART0:
#endif
#ifdef USART1
        case OSG_UART1:
#endif
#ifdef USART2
        case OSG_UART2:
#endif
#ifdef USART3
        case OSG_UART3:
#endif
#ifdef UART4
        case OSG_UART4:
#endif
#ifdef UART5
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
#ifdef LPUART1
        case OSG_UART0:
        {
            if (tx == OSG_GPIO_PC1
                || tx == OSG_GPIO_PB11)
            {
                return TRUE;
            }
        }

        break;
#endif
#ifdef USART1
        case OSG_UART1:
        {
            if (tx == OSG_GPIO_PA9
                || tx == OSG_GPIO_PB6)
            {
                return TRUE;
            }
        }

        break;
#endif
#ifdef USART2
        case OSG_UART2:
        {
            if (tx == OSG_GPIO_PA2)
            {
                return TRUE;
            }
        }

        break;
#endif
#ifdef USART3
        case OSG_UART3:
        {
            if (tx == OSG_GPIO_PC4
                || tx == OSG_GPIO_PB10
                || tx == OSG_GPIO_PC10)
            {
                return TRUE;
            }
        }

        break;
#endif
#ifdef UART4
        case OSG_UART4:
        {
            if (tx == OSG_GPIO_PA0
                || tx == OSG_GPIO_PC10)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            if (tx == OSG_GPIO_PC12)
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
#ifdef LPUART1
        case OSG_UART0:
        {
            if (rx == OSG_GPIO_PC0
                || rx == OSG_GPIO_PB10)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            if (rx == OSG_GPIO_PA10
                || rx == OSG_GPIO_PB7)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            if (rx == OSG_GPIO_PA3)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            if (rx == OSG_GPIO_PC5
                || rx == OSG_GPIO_PB11
                || rx == OSG_GPIO_PC11)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            if (rx == OSG_GPIO_PA1
                || rx == OSG_GPIO_PC11)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            if (rx == OSG_GPIO_PD2)
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
#ifdef LPUART1
        case OSG_UART0:
        {
            if (cts == OSG_GPIO_PB13)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            if (cts == OSG_GPIO_PA11
                || cts == OSG_GPIO_PB4)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            if (cts == OSG_GPIO_PA0)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            if (cts == OSG_GPIO_PA6
                || cts == OSG_GPIO_PB13)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            if (cts == OSG_GPIO_PB7)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            if (cts == OSG_GPIO_PB5)
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
#ifdef LPUART1
        case OSG_UART0:
        {
            if (rts == OSG_GPIO_PB12)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART1
        case OSG_UART1:
        {
            if (rts == OSG_GPIO_PA12
                || rts == OSG_GPIO_PB3)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART2
        case OSG_UART2:
        {
            if (rts == OSG_GPIO_PA1)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef USART3
        case OSG_UART3:
        {
            if (rts == OSG_GPIO_PB1
                || rts == OSG_GPIO_PB14
                || rts == OSG_GPIO_PD2)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART4
        case OSG_UART4:
        {
            if (rts == OSG_GPIO_PA15)
            {
                return TRUE;
            }

            break;
        }
#endif
#ifdef UART5
        case OSG_UART5:
        {
            if (rts == OSG_GPIO_PB4)
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

    _osg_board_Uart_enableClock(self->id);

    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    uart->handler = _osg_board_Uart_getUartHandler(self->id);
    uart->type = _osg_board_Uart_getUartType(self->id);

    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;
    handler->Instance = _osg_board_Uart_getUartInstance(self->id);
    handler->Init.Mode       = UART_MODE_TX_RX;
    handler->Init.BaudRate   = _osg_board_Uart_decodeUartBaudRate(baud);
    handler->Init.WordLength = _osg_board_Uart_decodeUartBits(bits);
    handler->Init.StopBits   = _osg_board_Uart_decodeUartStopBits(stopBits);
    handler->Init.Parity     = _osg_board_Uart_decodeUartParity(parity);
    handler->Init.OverSampling = UART_OVERSAMPLING_16;
    handler->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    handler->Init.HwFlowCtl  = _osg_board_Uart_decodeUartFlowControl(flow);
    handler->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    osg_CircularFifo_ctor(&uart->rxFifo, circularRxBuffer, rxBufferSize, OSG_FIFO_OVERWRITE_NO_ERROR);
    osg_CircularFifo_ctor(&uart->txFifo, circularTxBuffer, txBufferSize, OSG_FIFO_ERROR);
    uart->flags = OSG_UART_FLAG_DEFAULT;

    _osg_board_Uart_enable(handler, uart->type);

    if (HAL_UART_DeInit(handler) != HAL_OK)
    {
        osg_error("ERROR: UART deinit fails.");
    }
    
    const HAL_StatusTypeDef initErr = HAL_UART_Init(handler);
    if (initErr != HAL_OK) osg_error("ERROR: UART init failed");

    _osg_board_Uart_enableInterrupt(self->id);
}


void osg_board_Uart_dtor(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;
    HAL_UART_DeInit(handler);
    _osg_board_Uart_resetUart(self->id);

    uart->txCompleteNb = NULL;
    uart->rxCompleteNb = NULL;
    uart->txCompleteBuffered = NULL;
    uart->rxCompleteBuffered = NULL;
    osg_CircularFifo_dtor(&uart->txFifo);
    osg_CircularFifo_dtor(&uart->rxFifo);
    uart->handler = NULL;
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
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    const HAL_StatusTypeDef ret = HAL_UART_Transmit(handler, (uint8_t *)buffer, (uint16_t)size, timeout);
    return ret == HAL_OK ? TRUE : FALSE;
}

Bool osg_board_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    const osg_UartFlags restore = uart->flags;
    uart->flags |= OSG_UART_FLAG_IS_NB_SEND | OSG_UART_FLAG_IS_SEND;

    const HAL_StatusTypeDef status = HAL_UART_Transmit_IT(handler, (uint8_t *)buffer, (uint16_t)size);
    const Bool ret = status == HAL_OK ? TRUE : FALSE;
    if (!ret)
    {
        uart->flags = restore;
    }

    return ret;
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

    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    const HAL_StatusTypeDef ret = HAL_UART_Receive(handler, (uint8_t *)buffer, (uint16_t)size, timeout);
    return ret == HAL_OK ? TRUE : FALSE;
}

Bool osg_board_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    osg_board_UartHandler * handler = (osg_board_UartHandler *) uart->handler;

    if (uart->flags & OSG_UART_FLAG_RX_BUFF_ENABLED) return FALSE;
    const osg_UartFlags restore = uart->flags;
    uart->flags |= OSG_UART_FLAG_IS_NB_RECEIVE | OSG_UART_FLAG_IS_RECEIVE;

    const HAL_StatusTypeDef status = HAL_UART_Receive_IT(handler, (uint8_t *)buffer, (uint16_t)size);
    const Bool ret = status == HAL_OK ? TRUE : FALSE;
    if (!ret)
    {
        uart->flags = restore;
    }

    return ret;
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

Bool osg_board_Uart_isReceiveBufferedEnabled(osg_Uart * self)
{
    if (_osg_board_Uart_getUartById(self->id)->flags & OSG_UART_FLAG_RX_BUFF_ENABLED)
        return TRUE;

    return FALSE;
}

Size osg_board_Uart_getReceivedCharsRxBuffer(osg_Uart * self)
{
    osg_UartImpl * uart = _osg_board_Uart_getUartById(self->id);
    if (osg_CircularFifo_isNull(&uart->rxFifo) == FALSE)
        return osg_CircularFifo_getSize(&uart->rxFifo);
    
    return 0;
}


#ifdef LPUART1
void LPUART1_IRQHandler(void)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(OSG_UART0);
    HAL_UART_IRQHandler(&_osg_board_Uart_handlers[index]);
}
#endif
#ifdef USART1
void USART1_IRQHandler(void)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(OSG_UART1);
    HAL_UART_IRQHandler(&_osg_board_Uart_handlers[index]);
}
#endif
#ifdef USART2
void USART2_IRQHandler(void)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(OSG_UART2);
    HAL_UART_IRQHandler(&_osg_board_Uart_handlers[index]);
}
#endif
#ifdef USART3
void USART3_IRQHandler(void)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(OSG_UART3);
    HAL_UART_IRQHandler(&_osg_board_Uart_handlers[index]);
}
#endif
#ifdef UART4
void UART4_IRQHandler(void)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(OSG_UART4);
    HAL_UART_IRQHandler(&_osg_board_Uart_handlers[index]);
}
#endif
#ifdef UART5
void UART5_IRQHandler(void)
{
    uint8_t index = _osg_board_Uart_getDeviceIndex(OSG_UART5);
    HAL_UART_IRQHandler(&_osg_board_Uart_handlers[index]);
}
#endif
