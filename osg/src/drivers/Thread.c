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
#include "../../include/osg/drivers/Thread.h"
#include "../../include/osg/utils.h"

#ifdef OSG_HAS_CONFIG_FILE
    #include <osgConfig.h>
#else
    #include "../../../templates/osgConfig.h"
#endif

#if !(OSG_OS_NAME == OSG_OS_NONE)
#include "../../../rtos/include/rtos.h"
#endif

void osg_Thread_ctor(osg_Thread * self,  const osg_ThreadConfig * const config)
{
#if !(OSG_OS_NAME == OSG_OS_NONE)
    osg_rtos_Thread_ctor(self, config);
#endif
}

void osg_Thread_dtor(osg_Thread * self)
{
#if !(OSG_OS_NAME == OSG_OS_NONE)
    osg_rtos_Thread_dtor(self);
#endif
}

bool osg_Thread_resume(osg_Thread * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_Thread_resume(self);
#endif
}

bool osg_Thread_pause(osg_Thread * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_Thread_pause(self);
#endif
}

bool osg_Thread_suspend(void * id)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_Thread_suspend(id);
#endif
}

bool osg_Thread_isRunning(osg_Thread * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return (osg_rtos_Thread_getState(self) == OSG_THREAD_RUNNING ? true : false);
#endif
}

osg_ThreadState osg_Thread_getState(osg_Thread * self)
{
    if (self->handler == NULL)
        return OSG_THREAD_INACTIVE;

#if (OSG_OS_NAME == OSG_OS_NONE)
    return OSG_THREAD_ERROR;
#else
    return osg_rtos_Thread_getState(self);
#endif
}

bool osg_Thread_detach(osg_Thread * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_Thread_detach(self);
#endif
}

void * osg_Thread_getThreadId()
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return NULL;
#else
    return osg_rtos_Thread_getThreadId();
#endif
}

void osg_Thread_yieldNextThread(void)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    // ntd
#else
    osg_rtos_Thread_yieldNextThread();
#endif
}
