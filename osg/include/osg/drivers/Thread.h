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
#ifndef OSG_DRIVERS_THREAD_H
#define OSG_DRIVERS_THREAD_H

#include "../osgDataTypes.h"
#include "ThreadDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Thread Thread
/// @ingroup Drivers
/// Methods for Thread

/// @brief Constructor for thread.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @param function The function to call.
/// @param argument Argument passed to function.
/// @param attributes Thread's attributes (NULL: default values).
void osg_Thread_ctor(osg_Thread * self, osg_ThreadFunction function, void * argument, void * attributes);
/// @brief Destructor.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
void osg_Thread_dtor(osg_Thread * self);
/// @brief Start the thread.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @return TRUE if the thread correctly started.
Bool osg_Thread_resume(osg_Thread * self);
/// @brief Stop the thread.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @return TRUE if the thread correctly stopped.
Bool osg_Thread_pause(osg_Thread * self);
/// @brief Check if a thread is running.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @return TRUE if the thread is running.
Bool osg_Thread_isRunning(osg_Thread * self);
/// @brief Check if a thread is running.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @return The thread state.
osg_ThreadState osg_Thread_getState(osg_Thread * self);
/// @brief Sets the thread in detached mode.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @return TRUE on success.
Bool osg_Thread_detach(osg_Thread * self);
/// @brief Returns the thread low level handler.
/// @ingroup Thread
/// @memberof osg_Thread
/// @param self The thread object.
/// @return The low level handler.
void * osg_Thread_getHandler(osg_Thread * self);

#ifdef __cplusplus
}
#endif

#endif
