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
#ifndef OSG_DRIVERS_I2CDATATYPES_H
#define OSG_DRIVERS_I2CDATATYPES_H

#include "../osgDataTypes.h"
#include "GpioDataTypes.h"
#include "../utils/CircularFifo.h"

#ifdef __cplusplus
extern "C" {
#endif

enum osg_I2cMode;
typedef enum osg_I2cMode osg_I2cMode;
/// @brief The possible I2C operation mode.
/// @ingroup I2C
/// @memberof osg_I2c
enum osg_I2cMode
{
    OSG_I2C_MASTER_MODE,
    OSG_I2C_SLAVE_MODE
};

enum osg_I2cId;
typedef enum osg_I2cId osg_I2cId;
/// @brief The possible I2C IDs.
/// @ingroup I2C
/// @memberof osg_I2c
enum osg_I2cId
{
    OSG_I2C0,
    OSG_I2C1,
    OSG_I2C2,
    OSG_I2C3,
    OSG_I2C4,
    OSG_I2C5,
    OSG_I2C6,
    OSG_I2C7
};

enum osg_I2cAddressingMode;
typedef enum osg_I2cAddressingMode osg_I2cAddressingMode;
/// @brief The possible I2C addressing mode.
/// @ingroup I2C
/// @memberof osg_I2c
enum osg_I2cAddressingMode
{
    OSG_I2C_7BIT_ADDRESS,
    OSG_I2C_10BIT_ADDRESS
};

/// @brief Convenience type for I2C callback
/// @ingroup I2C
/// @memberof osg_I2c
typedef void (* osg_I2cCallback)(void);

struct osg_I2c;
typedef struct osg_I2c osg_I2c;
/// @brief The I2C class.
/// @ingroup I2C
/// @memberof osg_I2c
struct osg_I2c
{
    /// @brief The ID
    /// @private
    osg_I2cId id;
    /// @brief Mode
    /// @private
    osg_I2cMode mode;
    /// @brief SDA pin ID
    /// @private
    osg_Gpio sdaPin;
    /// @brief  SCL pin ID
    /// @private
    osg_Gpio sclPin;
};

enum osg_I2cFlags;
typedef enum osg_I2cFlags osg_I2cFlags;
/// @brief Enum used to represent I2C internal bit flags
/// @ingroup I2C
/// @memberof osg_I2c
enum osg_I2cFlags
{
    /// @brief Initial value
    OSG_I2C_FLAG_DEFAULT         = 0x00,
    /// @brief I2C is performing a non-blocking communication
    OSG_I2C_FLAG_IS_NB_SEND      = 0x02,
    /// @brief I2C is performing a non-blocking communication
    OSG_I2C_FLAG_IS_NB_RECEIVE   = 0x04,
    /// @brief I2C is sending data
    OSG_I2C_FLAG_IS_SEND         = 0x08,
    /// @brief I2C is receiving data
    OSG_I2C_FLAG_IS_RECEIVE      = 0x10,
    /// @brief I2C buffered rx process is enabled
    OSG_I2C_FLAG_RX_BUFF_ENABLED = 0x20,
    /// @brief I2C wait for aborting rx process
    OSG_I2C_FLAG_RX_ABORTED      = 0x40,
    /// @brief I2C configuration done.
    OSG_I2C_FLAG_HANDLE_CREATED  = 0x80
};

struct osg_I2cImpl;
typedef struct osg_I2cImpl osg_I2cImpl;
/// @brief I2c internal struct.
/// @ingroup I2C
/// @memberof osg_I2c
struct osg_I2cImpl
{
    /// @brief Low-level handler
    /// @private
    void * handler;
    /// @brief Send callback
    /// @private
    osg_I2cCallback txCompleteNb;
    /// @brief receive callback
    /// @private
    osg_I2cCallback rxCompleteNb;
    /// @brief Send callback
    /// @private
    osg_I2cCallback txCompleteBuffered;
    /// @brief receive callback
    /// @private
    osg_I2cCallback rxCompleteBuffered;
    /// @brief Internal flags
    /// @private
    osg_I2cFlags flags;
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
    uint16_t slaveAddress;
};

#ifdef __cplusplus
}
#endif

#endif
