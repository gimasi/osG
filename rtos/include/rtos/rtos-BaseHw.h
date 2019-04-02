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
#ifndef OSG_RTOS_BASEHW_H
#define OSG_RTOS_BASEHW_H

#include <stdint.h>
#include "rtos-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup RTOS-Base RTOS-Base
/// @ingroup RTOS
/// RTOS methods for base operations and general types

/// @brief Performs base RTOS initialization and starts the application.
/// @ingroup RTOS-Base
/// @param application The user application to run.
/// @return true on success.
bool osg_rtos_baseHw_start(osg_BaseHwApplication application);

/// @brief Performs base RTOS initialization and starts the application with custom settings.
/// @ingroup RTOS-Base
/// @param application The user application to run.
/// @param attributes The custom thread attributes to apply.
/// @return true on success.
bool osg_rtos_baseHw_startCustom(osg_BaseHwApplication application, const osg_BaseHwConfig * attributes);

/// @brief Perform a wait.
/// @ingroup RTOS-Base
/// @param millis The milliseconds to wait.
void osg_rtos_baseHw_wait(const uint32_t millis);

/// @brief Gets tick count.
/// @ingroup RTOS-Base
/// @return Tick count.
uint32_t osg_rtos_baseHw_getTick(void);

#ifdef __cplusplus
}
#endif

#endif
