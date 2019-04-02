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
#ifndef OSG_DRIVERS_MUTEX_H
#define OSG_DRIVERS_MUTEX_H

#include "../osgDataTypes.h"
#include "MutexDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Mutex Mutex
/// @ingroup Drivers
/// Methods for Mutex

/// @brief Constructor for mutex.
/// @ingroup Mutex
/// @memberof osg_Mutex
/// @param self The mutex object.
/// @param config The mutex configuration
void osg_Mutex_ctor(osg_Mutex * self, const osg_MutexConfig * const config);

/// @brief Destructor.
/// @ingroup Mutex
/// @memberof osg_Mutex
/// @param self The mutex object.
void osg_Mutex_dtor(osg_Mutex * self);

/// @brief Acquires the mutex.
/// @ingroup Mutex
/// @memberof osg_Mutex
/// @param self The mutex object.
/// @param timeout A timeout value of 0 means that the RTOS does not wait and the function returns instantly.
/// @return true if the mutex is correctly acquired.
bool osg_Mutex_acquire(osg_Mutex * self, const uint32_t timeout);

/// @brief Releases the mutex.
/// @ingroup Mutex
/// @memberof osg_Mutex
/// @param self The mutex object.
/// @return true if the mutex is correctly released.
bool osg_Mutex_release(osg_Mutex * self);

/// @brief Gets the mutex thread owner id.
/// @ingroup Mutex
/// @memberof osg_Mutex
/// @param self The mutex object.
/// @return The mutex thread ownr id.
void * osg_Mutex_getOwner(osg_Mutex * self);

/// @brief Gets the mutex name.
/// @ingroup Mutex
/// @memberof osg_Mutex
/// @param self The mutex object.
/// @return The mutex name.
const char * osg_Mutex_getName(osg_Mutex * self);

#ifdef __cplusplus
}
#endif

#endif
