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
/// @param config The led configuration.
void osg_Led_ctor(osg_Led * self, const osg_LedConfig * const config);

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

/// @brief Gets the led status.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return true if the Led is ON.
bool osg_Led_isOn(osg_Led * self);

/// @brief Toggles the led status.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
void osg_Led_toggle(osg_Led * self);

/// @brief Set the blinking period of the Led after call the start/toggle function. Default value is 500ms.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @param millis The blinking period expressed in milliseconds. Default is 500.
void osg_Led_setBlinkingPeriod(osg_Led * self, uint32_t millis);

/// @brief Set the number of Led blinks. When the count down reachs 0, then the led stop blinking; only the ON status is take into account. Default value is 0 = forever.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @param numberOfBlinks The number of blinks. Default value is 0 (forever).
void osg_Led_setNumberOfBlinks(osg_Led * self, uint32_t numberOfBlinks);

/// @brief Get the blinking period.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return The blinking period in milliseconds.
uint32_t osg_Led_getBlinkingPeriod(osg_Led * self);

/// @brief Get the total number of blinks.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return The total number of blinks.
uint32_t osg_Led_getNumberOfBlinks(osg_Led * self);

/// @brief Toggles the blinking led status.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return True on success
bool osg_Led_toggleBlinking(osg_Led * self);

/// @brief Start blinking the Led with the last (or default) period and number of blinks set.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return True on success
bool osg_Led_startBlinking(osg_Led * self);

/// @brief Stop blinking the led. It deos not affect the period and number of blinks.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return True on success
bool osg_Led_stopBlinking(osg_Led * self);

/// @brief Get the Led blinking status.
/// @ingroup Led
/// @memberof osg_Led
/// @param self The led object.
/// @return true if the led is blinking.
bool osg_Led_isBlinking(osg_Led * self);

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
osg_GpioId osg_Led_getGpioId(const osg_LedId id);

#ifdef __cplusplus
}
#endif

#endif
