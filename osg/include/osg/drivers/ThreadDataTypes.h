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
#ifndef OSG_DRIVERS_THREADDATATYPES_H
#define OSG_DRIVERS_THREADDATATYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum osg_ThreadState;
typedef enum osg_ThreadState osg_ThreadState;
/// @brief The possible thread states
/// @ingroup Thread
/// @memberof osg_Thread
enum osg_ThreadState
{
    OSG_THREAD_INACTIVE,
    OSG_THREAD_READY,
    OSG_THREAD_RUNNING,
    OSG_THREAD_BLOCKED,
    OSG_THREAD_TERMINATED,
    OSG_THREAD_ERROR,
    OSG_THREAD_RESEVED
};

/// @brief The thread function signature.
/// @ingroup Thread
/// @memberof osg_Thread
typedef void (*osg_ThreadFunction)(void *);

struct osg_ThreadAttributes;
typedef struct osg_ThreadAttributes osg_ThreadAttributes;
/// @brief The thread attributes.
/// @ingroup Thread
/// @memberof osg_Thread
struct osg_ThreadAttributes
{
    char * name;
    uint32_t stackSize;
};

struct osg_ThreadConfig;
typedef struct osg_ThreadConfig osg_ThreadConfig;
/// @brief The thread config.
/// @ingroup Thread
/// @memberof osg_Thread
struct osg_ThreadConfig
{
    /// @brief The function to call.
    osg_ThreadFunction function;
    /// @brief Argument passed to function.
    void * argument;
    /// @brief Thread's attributes.
    osg_ThreadAttributes attributes;
};

struct osg_Thread;
typedef struct osg_Thread osg_Thread;
/// @brief The thread class.
/// @ingroup Thread
/// @memberof osg_Thread
struct osg_Thread
{
    /// @brief Low-level handler
    /// @private
    void * handler;
};

#ifdef __cplusplus
}
#endif

#endif
