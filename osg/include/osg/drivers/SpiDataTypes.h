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
#ifndef OSG_DRIVERS_SPIDATATYPES_H
#define OSG_DRIVERS_SPIDATATYPES_H

#include "../osgDataTypes.h"
#include "GpioDataTypes.h"
#include "../utils/CircularFifo.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Convenience type for SPI callback
/// @ingroup SPI
/// @memberof osg_Spi
typedef void (* osg_SpiCallback)(void);

enum osg_SpiMode;
typedef enum osg_SpiMode osg_SpiMode;
/// @brief The possible SPI operation mode.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiMode
{
    OSG_SPI_MASTER_MODE,
    OSG_SPI_SLAVE_MODE
};

enum osg_SpiId;
typedef enum osg_SpiId osg_SpiId;
/// @brief The possible SPI IDs.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiId
{
    OSG_SPI0,
    OSG_SPI1,
    OSG_SPI2,
    OSG_SPI3,
    OSG_SPI4,
    OSG_SPI5,
    OSG_SPI6,
    OSG_SPI7
};

enum osg_SpiPolarity;
typedef enum osg_SpiPolarity osg_SpiPolarity;
/// @brief The possible SPI polarity configurations.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiPolarity
{
    OSG_SPI_POLARITY_LOW,
    OSG_SPI_POLARITY_HIGH
};

enum osg_SpiPhase;
typedef enum osg_SpiPhase osg_SpiPhase;
/// @brief The possible SPI clock phases.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiPhase
{
    OSG_SPI_PHASE_1ST,
    OSG_SPI_PHASE_2ND
};

enum osg_SpiEndianess;
typedef enum osg_SpiEndianess osg_SpiEndianess;
/// @brief Specifies whether data transfers start from MSB or LSB bit.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiEndianess
{
    OSG_SPI_MSB_FIRST,
    OSG_SPI_LSB_FIRST
};

enum osg_SpiDataSize;
typedef enum osg_SpiDataSize osg_SpiDataSize;
/// @brief Specifies the SPI data size.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiDataSize
{
    OSG_SPI_DATA_4_BITS,
    OSG_SPI_DATA_5_BITS,
    OSG_SPI_DATA_6_BITS,
    OSG_SPI_DATA_7_BITS,
    OSG_SPI_DATA_8_BITS,
    OSG_SPI_DATA_9_BITS,
    OSG_SPI_DATA_10_BITS,
    OSG_SPI_DATA_11_BITS,
    OSG_SPI_DATA_12_BITS,
    OSG_SPI_DATA_13_BITS,
    OSG_SPI_DATA_14_BITS,
    OSG_SPI_DATA_15_BITS,
    OSG_SPI_DATA_16_BITS
};

enum osg_SpiSlaveSelectMode;
typedef enum osg_SpiSlaveSelectMode osg_SpiSlaveSelectMode;
/// @brief Specifies the SPI slave select PIN mode.
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiSlaveSelectMode
{
    /// @brief Automatic or software selection.
    OSG_SPI_SLAVE_SELECT_AUTO,
    /// @brief Slave select PIN acts as input.
    OSG_SPI_SLAVE_SELECT_INPUT,
    /// @brief Slave select PIN acts as output.
    OSG_SPI_SLAVE_SELECT_OUTPUT,
    /// @brief Slave select PIN is unused.
    OSG_SPI_SLAVE_SELECT_UNUSED
};

struct osg_SpiConfig;
typedef struct osg_SpiConfig osg_SpiConfig;
/// @brief The SPI configuration.
/// @ingroup SPI
/// @memberof osg_Spi
struct osg_SpiConfig
{
    /// @brief The SPI id.
    osg_SpiId id;
    /// @brief The device mode (master/slave).
    osg_SpiMode mode;
    /// @brief The SPI *master* bus speed.
    uint32_t clockSpeed;
    /// @brief The SPI polarity configuration.
    osg_SpiPolarity polarity;
    /// @brief The SPI phase configuration.
    osg_SpiPhase phase;
    /// @brief The SPI data transfer order.
    osg_SpiEndianess endianess;
    /// @brief The SPI data size.
    osg_SpiDataSize dataSize;
    /// @brief Specify the slave select PIN functionality mode.
    osg_SpiSlaveSelectMode ssMode;
    /// @brief Board PIN used for SCLK (clock) port.
    osg_GpioId sclkPin;
    /// @brief Board PIN used for MOSI (master-output slave-input) port.
    osg_GpioId mosiPin;
    /// @brief Board PIN used for MISO (master-input slave-output) port.
    osg_GpioId misoPin;
    /// @brief Array of board PINs used as salve select (SS). In *slave* mode it can contain 1 PIN at most. In *master* mode it should contain 1 PIN for each slave.
    osg_GpioId * ssPinList;
    /// @brief The number of PINs used as slave select (SS).
    Size ssPinListSize;
    /// @brief The possible alternate function.
    uint16_t alternateFunction;
};

struct osg_Spi;
typedef struct osg_Spi osg_Spi;
/// @brief The SPI class.
/// @ingroup SPI
/// @memberof osg_Spi
struct osg_Spi
{
    /// @brief The ID.
    /// @private
    osg_SpiId id;
    /// @brief SCLK pin ID.
    /// @private
    osg_Gpio sclkPin;
    /// @brief MOSI pin ID.
    /// @private
    osg_Gpio mosiPin;
    /// @brief MISO pin ID.
    /// @private
    osg_Gpio misoPin;
    /// @brief SS pin ID list.
    /// @private
    osg_GpioId * ssPinList;
    /// @brief SS pin ID list size.
    /// @private
    Size ssPinListSize;
};

enum osg_SpiFlags;
typedef enum osg_SpiFlags osg_SpiFlags;
/// @brief Enum used to represent SPI internal bit flags
/// @ingroup SPI
/// @memberof osg_Spi
enum osg_SpiFlags
{
    /// @brief Initial value
    OSG_SPI_FLAG_DEFAULT         = 0x00,
    /// @brief I2C is performing a non-blocking communication
    OSG_SPI_FLAG_IS_NB_SEND      = 0x02,
    /// @brief I2C is performing a non-blocking communication
    OSG_SPI_FLAG_IS_NB_RECEIVE   = 0x04,
    /// @brief I2C is sending data
    OSG_SPI_FLAG_IS_SEND         = 0x08,
    /// @brief I2C is receiving data
    OSG_SPI_FLAG_IS_RECEIVE      = 0x10,
    /// @brief I2C buffered rx process is enabled
    OSG_SPI_FLAG_RX_BUFF_ENABLED = 0x20,
    /// @brief I2C wait for aborting rx process
    OSG_SPI_FLAG_RX_ABORTED      = 0x40,
    /// @brief I2C configuration done.
    OSG_SPI_FLAG_HANDLE_CREATED  = 0x80
};

struct osg_SpiImpl;
typedef struct osg_SpiImpl osg_SpiImpl;
/// @brief Spi internal struct.
/// @ingroup SPI
/// @memberof osg_Spi
struct osg_SpiImpl
{
    /// @brief Low-level handler
    /// @private
    void * handler;
    /// @brief Mode.
    /// @private
    osg_SpiMode mode;
    /// @brief Send NB callback
    /// @private
    osg_SpiCallback txCompleteNb;
    /// @brief receive NB callback
    /// @private
    osg_SpiCallback rxCompleteNb;
    /// @brief Send buffered callback
    /// @private
    osg_SpiCallback txCompleteBuffered;
    /// @brief receive buffered callback
    /// @private
    osg_SpiCallback rxCompleteBuffered;
    /// @brief Internal flags
    /// @private
    osg_SpiFlags flags;
    /// @brief Tx internal fifo
    /// @private
    osg_CircularFifo txFifo;
    /// @brief Rx internal fifo
    /// @private
    osg_CircularFifo rxFifo;
    /// @brief Rx receive byte
    /// @private
    Byte rxByte;
    /// @brief
    /// @private
    osg_GpioId lastSsPin;
};

#ifdef __cplusplus
}
#endif

#endif
