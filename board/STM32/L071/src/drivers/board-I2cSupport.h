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

#include <stm32l0xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef I2C_HandleTypeDef osg_board_I2cHandler;
typedef I2C_TypeDef osg_board_I2cInstance;

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_I2c_getI2cs(void);
uint8_t _osg_board_I2c_getDeviceIndex(const osg_I2cId id);
osg_I2cImpl * _osg_board_I2c_getI2cByHandler(void * handler);
osg_I2cImpl * _osg_board_I2c_getI2cById(const osg_I2cId id);
osg_board_I2cInstance * _osg_board_I2c_getI2cInstance(const osg_I2cId id);
osg_board_I2cHandler * _osg_board_I2c_getI2cHandler(const osg_I2cId id);
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
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////
HAL_I2C_ModeTypeDef _osg_board_I2c_decodeMode(const osg_I2cMode mode);
uint32_t _osg_board_I2c_decodeAddressingMode(const osg_I2cAddressingMode addrMode);

// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////
//osg_GpioAlternateFunction _osg_board_I2c_encodeAlternateFunction(const osg_I2cId i2cId);

// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////
void _osg_board_I2c_enableI2c(osg_board_I2cHandler * handler);
void _osg_board_I2c_enableI2cClk(const osg_I2cId id);
void _osg_board_I2c_enableI2cInterrupt(const osg_I2cId id);
void _osg_board_I2c_resetI2c(const osg_I2cId id);
void _osg_board_I2c_disableI2cClk(const osg_I2cId id);
void _osg_board_I2c_disableI2cInterrupt(const osg_I2cId id);

// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_I2c_doBufferedSend(osg_I2cImpl * i2c, const osg_I2cMode mode);
void _osg_board_I2c_doBufferedReceive(osg_I2cImpl * i2c, const osg_I2cMode mode);


#ifdef __cplusplus
}
#endif

#endif
