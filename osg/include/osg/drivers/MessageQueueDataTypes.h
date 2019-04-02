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
#ifndef OSG_DRIVERS_MESSAGEQUEUEDATATYPES_H
#define OSG_DRIVERS_MESSAGEQUEUEDATATYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum osg_MessageQueueAttributes;
typedef enum osg_MessageQueueAttributes osg_MessageQueueAttributes;
/// @brief The possible MessageQueue attributes
/// @ingroup MessageQueue
/// @memberof osg_MessageQueue
enum osg_MessageQueueAttributes
{
    OSG_MESSAGEQUEUE_NO_ATTRIBUTES      = 0x00,
};

enum osg_MessageQueueStatus;
typedef enum osg_MessageQueueStatus osg_MessageQueueStatus;
/// @brief The possible MessageQueue status
/// @ingroup MessageQueue
/// @memberof osg_MessageQueue
enum osg_MessageQueueStatus
{
    OSG_MESSAGEQUEUE_STATUS_UNDEFINED    = 0x00,
};

struct osg_MessageQueueConfig;
typedef struct osg_MessageQueueConfig osg_MessageQueueConfig;
/// @brief The MessageQueue configuration.
/// @ingroup MessageQueue
/// @memberof osg_MessageQueue
struct osg_MessageQueueConfig
{
    /// @brief Max number of messages in the queue at the same time.
    uint32_t maxNumberOfMessages;
    /// @brief Size of message.
    uint32_t sizeOfSingleMessage;
    /// @brief Optional name for the MessageQueue.
    char * name;
};

struct osg_MessageQueue;
typedef struct osg_MessageQueue osg_MessageQueue;
/// @brief The MessageQueue class.
/// @ingroup MessageQueue
/// @memberof osg_MessageQueue
struct osg_MessageQueue
{
    /// @brief Low-level handler
    /// @private
    void * handler;
    osg_MessageQueueStatus status;
};

#ifdef __cplusplus
}
#endif

#endif
