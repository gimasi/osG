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
#ifndef OSG_RTOS_DRIVERS_MUTEXSUPPORT_H
#define OSG_RTOS_DRIVERS_MUTEXSUPPORT_H

#include <stdint.h>
#include "../../../include/rtos/rtos-forwards.h"
#include "../../include/rtos/rtos-config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef osMutexId_t osg_rtos_Mutex;

void _osg_rtos_Mutex_populateAttr(osMutexAttr_t* attr, uint32_t flags, const char * name)
{
    uint32_t tmp  = 0;
    
    if (flags & OSG_MUTEX_RECURSIVE_ENABLED)
    {
        tmp |= osMutexRecursive;
    }

    if (flags & OSG_MUTEX_PRIORITY_INHERITANCE)
    {
        tmp |= osMutexPrioInherit;
    }

    if (flags & OSG_MUTEX_RELEASE_WHEN_THREAD_EXIT)
    {
        tmp |= osMutexRobust;
    }

    attr->attr_bits = tmp;
    attr->name = name;
    attr->cb_mem = NULL;
    attr->cb_size = 0;
}

#ifdef __cplusplus
}
#endif

#endif
