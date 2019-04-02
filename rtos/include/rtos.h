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
#ifndef OSG_RTOS_H
#define OSG_RTOS_H

#ifdef OSG_HAS_CONFIG_FILE
#include <osgConfig.h>
#else
#include "../../templates/osgConfig.h"
#endif

/// @defgroup RTOS RTOS
/// The internal API for RTOS support

/// @defgroup RTOS-Base RTOS-Base
/// @ingroup RTOS
/// General data types and base HW methods
#include "rtos/rtos-forwards.h"
#include "rtos/rtos-BaseHw.h"
/// @defgroup RTOS-Drivers RTOS-Drivers
/// @ingroup RTOS
/// Hardware driver classes
#include "rtos/rtos-drivers.h"

// Select the correct rtos
#if defined OSG_OS_NAME && OSG_OS_NAME == OSG_OS_CMSIS_RTOS2
    // RTOS2
    #include "../rtos2/include/rtos/rtos-config.h"
    #if OSG_OS_USE_MUTEX == 1
        #include "../../rtos/rtos2/src/drivers/rtos-MutexSupport.h"
    #endif
#else
    #error "INVALID BOARD FAMILY. Check the 'osgConfig.h' file."
#endif

#endif
