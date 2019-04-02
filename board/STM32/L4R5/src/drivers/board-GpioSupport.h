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
#ifndef OSG_BOARD_DRIVERS_GPIOSUPPORT_H
#define OSG_BOARD_DRIVERS_GPIOSUPPORT_H

#include <stm32l4xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef GPIO_TypeDef osg_board_Gpio;

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
osg_board_Gpio * _osg_board_Gpio_getPortHandler(const osg_GpioPort port);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////
uint32_t _osg_board_Gpio_decodeMode(const osg_GpioMode value);
uint32_t _osg_board_Gpio_decodePull(const osg_GpioPull value);
uint32_t _osg_board_Gpio_decodeSpeed(const osg_GpioSpeed value);
//uint32_t _osg_board_Gpio_decodeAlternate(const osg_GpioAlternateFunction value);
uint16_t _osg_board_Gpio_decodeGpioPin(const osg_GpioPin osgPin);
IRQn_Type _osg_board_Gpio_decodeIrqType(const osg_GpioPin pin);

// ///////////////////////////////////////////////////////
// ENCODE FROM STM TO OSG
// ///////////////////////////////////////////////////////
osg_GpioPin _osg_board_Gpio_encodeGpioPin(const uint16_t stm32Pin);

// ///////////////////////////////////////////////////////
// ENABLE
// ///////////////////////////////////////////////////////
void _osg_board_Gpio_enablePort(const osg_GpioPort port);

// ///////////////////////////////////////////////////////
// SUPPORT TO USER CALLBACK
// ///////////////////////////////////////////////////////
osg_GpioInterruptCallback _osg_board_Gpio_getCallback(const osg_GpioPin pin);
void _osg_board_Gpio_setCallback(const osg_GpioPin pin, const osg_GpioInterruptCallback callback);
void _osg_board_Gpio_callback(const uint16_t pin);


#ifdef __cplusplus
}
#endif

#endif
