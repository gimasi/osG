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
#include "../../../include/rtos/drivers/rtos-MessageQueue.h"
#include "../../include/rtos/rtos-config.h"
#include "../../../../osg/include/osg.h"
#include <string.h>

typedef osMessageQueueId_t osg_rtos_MessageQueue;

void osg_rtos_MessageQueue_ctor(osg_MessageQueue * self, const osg_MessageQueueConfig * const config)
{
    if (self == NULL)
        return;
    
    // validate attributes
    osg_assert(config->maxNumberOfMessages >= 1, "ERROR: Invalid max number of messages in queue.");
    osg_assert(config->sizeOfSingleMessage > 0, "ERROR: Invalid message size.");

    osMessageQueueAttr_t mqAttr;
    memset(&mqAttr, 0, sizeof(mqAttr));
    mqAttr.name = config->name;
    self->handler = osMessageQueueNew(config->maxNumberOfMessages, config->sizeOfSingleMessage, &mqAttr);
    
    osg_assert(self->handler != NULL, "ERROR: Cannot initialize Message queue.");
}

void osg_rtos_MessageQueue_dtor(osg_MessageQueue * self)
{
    if (self == NULL || self->handler == NULL)
        return;

    osMessageQueueDelete((osg_rtos_MessageQueue)self->handler);
    self->handler = NULL;
}

bool osg_rtos_MessageQueue_insertMessage(osg_MessageQueue * self, const void * message, const uint32_t timeout)
{
    return osg_bool(osMessageQueuePut((osg_rtos_MessageQueue)self->handler, message, NULL, timeout) == osOK);
}

bool osg_rtos_MessageQueue_getMessage(osg_MessageQueue * self, void * message, const uint32_t timeout)
{
    return osg_bool(osMessageQueueGet((osg_rtos_MessageQueue)self->handler, message, NULL, timeout) == osOK);
}

uint32_t osg_rtos_MessageQueue_getCapacity(osg_MessageQueue * self)
{
    return osMessageQueueGetCapacity((osg_rtos_MessageQueue)self->handler);
}

uint32_t osg_rtos_MessageQueue_getMessageSize(osg_MessageQueue * self)
{
    return osMessageQueueGetMsgSize((osg_rtos_MessageQueue)self->handler);
}

uint32_t osg_rtos_MessageQueue_getCount(osg_MessageQueue * self)
{
    return osMessageQueueGetCount((osg_rtos_MessageQueue)self->handler);
}

uint32_t osg_rtos_MessageQueue_getFreeSlots(osg_MessageQueue * self)
{
    return osMessageQueueGetSpace((osg_rtos_MessageQueue)self->handler);
}

bool osg_rtos_MessageQueue_reset(osg_MessageQueue * self)
{
    return osg_bool(osMessageQueueReset((osg_rtos_MessageQueue)self->handler) == osOK);
}

const char * osg_rtos_MessageQueue_getName(osg_MessageQueue * self)
{
    return osMessageQueueGetName((osg_rtos_MessageQueue)self->handler);
}
