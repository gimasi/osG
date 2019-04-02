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
#ifndef OSG_BOARD_DRIVERS_UARTSUPPORT_H
#define OSG_BOARD_DRIVERS_UARTSUPPORT_H

#include <stm32l0xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef UART_HandleTypeDef osg_board_UartHandler;
typedef USART_TypeDef osg_board_UartInstance;

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_Uart_getUarts(void);
uint8_t _osg_board_Uart_getDeviceIndex(const osg_UartId id);
osg_UartImpl * _osg_board_Uart_getUartByHandler(void * handler);
osg_UartImpl * _osg_board_Uart_getUartById(const osg_UartId id);
osg_board_UartInstance * _osg_board_Uart_getUartInstance(const osg_UartId id);
osg_board_UartHandler * _osg_board_Uart_getUartHandler(const osg_UartId id);

// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////
bool _osg_board_Uart_checkUart(const osg_UartId id);
bool _osg_board_Uart_validateTxPin(const osg_UartId id, const osg_GpioId tx);
bool _osg_board_Uart_validateRxPin(const osg_UartId id, const osg_GpioId rx);
bool _osg_board_Uart_validateCtsPin(const osg_UartId id, const osg_GpioId cts);
bool _osg_board_Uart_validateRtsPin(const osg_UartId id, const osg_GpioId rts);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////
uint32_t _osg_board_Uart_decodeUartBaudRate(const osg_UartBaudRate br);
uint32_t _osg_board_Uart_decodeUartBits(osg_UartBits b, const osg_UartParity p);
uint32_t _osg_board_Uart_decodeUartStopBits(const osg_UartStopBits sb);
uint32_t _osg_board_Uart_decodeUartParity(const osg_UartParity p);
uint32_t _osg_board_Uart_decodeUartFlowControl(const osg_UartFlowControl fc);

// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////
//osg_GpioAlternateFunction _osg_board_Uart_encodeAlternateFunction(const osg_UartId id);
osg_UartType _osg_board_Uart_encodeUartType(const osg_UartId id);

// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////
void _osg_board_Uart_enable(osg_board_UartHandler * handler, const osg_UartType type);
void _osg_board_Uart_enableClock(const osg_UartId id);
void _osg_board_Uart_enableInterrupt(const osg_UartId id);
void _osg_board_Uart_resetUart(const osg_UartId id);
void _osg_board_Uart_disableClock(const osg_UartId id);
void _osg_board_Uart_disableInterrupt(const osg_UartId id);

// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_Uart_doBufferedSend(osg_UartImpl * uart);
void _osg_board_Uart_doBufferedReceive(osg_UartImpl * uart);


#ifdef __cplusplus
}
#endif

#endif
