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
#ifndef OSG_BASEHW_H
#define OSG_BASEHW_H

#include "osgDataTypes.h"
#include "BaseHwDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Casts the condition result to the bool type
/// @ingroup Base
#define osg_bool(cond) ((cond)? true : false)

/// @brief Performs base HW initialization and system clock initialization.
/// @ingroup Base
/// @param application The main routine to execute as application main
/// @return true on success.
bool osg_baseHw_start(osg_BaseHwApplication application);

/// @brief Performs base HW initialization and starts the application with custom settings (useful only if RTOS enabled).
/// @ingroup Base
/// @param application The user application to run.
/// @param attributes The custom thread attributes to apply; ignored if RTOS is disabled.
/// @return true on success.
bool osg_baseHw_startCustom(osg_BaseHwApplication application, const osg_BaseHwConfig * attributes);

/// @brief Waits a delay in milliseconds
/// @ingroup Base
/// @param millis The time to wait.
void osg_baseHw_wait(const uint32_t millis);

/// @brief Gets tick count
/// @ingroup Base
/// @return Tick count.
uint32_t osg_baseHw_getTick(void);

#ifdef __cplusplus
}
#endif

#endif
