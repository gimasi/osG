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

#ifdef __cplusplus
extern "C" {
#endif

struct osg_Led;
typedef struct osg_Led osg_Led;
/// @brief The led class.
/// It is a convenience wrapper to the matching GPIO.
/// @ingroup Led
/// @memberof osg_Led
struct osg_Led
{
    /// @brief  The wrapped GPIO
    /// @private
    osg_Gpio gpio;
    /// @brief Internal timer
    /// @private
    osg_Timer timer;
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

#ifdef __cplusplus
}
#endif

#endif
