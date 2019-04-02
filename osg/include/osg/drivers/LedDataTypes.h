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
#ifndef OSG_DRIVERS_LEDDATATYPES_H
#define OSG_DRIVERS_LEDDATATYPES_H

#include "GpioDataTypes.h"
#include "TimerDataTypes.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum osg_LedFlags;
typedef enum osg_LedFlags osg_LedFlags;
/// @brief The Led flags.
/// @ingroup Led
/// @memberof osg_Led
enum osg_LedFlags
{
    OSG_LED_FLAG_DEFAULT          = 0x00,
    OSG_LED_FLAG_BLINKING_ENABLED = 0x02,
    OSG_LED_FLAG_TIMER_CREATED    = 0x04
};

struct osg_Led;
typedef struct osg_Led osg_Led;
/// @brief The led class.
/// It is a convenience wrapper to the matching GPIO.
/// @ingroup Led
/// @memberof osg_Led
struct osg_Led
{
    /// @brief  The wrapped GPIO.
    /// @private
    osg_Gpio gpio;
    /// @brief Internal timer.
    /// @private
    osg_Timer timer;    /// @brief Blinking Period expressed in milliseconds.
    /// @private
    uint32_t millis;
    /// @brief How many times the Led has to blink. Set 0 for "infinite".
    /// @private
    uint32_t totalNumberOfBlinks;
    /// @brief This field is valid only if @p totalNumberOfBlinks > 0. Count down of blinking times. When it reaches 0, then the Led stop blinking.
    /// @private
    uint32_t remainingNumberOfBlinks;
    /// @brief Led flags
    /// @private
    osg_LedFlags flags;
};

enum osg_LedId;
typedef enum osg_LedId osg_LedId;
/// @brief The possible led IDs.
/// @ingroup Led
/// @memberof osg_Led
/// @todo Add macros to select available IDs
enum osg_LedId
{
    OSG_LED1,
    OSG_LED2,
    OSG_LED3,
    OSG_LED4,
    OSG_LED_GREEN,
    OSG_LED_RED,
    OSG_LED_ORANGE,
    OSG_LED_BLUE
};

struct osg_LedConfig;
typedef struct osg_LedConfig osg_LedConfig;
/// @brief The configuration for a led.
/// @ingroup Led
/// @memberof osg_Led
struct osg_LedConfig
{
    /// @brief The led ID.
    /// Considered only if gpioId is unset (holds OSG_GPIO_NONE)
    osg_LedId ledId;
    /// @brief The Gpio ID for the led.
    /// If set to OSG_GPIO_NONE, the ledId is used.
    osg_GpioId gpioId;
};

#ifdef __cplusplus
}
#endif

#endif
