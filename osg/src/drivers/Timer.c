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
#include "osg/drivers/Timer.h"
#include "osg/utils.h"
#include <board.h>
#ifdef OSG_USE_RTOS
#include <rtos.h>
#endif

void osg_Timer_ctor(osg_Timer * self, const osg_TimerType type, osg_TimerCallback callback, void * argument)
{
#ifdef OSG_USE_RTOS
    osg_rtos_Timer_ctor(self, type, callback, argument);
#else
    osg_board_Timer_ctor(self, type, callback, argument);
#endif
}

void osg_Timer_dtor(osg_Timer * self)
{
#ifdef OSG_USE_RTOS
    osg_rtos_Timer_dtor(self);
#else
    osg_board_Timer_dtor(self);
#endif
}

Bool osg_Timer_start(osg_Timer * self, const uint32_t millis)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Timer_start(self, millis);
#else
    return osg_board_Timer_start(self, millis);
#endif
}

Bool osg_Timer_stop(osg_Timer * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Timer_stop(self);
#else
    return osg_board_Timer_stop(self);
#endif
}

Bool osg_Timer_isRunning(osg_Timer * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Timer_isRunning(self);
#else
    return osg_board_Timer_isRunning(self);
#endif
}

void * osg_Timer_getHandler(osg_Timer * self)
{
    return self->handler;
}

osg_TimerType osg_Timer_getType(osg_Timer * self)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_Timer_getType(self);
#else
    return osg_board_Timer_getType(self);
#endif
}
