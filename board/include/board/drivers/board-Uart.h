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
/// @param self The UART object.
/// @param config The UART config
void osg_board_Uart_ctor(osg_Uart * self,
    const osg_UartConfig * const config
    );

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
void osg_board_Uart_dtor(osg_Uart * self);

/// @brief Registers buffers used in buffered functions.
/// @ingroup Board-Uart
/// @param self The UART object.
/// @param circularTxBuffer Optional buffer for transmission queue
/// @param txBufferSize Transmission bufffer size
/// @param circularRxBuffer Optional buffer for receive queue
/// @param rxBufferSize Receive bufffer size
void osg_board_Uart_setBuffers(osg_Uart * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_board_Uart_setBufferedCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_board_Uart_setNbCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_Uart_sendBlocking(osg_Uart * self, const void * buffer, const Size size, const uint32_t timeout);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns true if the request is successful handled.
bool osg_board_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @return Returns true if the request is successful handled.
bool osg_board_Uart_sendBuffered(osg_Uart * self, const void * buffer, const Size bufferSize);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param buffer The buffer where the data received are stored.
/// @param size The amount of data to be received.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return true if no errors occur.
bool osg_board_Uart_receiveBlocking(osg_Uart * self, void * buffer, const Size size, const uint32_t timeout);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param buffer The buffer where the data received are stored.
/// @param size The amount of data to be received.
/// @return true if the request is successful handled.
bool osg_board_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @param buffer The buffer where the data are stored.
/// @param bufferSize The size of the buffer
/// @return The amount of bytes wrote into the buffer.
Size osg_board_Uart_receiveBuffered(osg_Uart * self, void * buffer, const Size bufferSize);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
void osg_board_Uart_startReceiveBuffered(osg_Uart * self);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
void osg_board_Uart_stopReceiveBuffered(osg_Uart * self);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @return true if the buffer is enabled.
bool osg_board_Uart_isReceiveBufferedEnabled(osg_Uart * self);

/// @brief The UART destructor
/// @ingroup Board-Uart
/// @param self The UART object
/// @return The amount of characters received from last read.
Size osg_board_Uart_getReceivedCharsRxBuffer(osg_Uart * self);

/// @brief Gets the number of UART peripherals.
/// @ingroup Board-Uart
/// @return Number of UARTs peripheral on this board.
uint8_t osg_board_Uart_countUarts(void);

#ifdef __cplusplus
}
#endif

#endif
