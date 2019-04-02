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
#ifndef OSG_BOARD_DRIVERS_SPISUPPORT_H
#define OSG_BOARD_DRIVERS_SPISUPPORT_H

#define SPI_MUTEX_ENABLED   defined(OSG_OS_NAME) && (OSG_OS_NAME != OSG_OS_NONE) && defined(OSG_OS_USE_MUTEX) && OSG_OS_USE_MUTEX == 1

#include <fsl_dspi.h>
#include <fsl_port.h>

#ifdef __cplusplus
extern "C" {
#endif

// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef SPI_Type osg_board_SpiInstance;
union osg_board_SpiHandler;
typedef union osg_board_SpiHandler osg_board_SpiHandler;
union osg_board_SpiHandler
{
    dspi_master_handle_t masterHandle;
    dspi_slave_handle_t slaveHandle;
};

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_Spi_getSpis(void);
uint8_t _osg_board_Spi_getDeviceIndex(const osg_SpiId id);
osg_SpiImpl * _osg_board_Spi_getSpiByInstance(void * handler);
osg_SpiImpl * _osg_board_Spi_getSpiById(const osg_SpiId id);
osg_board_SpiInstance * _osg_board_Spi_getSpiInstance(const osg_SpiId id);
osg_board_SpiHandler * _osg_board_Spi_getSpiHandler(const osg_SpiId id);
osg_board_SpiHandler * _osg_board_Spi_getSpiHandlerByInstance(void * handler);
#if SPI_MUTEX_ENABLED
void _osg_board_Spi_configureMutex(const osg_SpiId id);
osg_Mutex * _osg_board_Spi_getMutex(const osg_SpiId id);
#endif

// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////
bool _osg_board_Spi_checkSpi(const osg_SpiId spi);
bool _osg_board_Spi_validateSpiPins(const osg_SpiId spiId, const osg_SpiMode mode, const osg_GpioId sclk, const osg_GpioId mosi, const osg_GpioId miso);
bool _osg_board_Spi_validateSsPins(const osg_SpiId spiId, const osg_SpiMode mode, osg_GpioId * ssPinList, const Size ssPinListSize);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////
dspi_clock_phase_t _osg_board_Spi_decodePhase(const osg_SpiPhase phase);
dspi_clock_polarity_t _osg_board_Spi_decodePolarity(const osg_SpiPolarity polarity);
dspi_shift_direction_t _osg_board_Spi_decodeEndianess(const osg_SpiEndianess endianess);
uint32_t _osg_board_Spi_decodeDataSize(const osg_SpiDataSize size);
//port_mux_t _osg_board_Spi_decodePinMuxAlt(const osg_GpioId pin, const osg_GpioAlternateFunction alt);
uint32_t _osg_board_Spi_getSpiClk(const osg_SpiId id);

// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////
//osg_GpioAlternateFunction _osg_board_Spi_encodeAlternateFunction(const osg_SpiId spiId, const bool isMosiPin, const bool isMisoPin);

// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////
void _osg_board_Spi_initSsPins(const osg_SpiId spiId, const osg_SpiSlaveSelectMode ssMode, const osg_GpioId * ssPins, const Size ssPinsSize, const uint16_t alternate);
void _osg_board_Spi_deinitSSPins(const osg_SpiId id, osg_GpioId * ssList, const Size ssListSize);

// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_Spi_doBufferedSend(osg_SpiImpl * spi);
void _osg_board_Spi_doBufferedReceive(osg_SpiImpl * spi);
void _osg_board_Spi_masterBlockingCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_masterSendNBCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_masterRecvNBCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_masterSendBuffCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_masterRecvBuffCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_slaveBlockingCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_slaveSendNBCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_slaveRecvNBCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_slaveSendBuffCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_slaveRecvBuffCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData);
void _osg_board_Spi_timeoutCallback(void * argument);


#ifdef __cplusplus
}
#endif

#endif
