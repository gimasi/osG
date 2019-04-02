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
#ifndef OSG_BOARD_LED_H
#define OSG_BOARD_LED_H

#include "../board-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Board-Led Board-Led
/// @ingroup Board-Drivers
/// Board methods for Led

/// @brief Maps the led ID into the matching GPIO ID
/// @ingroup Board-Led
/// @param id The Led ID
/// @return The GPIO ID
osg_GpioId osg_board_Led_getGpioId(const osg_LedId id);

#ifdef __cplusplus
}
#endif

#endif
