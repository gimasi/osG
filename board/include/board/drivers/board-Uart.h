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
#ifndef OSG_BOARD_DRIVERS_UART_H
#define OSG_BOARD_DRIVERS_UART_H

#include "../board-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Constructor.
/// @ingroup Board-Uart
void osg_board_Uart_ctor(osg_Uart * self,
    const osg_UartBaudRate baud,
    const osg_UartParity parity,
    const osg_UartBits bits,
    const osg_UartStopBits stopBits,
    const osg_UartFlowControl flow,
    void * circularTxBuffer,
    Size txBufferSize,
    void * circularRxBuffer,
    Size rxBufferSize
    );
/// @brief The pin destructor
/// @ingroup Board-Uart
/// @param self The pin object
void osg_board_Uart_dtor(osg_Uart * self);

void osg_board_Uart_setBufferedCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx);
void osg_board_Uart_setNbCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx);

Bool osg_board_Uart_sendBlocking(osg_Uart * self, const void * buffer, const Size size, const uint32_t timeout);
Bool osg_board_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size);
Bool osg_board_Uart_sendBuffered(osg_Uart * self, const void * buffer, const Size bufferSize);

Bool osg_board_Uart_receiveBlocking(osg_Uart * self, void * buffer, const Size size, const uint32_t timeout);
Bool osg_board_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size);
Size osg_board_Uart_receiveBuffered(osg_Uart * self, void * buffer, const Size bufferSize);
    
void osg_board_Uart_startReceiveBuffered(osg_Uart * self);
void osg_board_Uart_stopReceiveBuffered(osg_Uart * self);
Bool osg_board_Uart_isReceiveBufferedEnabled(osg_Uart * self);
Size osg_board_Uart_getReceivedCharsRxBuffer(osg_Uart * self);

#ifdef __cplusplus
}
#endif

#endif
