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
#ifndef OSG_DRIVERS_UART_H
#define OSG_DRIVERS_UART_H

#include "UartDataTypes.h"
#include "../osgDataTypes.h"
#include "GpioDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup UART UART
/// @ingroup Drivers
/// Methods for UART

/// @brief Constructor for UART.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param id The UART id of the board.
/// @param baud The baud rate
/// @param parity The parity
/// @param bits Data bits
/// @param stopBits Stop bits
/// @param flow THe flow control mode
/// @param circularTxBuffer Optional buffer for transmission queue
/// @param txBufferSize Transmission bufffer size
/// @param circularRxBuffer Optional buffer for receive queue
/// @param rxBufferSize Receive bufffer size
/// @param txPin Transmission pin
/// @param rxPin Receive pin
/// @param ctsPin CTS pin
/// @param rtsPin RTS pin
void osg_Uart_ctor(osg_Uart * self,
    const osg_UartId id,
    const osg_UartBaudRate baud,
    const osg_UartParity parity,
    const osg_UartBits bits,
    const osg_UartStopBits stopBits,
    const osg_UartFlowControl flow,
    void * circularTxBuffer,
    Size txBufferSize,
    void * circularRxBuffer,
    Size rxBufferSize,
    const osg_GpioId txPin,
    const osg_GpioId rxPin,
    const osg_GpioId ctsPin,
    const osg_GpioId rtsPin
    );
/// @brief Destructor for UART.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
void osg_Uart_dtor(osg_Uart * self);
/// @brief Performs a send in blocking mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns TRUE if the buffer is successful transmitted.
Bool osg_Uart_sendBlocking(osg_Uart * self, const void * buffer, const Size size, const uint32_t timeout);
/// @brief Performs a send in non-blocking mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns TRUE if the request is successful handled.
Bool osg_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const Size size);
/// @brief Use this method to send an amount of data through UART in buffered mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @return Returns TRUE if the request is successful handled.
Bool osg_Uart_sendBuffered(osg_Uart * self, const void * buffer, const Size bufferSize);
/// @brief Registers the callbacks to call in buffered mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_Uart_setBufferedCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx);
/// @brief Registers the callbacks to call after a non-blocking communication.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_Uart_setNbCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx);
/// @brief Receive an amount of bytes in blocking mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param buffer The buffer where the data received are stored.
/// @param size The amount of data to be received.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return TRUE if no errors occur.
Bool osg_Uart_receiveBlocking(osg_Uart * self, void * buffer, const Size size, const uint32_t timeout);
/// @brief Receive an amount of data in non-blocking mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param buffer The buffer where the data received are stored.
/// @param size The amount of data to be received.
/// @return TRUE if the request is successful handled.
Bool osg_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size);
/// @brief Use this method to get the data received through UART in circular-buffered mode.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @param buffer The buffer where the data are stored.
/// @param bufferSize The size of the buffer
/// @return The amount of bytes wrote into the buffer.
Size osg_Uart_receiveBuffered(osg_Uart * self, void * buffer, const Size bufferSize);
/// @brief Use this method to start storing data in buffer.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
void osg_Uart_startReceiveBuffered(osg_Uart * self);
/// @brief Use this method to stop storing data in buffer.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
void osg_Uart_stopReceiveBuffered(osg_Uart * self);
/// @brief Use this method to know whether the buffer is enabled.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @return TRUE if the buffer is enabled.
Bool osg_Uart_isReceiveBufferedEnabled(osg_Uart * self);
/// @brief Use this method to know how many chars are received.
/// @ingroup UART
/// @memberof osg_Uart
/// @param self The UART object.
/// @return The amount of characters received from last read.
Size osg_Uart_getReceivedCharsRxBuffer(osg_Uart * self);

#ifdef __cplusplus
}
#endif

#endif
