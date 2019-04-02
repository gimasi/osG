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
#ifndef OSG_BOARD_DRIVERS_TIMER_H
#define OSG_BOARD_DRIVERS_TIMER_H

#include <stdint.h>
#include "../board-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Board-Timer Board-Timer
/// @ingroup Board-Drivers
/// Board methods for Timer

/// @brief Constructor for timer.
/// @ingroup Board-Timer
/// @param self The timer object.
/// @param config The timer type config.
void osg_board_Timer_ctor(osg_Timer * self, const osg_TimerConfig * const config);
/// @brief Destructor.
/// @ingroup Board-Timer
/// @param self The timer object.
void osg_board_Timer_dtor(osg_Timer * self);
/// @brief Start the timer.
/// @ingroup Board-Timer
/// @param self The timer object.
/// @param millis Value of the timer in milliseconds.
/// @return true if the timer correctly started.
bool osg_board_Timer_start(osg_Timer * self, const uint32_t millis);
/// @brief Stop the timer.
/// @ingroup Board-Timer
/// @param self The timer object.
/// @return true if the timer correctly stopped.
bool osg_board_Timer_stop(osg_Timer * self);
/// @brief Check if a timer is running.
/// @ingroup Board-Timer
/// @param self The timer object.
/// @return true if the timer is running.
bool osg_board_Timer_isRunning(osg_Timer * self);
/// @brief Returns the timer type.
/// @ingroup Board-Timer
/// @param self The timer.
/// @return The timer type
osg_TimerType osg_board_Timer_getType(osg_Timer * self);

#ifdef __cplusplus
}
#endif

#endif
