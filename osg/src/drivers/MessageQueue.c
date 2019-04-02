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
#include "../../include/osg/drivers/MessageQueue.h"
#include "../../include/osg/utils.h"

#ifdef OSG_HAS_CONFIG_FILE
    #include <osgConfig.h>
#else
    #include "../../../templates/osgConfig.h"
#endif

#if !(OSG_OS_NAME == OSG_OS_NONE)
#include "../../../rtos/include/rtos.h"
#endif

void osg_MessageQueue_ctor(osg_MessageQueue * self, const osg_MessageQueueConfig * const config)
{
#if !(OSG_OS_NAME == OSG_OS_NONE)
    osg_rtos_MessageQueue_ctor(self, config);
#endif
}

void osg_MessageQueue_dtor(osg_MessageQueue * self)
{
#if !(OSG_OS_NAME == OSG_OS_NONE)
    osg_rtos_MessageQueue_dtor(self);
#endif
}

bool osg_MessageQueue_insertMessage(osg_MessageQueue * self, const void * message, const uint32_t timeout)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_MessageQueue_insertMessage(self, message, timeout);
#endif
}

bool osg_MessageQueue_getMessage(osg_MessageQueue * self, void * message, const uint32_t timeout)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_MessageQueue_getMessage(self, message, timeout);
#endif
}

uint32_t osg_MessageQueue_getCapacity(osg_MessageQueue * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return 0;
#else
    return osg_rtos_MessageQueue_getCapacity(self);
#endif
}

uint32_t osg_MessageQueue_getMessageSize(osg_MessageQueue * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return 0;
#else
    return osg_rtos_MessageQueue_getMessageSize(self);
#endif
}

uint32_t osg_MessageQueue_getCount(osg_MessageQueue * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return 0;
#else
    return osg_rtos_MessageQueue_getCount(self);
#endif
}

uint32_t osg_MessageQueue_getFreeSlots(osg_MessageQueue * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return 0;
#else
    return osg_rtos_MessageQueue_getFreeSlots(self);
#endif
}

bool osg_MessageQueue_reset(osg_MessageQueue * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_MessageQueue_reset(self);
#endif
}

const char * osg_MessageQueue_getName(osg_MessageQueue * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return NULL;
#else
    return osg_rtos_MessageQueue_getName(self);
#endif
}
