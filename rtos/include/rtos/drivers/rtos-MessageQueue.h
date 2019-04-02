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
#ifndef OSG_RTOS_DRIVERS_MESSAGEQUEUE_H
#define OSG_RTOS_DRIVERS_MESSAGEQUEUE_H

#include "../rtos-forwards.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup RTOS-MessageQueue RTOS-MessageQueue
/// @ingroup RTOS-Drivers
/// RTOS methods for MessageQueue

/// @brief Constructor for MessageQueue.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @param config MessageQueue's config
void osg_rtos_MessageQueue_ctor(osg_MessageQueue * self, const osg_MessageQueueConfig * const config);

/// @brief Destructor.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
void osg_rtos_MessageQueue_dtor(osg_MessageQueue * self);

/// @brief Put a Message into the queue or timeout if queue is full.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @param message Pointer to message to put into the queue.
/// @param timeout Timeout (0 in case of no time-out).
/// @return false in case of error.
bool osg_rtos_MessageQueue_insertMessage(osg_MessageQueue * self, const void * message, const uint32_t timeout);

/// @brief Get a Message from the queue or timeout if queue is empty.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @param message Pointer to message to get from the queue.
/// @param timeout Timeout (0 in case of no time-out).
/// @return false in case of error.
bool osg_rtos_MessageQueue_getMessage(osg_MessageQueue * self, void * message, const uint32_t timeout);

/// @brief Returns the maximum number of messages in the queue.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @return The maximum number of messages.
uint32_t osg_rtos_MessageQueue_getCapacity(osg_MessageQueue * self);

/// @brief Returns the maximum size of a single message in bytes.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @return maximum message size in bytes.
uint32_t osg_rtos_MessageQueue_getMessageSize(osg_MessageQueue * self);

/// @brief Returns the number of queued messages in the queue.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @return number of queued messages.
uint32_t osg_rtos_MessageQueue_getCount(osg_MessageQueue * self);

/// @brief Returns the number of available slots for messages in the queue.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @return number of available slots for messages.
uint32_t osg_rtos_MessageQueue_getFreeSlots(osg_MessageQueue * self);

/// @brief Resets the message queue.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @return false in case of error.
bool osg_rtos_MessageQueue_reset(osg_MessageQueue * self);

/// @brief returns name of queue.
/// @ingroup RTOS-MessageQueue
/// @param self The MessageQueue object.
/// @return name as null-terminated string.
const char * osg_rtos_MessageQueue_getName(osg_MessageQueue * self);

#ifdef __cplusplus
}
#endif

#endif
