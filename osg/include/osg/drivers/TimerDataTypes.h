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
#ifndef OSG_DRIVERS_TIMERDATATYPES_H
#define OSG_DRIVERS_TIMERDATATYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/// @brief The timer callback type.
/// @memberof osg_Timer
typedef void (*osg_TimerCallback)(void *);

enum osg_TimerType;
typedef enum osg_TimerType osg_TimerType;
/// @brief The possible timer types.
/// @ingroup Timer
/// @memberof osg_Timer
enum osg_TimerType
{
    OSG_TIMER_ONE_SHOT,
    OSG_TIMER_PERIODIC
};

struct osg_Timer;
typedef struct osg_Timer osg_Timer;
/// @brief The timer class.
/// @ingroup Timer
/// @memberof osg_Timer
/// @brief It is a convenience wrapper.
struct osg_Timer
{
    /// @brief Low-level handler
    /// @private
    void * handler;
};

#ifdef __cplusplus
}
#endif

#endif
