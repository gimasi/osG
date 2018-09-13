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



/// @brief Performs base HW initialization and system clock initialization.
/// @ingroup Base
/// @param application The main routine to execute as application main
/// @return TRUE on success.
Bool osg_baseHw_start(osg_BaseHwApplication application);
/// @brief Waits a delay in milliseconds
/// @ingroup Base
/// @param millis The time to wait.
void osg_baseHw_wait(const uint32_t millis);

#ifdef __cplusplus
}
#endif

#endif
