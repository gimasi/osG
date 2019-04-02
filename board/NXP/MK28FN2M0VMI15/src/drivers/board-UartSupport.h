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

#include <fsl_lpuart.h>
#include <fsl_port.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef lpuart_handle_t osg_board_UartHandler;
typedef LPUART_Type osg_board_UartInstance;

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_Uart_getUarts(void);
uint8_t _osg_board_Uart_getDeviceIndex(const osg_UartId id);
osg_UartImpl * _osg_board_Uart_getUartByInstance(void * handler);
osg_UartImpl * _osg_board_Uart_getUartById(const osg_UartId id);
osg_board_UartInstance * _osg_board_Uart_getUartInstance(const osg_UartId id);
osg_board_UartHandler * _osg_board_Uart_getUartHandler(const osg_UartId id);
osg_board_UartHandler * _osg_board_Uart_getUartHandlerFromInstance(const osg_board_UartInstance * instance);

// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////
bool _osg_board_Uart_checkUart(const osg_UartId id);
bool _osg_board_Uart_validateTxPin(const osg_UartId id, const osg_GpioId tx);
bool _osg_board_Uart_validateRxPin(const osg_UartId id, const osg_GpioId rx);
bool _osg_board_Uart_validateCtsPin(const osg_UartId id, const osg_GpioId cts);
bool _osg_board_Uart_validateRtsPin(const osg_UartId id, const osg_GpioId rts);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////
uint32_t _osg_board_Uart_decodeUartBaudRate(const osg_UartBaudRate br);
lpuart_data_bits_t _osg_board_Uart_decodeUartBits(osg_UartBits b, const osg_UartParity p);
lpuart_stop_bit_count_t _osg_board_Uart_decodeUartStopBits(const osg_UartStopBits sb);
lpuart_parity_mode_t _osg_board_Uart_decodeUartParity(const osg_UartParity p);
bool _osg_board_Uart_decodeUartCtsEnabled(const osg_UartFlowControl fc);
bool _osg_board_Uart_decodeUartRtsEnabled(const osg_UartFlowControl fc);
//port_mux_t _osg_board_Uart_decodePinMuxAlt(const osg_GpioId pin);

// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////
//osg_GpioAlternateFunction _osg_board_Uart_encodeAlternateFunction(const osg_UartId id);
osg_UartType _osg_board_Uart_encodeUartType(const osg_UartId id);

// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////
void _osg_board_Uart_enableClock(const osg_UartId id);

// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_Uart_doBufferedSend(osg_UartImpl * uart);
void _osg_board_Uart_doBufferedReceive(osg_UartImpl * uart);
void _osg_board_Uart_sendReceiveCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
void _osg_board_Uart_timeoutCallback(void * argument);


#ifdef __cplusplus
}
#endif

#endif
