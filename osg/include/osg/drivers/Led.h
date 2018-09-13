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
#ifndef OSG_DRIVERS_LED_H
#define OSG_DRIVERS_LED_H

#include "../osgDataTypes.h"
#include "LedDataTypes.h"
#include "GpioDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Led Led
/// @ingroup Drivers
/// Methods for Led

/// @brief Constructor for board Led.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @param id The led id of the board.
void osg_Led_ctor(osg_Led * self, const osg_LedId id);
/// @brief Constructor for custom Led.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @param id The Gpio id connected to external Led.
void osg_Led_customCtor(osg_Led * self, const osg_GpioId id);
/// @brief Destructor.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
void osg_Led_dtor(osg_Led * self);
/// @brief Turns the led on.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
void osg_Led_turnOn(osg_Led * self);
/// @brief Turns the led off.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
void osg_Led_turnOff(osg_Led * self);
/// @brief Toggles the led status.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
void osg_Led_toggle(osg_Led * self);
/// @brief Start blinking the led forever.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @param millis The frequency in milliseconds.
/// @return True on success
Bool osg_Led_startBlinking(osg_Led * self, uint32_t millis);
/// @brief Stop blinking the led.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return True on success
Bool osg_Led_stopBlinking(osg_Led * self);
/// @brief Gets the wrapped GPIO object.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return The GPIO internal object
osg_Gpio * osg_Led_toGpio(osg_Led * self);
/// @brief Maps the led id to the matching GPIO id.
/// @ingroup Led
/// @memberof osg_Led
/// @param id The led id.
/// @return The GPIO id.
osg_GpioId osg_led_getGpioId(const osg_LedId id);

#ifdef __cplusplus
}
#endif

#endif
