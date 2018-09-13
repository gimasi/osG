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
#ifndef OSG_DRIVERS_TIMER_H
#define OSG_DRIVERS_TIMER_H

#include "TimerDataTypes.h"
#include "../osgDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Timer Timer
/// @ingroup Drivers
/// Methods for Timer

/// @brief Constructor for timer.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer object.
/// @param type The timer type: one shot or periodic.
/// @param callback The callback to call.
/// @param argument Argument passed to callback function.
void osg_Timer_ctor(osg_Timer * self, const osg_TimerType type, osg_TimerCallback callback, void * argument);
/// @brief Destructor.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer object.
void osg_Timer_dtor(osg_Timer * self);
/// @brief Start the timer.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer object.
/// @param millis Value of the timer in milliseconds.
/// @return TRUE if the timer correctly started.
Bool osg_Timer_start(osg_Timer * self, const uint32_t millis);
/// @brief Stop the timer.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer object.
/// @return TRUE if the timer correctly stopped.
Bool osg_Timer_stop(osg_Timer * self);
/// @brief Check if a timer is running.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer object.
/// @return TRUE if the timer is running.
Bool osg_Timer_isRunning(osg_Timer * self);
/// @brief Returns the internal low level handler.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer.
/// @return The internal handler
void * osg_Timer_getHandler(osg_Timer * self);
/// @brief Returns the timer type.
/// @ingroup Timer
/// @memberof osg_Timer
/// @param self The timer.
/// @return The timer type
osg_TimerType osg_Timer_getType(osg_Timer * self);

#ifdef __cplusplus
}
#endif

#endif
