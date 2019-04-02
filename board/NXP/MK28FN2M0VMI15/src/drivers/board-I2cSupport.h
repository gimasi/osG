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
#ifndef OSG_BOARD_DRIVERS_I2CSUPPORT_H
#define OSG_BOARD_DRIVERS_I2CSUPPORT_H

#define I2C_MUTEX_ENABLED   defined(OSG_OS_NAME) && (OSG_OS_NAME != OSG_OS_NONE) && defined(OSG_OS_USE_MUTEX) && OSG_OS_USE_MUTEX == 1

#include <fsl_i2c.h>
#include <fsl_port.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef I2C_Type osg_board_I2cInstance;
struct osg_board_I2cSlaveData;
typedef struct osg_board_I2cSlaveData osg_board_I2cSlaveData;
struct osg_board_I2cSlaveData
{
    int state;
    Size dataSize;
    uint8_t * data;
};
union osg_board_I2cHandler;
typedef union osg_board_I2cHandler osg_board_I2cHandler;
union osg_board_I2cHandler
{
    i2c_master_handle_t masterHandle;
    i2c_slave_handle_t slaveHandle;
};

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_I2c_getI2cs(void);
uint8_t _osg_board_I2c_getDeviceIndex(const osg_I2cId id);
osg_I2cImpl * _osg_board_I2c_getI2cByInstance(void * handler);
osg_I2cImpl * _osg_board_I2c_getI2cById(const osg_I2cId id);
osg_board_I2cInstance * _osg_board_I2c_getI2cInstance(const osg_I2cId id);
osg_board_I2cHandler * _osg_board_I2c_getI2cHandler(const osg_I2cId id);
osg_board_I2cHandler * _osg_board_I2c_getI2cHandlerByInstance(void * handler);
osg_board_I2cSlaveData * _osg_board_I2c_getNonBlockingTransferSlaveData(void);
#if I2C_MUTEX_ENABLED
void _osg_board_I2c_configureMutex(const osg_I2cId id);
osg_Mutex * _osg_board_I2c_getMutex(const osg_I2cId id);
#endif

// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////
bool _osg_board_I2c_checkI2c(const osg_I2cId i2c);
bool _osg_board_I2c_validatePins(const osg_I2cId i2cId, const osg_GpioId sda, const osg_GpioId scl);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////
i2c_slave_address_mode_t _osg_board_I2c_decodeAddressingMode(const osg_I2cAddressingMode addrMode);
//port_mux_t _osg_board_I2c_decodePinMuxAlt(const osg_GpioId pin);
uint32_t _osg_board_I2c_getI2cClk(const osg_I2cId id);

// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////
//osg_GpioAlternateFunction _osg_board_I2c_encodeAlternateFunction(const osg_I2cId i2cId);

// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////


// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_I2c_doBufferedSend(osg_I2cImpl * i2c, const osg_I2cMode mode);
void _osg_board_I2c_doBufferedReceive(osg_I2cImpl * i2c, const osg_I2cMode mode);
void _osg_board_I2c_masterBlockingCallback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
void _osg_board_I2c_masterSendCallback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
void _osg_board_I2c_masterRecvCallback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
void _osg_board_I2c_slaveBlockingCallback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData);
void _osg_board_I2c_slaveSendRecvCallback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData);
void _osg_board_I2c_timeoutCallback(void * argument);


#ifdef __cplusplus
}
#endif

#endif
