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
#include "osg/drivers/Thread.h"
#include "osg/utils.h"
#ifdef OSG_USE_RTOS
#include <rtos.h>
#endif

void osg_Thread_ctor(osg_Thread * self, osg_ThreadFunction function, void * argument, void * attributes)
{
#ifdef OSG_USE_RTOS
    osg_rtos_Thread_ctor(self, function, argument, attributes);
#endif
}

void osg_Thread_dtor(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    osg_rtos_Thread_dtor(self);
#endif
}

Bool osg_Thread_resume(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Thread_resume(self);
#else
    return FALSE;
#endif
}

Bool osg_Thread_pause(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Thread_pause(self);
#else
    return FALSE;
#endif
}

Bool osg_Thread_isRunning(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    return (osg_rtos_Thread_getState(self) == OSG_THREAD_RUNNING ? TRUE : FALSE);
#else
    return FALSE;
#endif
}

osg_ThreadState osg_Thread_getState(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Thread_getState(self);
#else
    return OSG_THREAD_ERROR;
#endif
}

Bool osg_Thread_detach(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Thread_detach(self);
#else
    return FALSE;
#endif
}

void * osg_Thread_getHandler(osg_Thread * self)
{
#ifdef OSG_USE_RTOS
    return self->handler;
#else
    return NULL;
#endif
}
