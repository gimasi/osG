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
#ifndef OSG_DRIVERS_SPI_H
#define OSG_DRIVERS_SPI_H

#include "SpiDataTypes.h"
#include "GpioDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup SPI SPI
/// @ingroup Drivers
/// Methods for SPI

/// @brief Constructor.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param config The SPI configuration.
void osg_Spi_ctor(
    osg_Spi * self,
    const osg_SpiConfig * const config
    );

/// @brief Destructor.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
void osg_Spi_dtor(osg_Spi * self);

/// @brief Registers the buffers used in buffered functions.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param circularTxBuffer Pointer to a buffer used as Circular Buffer for continuos transmission.
/// @param txBufferSize The size of the buffer used for transmissions.
/// @param circularRxBuffer Pointer to a buffer used as Circular Buffer for continuos reception.
/// @param rxBufferSize The size of the buffer used for reception.
void osg_Spi_setBuffers(osg_Spi * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize);

/// @brief Registers the callbacks to call in buffered mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_Spi_setBufferedCallbacks(osg_Spi * self, const osg_SpiCallback tx, const osg_SpiCallback rx);

/// @brief Registers the callbacks to call after a non-blocking communication.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param tx The callback for successful transmission communication.
/// @param rx The callback for successful reception communication.
void osg_Spi_setNbCallbacks(osg_Spi * self, const osg_SpiCallback tx, const osg_SpiCallback rx);

/// @brief Use this method to send an amount of data through SPI in blocking mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @param timeout After this timeout (in millisec) the transmission is aborted.
/// @return true if the transmission complete successfully.
bool osg_Spi_sendBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize, const uint32_t timeout);

/// @brief Use this method to receive an amount of data through SPI in blocking mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data received.
/// @param bufferSize The size of the buffer.
/// @param timeout After this timeout (in millisec) the transmission is aborted.
/// @return true if the transmission complete successfully.
bool osg_Spi_receiveBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize, const uint32_t timeout);

/// @brief Use this method to send an amount of data through SPI in NON-blocking mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @return true if the transmission is successfully handled.
bool osg_Spi_sendNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize);

/// @brief Use this method to receive an amount of data through SPI in NON-blocking mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data received.
/// @param bufferSize The size of the buffer.
/// @return true if the transmission is successfully handled.
bool osg_Spi_receiveNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize);

/// @brief Use this method to send an amount of data through SPI in buffered mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @return The number of inserted bytes.
bool osg_Spi_sendBuffered(osg_Spi * self, const uint16_t ssPinIndex, const void * buffer, const Size bufferSize);

/// @brief Use this method to receive an amount of data through SPI in buffered mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data received.
/// @param bufferSize The size of the buffer.
/// @return The amount of data received and wrote in the buffer; it can be less than the bufferSize.
Size osg_Spi_receiveBuffered(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize);

/// @brief Use this method to start storing data in buffer.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
void osg_Spi_startReceiveBuffered(osg_Spi * self, const uint16_t ssPinIndex);

/// @brief Use this method to stop storing data in buffer.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
void osg_Spi_stopReceiveBuffered(osg_Spi * self);

/// @brief Use this method to know whether the buffer is enabled.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @return true if the buffer is enabled.
bool osg_Spi_isReceiveBufferedEnabled(osg_Spi * self);

/// @brief Gets the number of SPI peripherals.
/// @ingroup SPI
/// @memberof osg_Spi
/// @return Number of SPIs peripheral on this board.
uint8_t osg_Spi_countSpis(void);

/// @brief Use this method to send and receive an amount of data through SPI in blocking mode.
/// @ingroup SPI
/// @memberof osg_Spi
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param bufferTx The pointer to a valid buffer contains the data to send.
/// @param bufferRx The pointer to a valid buffer contains the data received.
/// @param transferSize The size of the buffers.
/// @param timeout After this timeout (in millisec) the transmission is aborted.
/// @return true if the transmission complete successfully.
bool osg_Spi_sendReceiveBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * bufferTx, void * bufferRx, const Size transferSize, const uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
