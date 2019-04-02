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
/// @param config The config.
void osg_board_I2c_ctor(
    osg_I2c * self,
    const osg_I2cConfig * const config
    );

/// @brief Destructor.
/// @ingroup Board-I2C
/// @param self The I2C object.
void osg_board_I2c_dtor(osg_I2c * self);

/// @brief Register the buffer for buffered .
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param circularTxBuffer Optional buffer to be used for implementing the transmission queue
/// @param txBufferSize Transmission buffer size
/// @param circularRxBuffer Optional buffer to be used for implementing the receive queue
/// @param rxBufferSize Receive buffer size
void osg_board_I2c_setBuffers(osg_I2c * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize);

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

/// @brief Performs a master send in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_I2c_masterSendBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size, const uint32_t timeout);

/// @brief Performs a master send in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns true if the request id successful handled.
bool osg_board_I2c_masterSendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size);


/// @brief Performs a master send in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns true if the request id successful handled.
bool osg_board_I2c_masterSendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize);

/// @brief Performs a  master receive in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_I2c_masterReceiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout);

/// @brief Performs a master receive in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns true if the request id successful handled.
bool osg_board_I2c_masterReceiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size);

/// @brief Performs a master receive inbuffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns true if the request id successful handled.
Size osg_board_I2c_masterReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize);

/// @brief Performs a slave send in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_I2c_slaveSendBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout);

/// @brief Performs a slave send in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns true if the request id successful handled.
bool osg_board_I2c_slaveSendNonBlocking(osg_I2c * self, void * buffer, const size_t size);

/// @brief Performs a slave send in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns true if the request id successful handled.
bool osg_board_I2c_slaveSendBuffered(osg_I2c * self, const void * buffer, const Size bufferSize);

/// @brief Performs a slave receive in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_I2c_slaveReceiveBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout);

/// @brief Performs a slave revceive in non-blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns true if the request id successful handled.
bool osg_board_I2c_slaveReceiveNonBlocking(osg_I2c * self, void * buffer, const size_t size);

/// @brief Performs a slave revceive in buffered mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param buffer A pointer to the message.
/// @param bufferSize The size of the buffer.
/// @return Returns true if the request id successful handled.
Size osg_board_I2c_slaveReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize);

/// @brief Use this method to start storing data in buffer.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
void osg_board_I2c_startReceiveBuffered(osg_I2c * self, const uint16_t slaveAddress);

/// @brief Use this method to stop storing data in buffer.
/// @ingroup Board-I2C
/// @param self The I2C object.
void osg_board_I2c_stopReceiveBuffered(osg_I2c * self);

/// @brief Use this method to know whether the buffer is enabled.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @return true if the buffer is enabled.
bool osg_board_I2c_isReceiveBufferedEnabled(osg_I2c * self);

/// @brief Use this method to write an I2C register in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param memAddress The I2C register memory address.
/// @param memAddrSize The I2C register memory address size.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_I2c_writeMemBlocking(osg_I2c * self, const uint16_t slaveAddress, const uint16_t memAddress, const uint16_t memAddrSize, void * buffer, const Size size, const uint32_t timeout);

/// @brief Use this method to read an I2C register in blocking mode.
/// @ingroup Board-I2C
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*. If STM-family board then the slave address is internally left shifted  as requested by STM driver.
/// @param memAddress The I2C register memory address.
/// @param memAddrSize The I2C register memory address size.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns true if the buffer is successful transmitted.
bool osg_board_I2c_readMemBlocking(osg_I2c * self, const uint16_t slaveAddress, const uint16_t memAddress, const uint16_t memAddrSize, void * buffer, const Size size, const uint32_t timeout);

/// @brief Use this method to read an I2C register in blocking mode.
/// @ingroup Board-I2C
/// @return Number of I2Cs peripheral on this board.
uint8_t osg_board_I2c_countI2cs(void);

#ifdef __cplusplus
}
#endif

#endif
