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
#ifndef OSG_RTOS_DRIVERS_THREAD_H
#define OSG_RTOS_DRIVERS_THREAD_H

#include "../rtos-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup RTOS-Thread RTOS-Thread
/// @ingroup RTOS-Drivers
/// RTOS methods for Thread

/// @brief Constructor for thread.
/// @ingroup RTOS-Thread
/// @param self The thread object.
/// @param config The config.
void osg_rtos_Thread_ctor(osg_Thread * self, const osg_ThreadConfig * const config);

/// @brief Destructor.
/// @ingroup RTOS-Thread
/// @param self The thread object.
void osg_rtos_Thread_dtor(osg_Thread * self);

/// @brief Start the thread.
/// @ingroup RTOS-Thread
/// @param self The thread object.
/// @return true if the thread correctly started.
bool osg_rtos_Thread_resume(osg_Thread * self);

/// @brief Stop the thread.
/// @ingroup RTOS-Thread
/// @param self The thread object.
/// @return true if the thread correctly stopped.
bool osg_rtos_Thread_pause(osg_Thread * self);

/// @brief Stop the thread.
/// @ingroup RTOS-Thread
/// @param id The thread id.
/// @return true if the thread correctly stopped.
bool osg_rtos_Thread_suspend(void * id);

/// @brief Check if a thread is running.
/// @ingroup RTOS-Thread
/// @param self The thread object.
/// @return The thread state.
osg_ThreadState osg_rtos_Thread_getState(osg_Thread * self);

/// @brief Sets the thread in detached mode.
/// @ingroup RTOS-Thread
/// @param self The thread object.
/// @return true on success.
bool osg_rtos_Thread_detach(osg_Thread * self);

/// @brief Returns the thread id.
/// @ingroup RTOS-Thread
/// @return The low level id.
void * osg_rtos_Thread_getThreadId(void);

/// @brief Pass control to next thread that is in state READY.
/// @ingroup RTOS-Thread
void osg_rtos_Thread_yieldNextThread(void);

#ifdef __cplusplus
}
#endif

#endif
