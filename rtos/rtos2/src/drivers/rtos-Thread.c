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
#include "../../../include/rtos/drivers/rtos-Thread.h"
#include "../../include/rtos/rtos-config.h"
#include "../../../../osg/include/osg.h"
#include <string.h>

static osg_ThreadState _osg_rtos_thread_getOsgThreadState(osThreadState_t state)
{
    switch (state)
    {
        case osThreadInactive: return OSG_THREAD_INACTIVE;
        case osThreadReady: return OSG_THREAD_READY;
        case osThreadRunning: return OSG_THREAD_RUNNING;
        case osThreadBlocked: return OSG_THREAD_BLOCKED;
        case osThreadTerminated: return OSG_THREAD_TERMINATED;
        case osThreadError: return OSG_THREAD_ERROR;
        case osThreadReserved: return OSG_THREAD_RESEVED;
        default:
            osg_error("ERROR: Invalid osG Thread state.");
    }

    // suppress warning
    return OSG_THREAD_ERROR;
}

void osg_rtos_Thread_ctor(osg_Thread * self, const osg_ThreadConfig * const config)
{
    if (self == NULL)
        return;

    osThreadAttr_t thAttr;
    memset(&thAttr, 0, sizeof(thAttr));
    if (config->attributes.name != NULL)
        thAttr.name = config->attributes.name;
    if (config->attributes.stackSize > 0)
        thAttr.stack_size = config->attributes.stackSize;
    
    self->handler = osThreadNew((osThreadFunc_t)config->function, config->argument, &thAttr);
    osg_assert(self->handler != NULL, "ERROR: Cannot initialize thread.");
}

void osg_rtos_Thread_dtor(osg_Thread * self)
{
    if (self == NULL || self->handler == NULL)
        return;

    osThreadTerminate((osThreadId_t)self->handler);
    self->handler = NULL;
}

bool osg_rtos_Thread_resume(osg_Thread * self)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osg_bool(osThreadResume((osThreadId_t)self->handler) == osOK);
}

bool osg_rtos_Thread_pause(osg_Thread * self)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osg_bool(osThreadSuspend((osThreadId_t)self->handler) == osOK);
}

bool osg_rtos_Thread_suspend(void * id)
{
    if (id == NULL) return false;
    
    return osg_bool(osThreadSuspend((osThreadId_t)id) == osOK);
}

osg_ThreadState osg_rtos_Thread_getState(osg_Thread * self)
{
    return _osg_rtos_thread_getOsgThreadState(osThreadGetState((osThreadId_t)self->handler));
}

bool osg_rtos_Thread_detach(osg_Thread * self)
{
    if (self == NULL || self->handler == NULL)
        return false;

    return osg_bool(osThreadDetach((osThreadId_t)self->handler) == osOK);
}

void * osg_rtos_Thread_getThreadId(void)
{
    return osThreadGetId();
}

void osg_rtos_Thread_yieldNextThread(void)
{
    osThreadYield();
}
