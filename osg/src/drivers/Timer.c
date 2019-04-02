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
#include "../../include/osg/drivers/Timer.h"
#include "../../include/osg/utils.h"
#include "../../../board/include/board.h"

#ifdef OSG_HAS_CONFIG_FILE
    #include <osgConfig.h>
#else
    #include "../../../templates/osgConfig.h"
#endif

#if defined(OSG_OS_NAME) && !(OSG_OS_NAME == OSG_OS_NONE)
#include "../../../rtos/include/rtos.h"
#endif

void osg_Timer_ctor(osg_Timer * self, const osg_TimerConfig * const config)
{
#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
    osg_board_Timer_ctor(self, config);
#else
    osg_rtos_Timer_ctor(self, config);
#endif
}

void osg_Timer_dtor(osg_Timer * self)
{
    if (self == NULL)
        return;

#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
    osg_board_Timer_dtor(self);
#else
    osg_rtos_Timer_dtor(self);
#endif
}

bool osg_Timer_start(osg_Timer * self, const uint32_t millis)
{
#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
    return osg_board_Timer_start(self, millis);
#else
    return osg_rtos_Timer_start(self, millis);
#endif
}

bool osg_Timer_stop(osg_Timer * self)
{
#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
    return osg_board_Timer_stop(self);
#else
    return osg_rtos_Timer_stop(self);
#endif
}

bool osg_Timer_isRunning(osg_Timer * self)
{
#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
    return osg_board_Timer_isRunning(self);
#else
    return osg_rtos_Timer_isRunning(self);
#endif
}

void * osg_Timer_getHandler(osg_Timer * self)
{
    return self->handler;
}

osg_TimerType osg_Timer_getType(osg_Timer * self)
{
#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
    return osg_board_Timer_getType(self);
#else
    return osg_rtos_Timer_getType(self);
#endif
}
