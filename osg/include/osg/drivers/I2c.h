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
#ifndef OSG_DRIVERS_I2C_H
#define OSG_DRIVERS_I2C_H

#include "I2cDataTypes.h"
#include "GpioDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup I2C I2C
/// @ingroup Drivers
/// Methods for I2C

/// @brief Constructor.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param id The I2C id.
/// @param mode The device mode (master/slave).
/// @param clockSpeed The *master* bus speed.
/// @param addressingMode The I2C *slave* addressing mode.
/// @param enableGeneralCall Indicates if the *slave* accepts a General Call.
/// @param ownAddress I2C *slave* address.
/// @param circularTxBuffer Optional buffer to be used for implementing the transmission queue
/// @param txBufferSize Transmission buffer size
/// @param circularRxBuffer Optional buffer to be used for implementing the receive queue
/// @param rxBufferSize Receive buffer size
/// @param sdaPin Board PIN used for I2C SDA port.
/// @param sclPin Board PIN used for I2C SCL port.
void osg_I2c_ctor(osg_I2c * self,
    const osg_I2cId id,
    const osg_I2cMode mode,
    const uint32_t clockSpeed,
    const osg_I2cAddressingMode addressingMode,
    const Bool enableGeneralCall,
    const uint16_t ownAddress,
    void * circularTxBuffer,
    Size txBufferSize,
    void * circularRxBuffer,
    Size rxBufferSize,
    const osg_GpioId sdaPin,
    const osg_GpioId sclPin);
/// @brief Destructor.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
void osg_I2c_dtor(osg_I2c * self);
/// @brief Performs a send in blocking mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return Returns TRUE if the buffer is successful transmitted.
Bool osg_I2c_sendBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const Size size, const uint32_t timeout);
/// @brief Performs a send in non-blocking mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer A pointer to the message.
/// @param size The size of the buffer.
/// @return Returns TRUE if the request is successful handled.
Bool osg_I2c_sendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const Size size);
/// @brief Use this method to send an amount of data through UART in buffered mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @return Returns TRUE if the request is successful handled.
Bool osg_I2c_sendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize);
/// @brief Registers the callbacks to call in buffered mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_I2c_setBufferedCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx);
/// @brief Registers the callbacks to call after a non-blocking communication.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_I2c_setNbCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx);
/// @brief Receive an amount of bytes in blocking mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer The buffer where the data received are stored.
/// @param size The amount of data to be received.
/// @param timeout The amount of time in seconds to wait for transmission.
/// @return TRUE if no errors occur.
Bool osg_I2c_receiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const Size size, const uint32_t timeout);
/// @brief Receive an amount of data in non-blocking mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
/// @param buffer The buffer where the data received are stored.
/// @param size The amount of data to be received.
/// @return TRUE if the request is successful handled.
Bool osg_I2c_receiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const Size size);
/// @brief Use this method to get the data received through UART in circular-buffered mode.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param buffer The buffer where the data are stored.
/// @param bufferSize The size of the buffer
/// @return The amount of bytes wrote into the buffer.
Size osg_I2c_receiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize);
/// @brief Use this method to start storing data in buffer.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @param slaveAddress The I2C slave address of the receiver. Used only if the device is the *master*.
void osg_I2c_startReceiveBuffered(osg_I2c * self, const uint16_t slaveAddress);
/// @brief Use this method to stop storing data in buffer.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
void osg_I2c_stopReceiveBuffered(osg_I2c * self);
/// @brief Use this method to know whether the buffer is enabled.
/// @ingroup I2C
/// @memberof osg_I2c
/// @param self The I2C object.
/// @return TRUE if the buffer is enabled.
Bool osg_I2c_isReceiveBufferedEnabled(osg_I2c * self);

#ifdef __cplusplus
}
#endif

#endif
