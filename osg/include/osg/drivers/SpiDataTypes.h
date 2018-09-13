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
    OSG_SPI_DATA_8_BITS,
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

struct osg_Spi;
typedef struct osg_Spi osg_Spi;
/// @brief The SPI class.
/// @ingroup SPI
/// @memberof osg_Spi
struct osg_Spi
{
    /// @brief Low-level handler.
    /// @private
    void * handler;
    /// @brief The ID.
    /// @private
    osg_SpiId id;
    /// @brief Mode.
    /// @private
    osg_SpiMode mode;
    /// @brief Configured flag.
    /// @private
    Bool isConfigured;
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
    /// @brief Circular FIFO used for bufferes reception.
    /// @private
    osg_CircularFifo rxBuffer;
    /// @brief Circular FIFO used for buffered transmission.
    /// @private
    osg_CircularFifo txBuffer;
};

#ifdef __cplusplus
}
#endif

#endif
