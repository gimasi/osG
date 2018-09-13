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
#ifndef OSG_BOARD_DRIVERS_I2C_H
#define OSG_BOARD_DRIVERS_I2C_H

#include "../board-forwards.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Board-I2C Board-I2C
/// @ingroup Board-Drivers
/// Board methods for I2C

/// @brief Constructor.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param clockSpeed The *master* bus speed.
/// @param addressingMode The I2C *slave* addressing mode.
/// @param enableGeneralCall Indicates if the *slave* accepts a General Call.
/// @param ownAddress I2C *slave* address.
/// @param circularTxBuffer Optional transmission buffer
/// @param txBufferSize Transmit buffer size
/// @param circularRxBuffer Optional receive buffer
/// @param rxBufferSize Receive buffer size
void osg_board_I2c_ctor(osg_I2c * self, const uint32_t clockSpeed, const osg_I2cAddressingMode addressingMode, const Bool enableGeneralCall, const uint16_t ownAddress, void * circularTxBuffer, Size txBufferSize, void * circularRxBuffer, Size rxBufferSize);
/// @brief Destructor.
/// @ingroup Board-I2C
/// @param self The I2C object.
void osg_board_I2c_dtor(osg_I2c * self);
/// @brief Performs a master send in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns TRUE if the buffer is successful transmitted.
Bool osg_board_I2c_masterSendBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size, const uint32_t timeout);
/// @brief Performs a master send in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Bool osg_board_I2c_masterSendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size);
/// @brief Performs a master send in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Bool osg_board_I2c_masterSendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize);
/// @brief Performs a  master receive in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns TRUE if the buffer is successful transmitted.
Bool osg_board_I2c_masterReceiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout);
/// @brief Performs a master receive in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Bool osg_board_I2c_masterReceiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size);
/// @brief Performs a master receive inbuffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Size osg_board_I2c_masterReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize);
/// @brief Performs a slave send in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns TRUE if the buffer is successful transmitted.
Bool osg_board_I2c_slaveSendBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout);
/// @brief Performs a slave send in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Bool osg_board_I2c_slaveSendNonBlocking(osg_I2c * self, void * buffer, const size_t size);
/// @brief Performs a slave send in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Bool osg_board_I2c_slaveSendBuffered(osg_I2c * self, const void * buffer, const Size bufferSize);
/// @brief Performs a slave receive in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns TRUE if the buffer is successful transmitted.
Bool osg_board_I2c_slaveReceiveBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout);
/// @brief Performs a slave revceive in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Bool osg_board_I2c_slaveReceiveNonBlocking(osg_I2c * self, void * buffer, const size_t size);
/// @brief Performs a slave revceive in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns TRUE if the request id successful handled.
Size osg_board_I2c_slaveReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize);
/// @brief Use this method to start storing data in buffer.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
void osg_board_I2c_startReceiveBuffered(osg_I2c * self, const uint16_t slaveAddress);
/// @brief Use this method to stop storing data in buffer.
/// @ingroup Board-I2C
/// @param self The I2C object.
void osg_board_I2c_stopReceiveBuffered(osg_I2c * self);
/// @brief Use this method to know whether the buffer is enabled.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @return TRUE if the buffer is enabled.
Bool osg_board_I2c_isReceiveBufferedEnabled(osg_I2c * self);
/// @brief Performs a slave revceive in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_board_I2c_setBufferedCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx);
/// @brief Performs a slave revceive in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_board_I2c_setNbCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx);

#ifdef __cplusplus
}
#endif

#endif
