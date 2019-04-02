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
#include "../../../include/rtos/drivers/rtos-Mutex.h"
#include "rtos-MutexSupport.h"
#include "../../include/rtos/rtos-config.h"
#include "../../../../osg/include/osg.h"

void osg_rtos_Mutex_ctor(osg_Mutex * self, const osg_MutexConfig * const config)
{
    if (self == NULL)
        return;

    osMutexAttr_t mutexAttr;
    _osg_rtos_Mutex_populateAttr(&mutexAttr, config->attributes, config->name);
    self->handler = osMutexNew(&mutexAttr);

    osg_assert(self->handler != NULL, "ERROR: Cannot initialize mutex.");
}

void osg_rtos_Mutex_dtor(osg_Mutex * self)
{
    if (self == NULL || self->handler == NULL)
        return;

    osMutexDelete((osg_rtos_Mutex)self->handler);
    self->handler = NULL;
}

bool osg_rtos_Mutex_acquire(osg_Mutex * self, const uint32_t timeout)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osg_bool(osMutexAcquire((osg_rtos_Mutex)self->handler, timeout) == osOK);
}

bool osg_rtos_Mutex_release(osg_Mutex * self)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osg_bool(osMutexRelease((osg_rtos_Mutex)self->handler) == osOK);
}

void * osg_rtos_Mutex_getOwner(osg_Mutex * self)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osMutexGetOwner((osg_rtos_Mutex)self->handler);
}

const char * osg_rtos_Mutex_getName(osg_Mutex * self)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osMutexGetName((osg_rtos_Mutex)self->handler);
}
