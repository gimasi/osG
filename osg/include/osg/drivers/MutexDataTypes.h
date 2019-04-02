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
#ifndef OSG_DRIVERS_MUTEXDATATYPES_H
#define OSG_DRIVERS_MUTEXDATATYPES_H

#ifdef __cplusplus
extern "C" {
#endif

enum osg_MutexAttributes;
typedef enum osg_MutexAttributes osg_MutexAttributes;
/// @brief The possible mutex attributes
/// @ingroup Mutex
/// @memberof osg_Mutex
enum osg_MutexAttributes
{
    OSG_MUTEX_NO_ATTRIBUTES             = 0x00,
    OSG_MUTEX_RECURSIVE_ENABLED         = 0x02,
    OSG_MUTEX_PRIORITY_INHERITANCE      = 0x04,
    OSG_MUTEX_RELEASE_WHEN_THREAD_EXIT  = 0x08
};

enum osg_MutexStatus;
typedef enum osg_MutexStatus osg_MutexStatus;
/// @brief The possible mutex status
/// @ingroup Mutex
/// @memberof osg_Mutex
enum osg_MutexStatus
{
    OSG_MUTEX_STATUS_UNDEFINED    = 0x00,
    OSG_MUTEX_STATUS_CONFIGURED   = 0x02,
};

struct osg_MutexConfig;
typedef struct osg_MutexConfig osg_MutexConfig;
/// @brief The mutex configuration.
/// @ingroup Mutex
/// @memberof osg_Mutex
struct osg_MutexConfig
{
    /// @brief Mutex's attributes (Use logical 'OR' operation to select multiple options).
    uint32_t attributes;
    /// @brief Optional name for the mutex.
    char * name;
};

struct osg_Mutex;
typedef struct osg_Mutex osg_Mutex;
/// @brief The mutex class.
/// @ingroup Mutex
/// @memberof osg_Mutex
struct osg_Mutex
{
    /// @brief Low-level handler
    /// @private
    void * handler;
    /// @brief Internal status
    /// @private
    osg_MutexStatus status;
};

#ifdef __cplusplus
}
#endif

#endif
