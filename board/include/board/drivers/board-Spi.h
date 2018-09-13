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
#ifndef OSG_BOARD_DRIVERS_SPI_H
#define OSG_BOARD_DRIVERS_SPI_H

#include "../board-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Board-SPI Board-SPI
/// @ingroup Board-Drivers
/// Board methods for SPI

/// @brief Constructor.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param clockSpeed The SPI *master* bus speed.
/// @param polarity The SPI polarity configuration.
/// @param phase The SPI phase configuration.
/// @param endianess The SPI data transfer order.
/// @param dataSize The SPI data size.
/// @param ssMode Specify the slave select PIN functionality mode.
void osg_board_spi_ctor(osg_Spi * self, uint32_t clockSpeed, osg_SpiPolarity polarity, osg_SpiPhase phase, osg_SpiEndianess endianess, osg_SpiDataSize dataSize, osg_SpiSlaveSelectMode ssMode);
/// @brief Destructor.
/// @ingroup Board-SPI
/// @param self The SPI object.
void osg_board_spi_dtor(osg_Spi * self);
/// @brief Use this method to send an amount of data through SPI in blocking mode.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @param timeout After this timeout (in millisec) the transmission is aborted.
/// @return TRUE if the transmission complete successfully.
Bool osg_board_spi_sendBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout);
/// @brief Use this method to receive an amount of data through SPI in blocking mode.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data received.
/// @param bufferSize The size of the buffer.
/// @param timeout After this timeout (in millisec) the transmission is aborted.
/// @return TRUE if the transmission complete successfully.
Bool osg_board_spi_receiveBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout);
/// @brief Use this method to send an amount of data through SPI in NON-blocking mode.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @param callback The function called after the trasmission is completed.
/// @return TRUE if the transmission is successfully handled.
Bool osg_board_spi_sendNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback);
/// @brief Use this method to receive an amount of data through SPI in NON-blocking mode.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data received.
/// @param bufferSize The size of the buffer.
/// @param callback The function called after the trasmission is completed.
/// @return TRUE if the transmission is successfully handled.
Bool osg_board_spi_receiveNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback);
/// @brief Use this method to send an amount of data through SPI in buffered mode.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data to send.
/// @param bufferSize The size of the buffer.
/// @param callback The function called after the trasmission is completed.
/// @return TRUE if the transmission is successfully handled.
Size osg_board_spi_sendBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback);
/// @brief Use this method to receive an amount of data through SPI in buffered mode.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @param ssPinIndex The index of the ssPinList used in *master* mode for activates the correct slave.
/// @param buffer The pointer to a valid buffer contains the data received.
/// @param bufferSize The size of the buffer.
/// @return The amount of data received and wrote in the buffer; it can be less than the bufferSize.
Size osg_board_spi_receivedBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize);
/// @brief Use this method to check if there are some unread data in the circular Rx buffer.
/// @ingroup Board-SPI
/// @param self The SPI object.
/// @return The amount of data received and not already read.
Size osg_board_spi_getSizeOfReceivedData(osg_Spi * self);

#ifdef __cplusplus
}
#endif

#endif
