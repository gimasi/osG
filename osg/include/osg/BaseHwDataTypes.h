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
#ifndef OSG_BASEHWDATATYPES_H
#define OSG_BASEHWDATATYPES_H

#include <stdint.h>
#include "drivers/ThreadDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief The user application type.
/// @ingroup Base
typedef void (*osg_BaseHwApplication)(void);

struct osg_BaseHwConfig;
typedef struct osg_BaseHwConfig osg_BaseHwConfig;
/// @brief The user main configuration settings.
/// @ingroup Base
struct osg_BaseHwConfig
{
    /// @brief The attributes for the main application thread in case of RTOS.
    osg_ThreadAttributes threadAttributes;
};

#ifdef __cplusplus
}
#endif

#endif
