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

#include <stm32l0xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef SPI_HandleTypeDef osg_board_SpiHandler;
typedef SPI_TypeDef osg_board_SpiInstance;

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_Spi_getSpis(void);
uint8_t _osg_board_Spi_getDeviceIndex(const osg_SpiId spi);
osg_SpiImpl * _osg_board_Spi_getSpiByHandler(void * handler);
osg_SpiImpl * _osg_board_Spi_getSpiById(const osg_SpiId id);
osg_board_SpiInstance * _osg_board_Spi_getSpiInstance(const osg_SpiId spi);
osg_board_SpiHandler * _osg_board_Spi_getSpiHandler(const osg_SpiId id);
#if SPI_MUTEX_ENABLED
void _osg_board_Spi_configureMutex(const osg_SpiId id);
osg_Mutex * _osg_board_Spi_getMutex(const osg_SpiId id);
#endif

// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////
bool _osg_board_Spi_checkSpi(const osg_SpiId spi);
bool _osg_board_Spi_validatePins(const osg_SpiId spiId, const osg_GpioId sclk, const osg_GpioId mosi, const osg_GpioId miso);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////
uint32_t _osg_board_Spi_decodeBaudRate(const uint32_t speed);
uint32_t _osg_board_Spi_decodeMode(const osg_SpiMode mode);
uint32_t _osg_board_Spi_decodePhase(const osg_SpiPhase phase);
uint32_t _osg_board_Spi_decodePolarity(const osg_SpiPolarity polarity);
uint32_t _osg_board_Spi_decodeEndianess(const osg_SpiEndianess endianess);
uint32_t _osg_board_Spi_decodeDataSize(const osg_SpiDataSize size);
uint32_t _osg_board_Spi_decodeSSMode(const osg_SpiSlaveSelectMode mode);
//uint32_t _osg_board_Spi_decodeAlternateFunction(const osg_SpiId spiId);

// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////
//osg_GpioAlternateFunction _osg_board_Spi_encodeAlternateFunction(const osg_SpiId spiId, const bool isMosiPin, const bool isMisoPin);

// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////
void _osg_board_Spi_enableSpi(osg_board_SpiHandler * handler);
void _osg_board_Spi_enableSpiClk(const osg_SpiId spi);
void _osg_board_Spi_enableSpiInterrupt(const osg_SpiId id);
void _osg_board_Spi_initSSPins(const osg_SpiId spiId, const osg_GpioId * ssPins, const Size ssPinsSize, const osg_SpiSlaveSelectMode ssMode, const uint16_t alternate);
void _osg_board_Spi_deinitSSPins(const osg_SpiId id, osg_GpioId * ssList, const Size ssListSize);
void _osg_board_Spi_resetSpi(const osg_SpiId id);
void _osg_board_Spi_disableSpiClk(const osg_SpiId spi);
void _osg_board_Spi_disableSpiInterrupt(const osg_SpiId id);

// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_Spi_activateSlave(const osg_GpioId slaveSelectPin);
void _osg_board_Spi_deactivateSlave(const osg_GpioId slaveSelectPin);
void _osg_board_Spi_doBufferedSend(osg_SpiImpl * spi);
void _osg_board_Spi_doBufferedReceive(osg_SpiImpl * spi);


#ifdef __cplusplus
}
#endif

#endif
