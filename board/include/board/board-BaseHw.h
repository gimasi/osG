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
#ifndef OSG_BOARD_BASEHW_H
#define OSG_BOARD_BASEHW_H

#include <stdint.h>
#include "board-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Board-Base Board-Base
/// @ingroup Board
/// Board methods for base operations and general types

/// @brief Method to setup system clock.
/// It is implicitally called, and can be overwritten (it is a weak symbol).
/// @ingroup Board-Base
/// @return true on success
bool osg_board_baseHw_systemClockConfig(void);

/// @brief Performs base HW initialization and starts the application.
/// Implicitally calls osg_board_baseHw_systemClockConfig()
/// @ingroup Board-Base
/// @param application The user application to run.
/// @return true on success.
bool osg_board_baseHw_start(osg_BaseHwApplication application);

/// @brief Perform a wait.
/// @ingroup Board-Base
/// @param millis The milliseconds to wait.
void osg_board_baseHw_wait(const uint32_t millis);

/// @brief Performs base HW initialization and starts the application.
/// Implicitally calls osg_board_baseHw_systemClockConfig()
/// @ingroup Board-Base
/// @return true on success.
bool osg_board_baseHw_init(void);

/// @brief Gets tick count.
/// @ingroup Board-Base
/// @return Tick count.
uint32_t osg_board_baseHw_getTick(void);

#ifdef __cplusplus
}
#endif

#endif
