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
#ifndef OSG_RTOS_FORWARDS_H
#define OSG_RTOS_FORWARDS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// /////////////////////////////////////////////////////////////////////////////
// Data types
// /////////////////////////////////////////////////////////////////////////////
enum osg_IrqPriority;
typedef enum osg_IrqPriority osg_IrqPriority;

enum osg_PinMax;
typedef enum osg_PinMax osg_PinMax;

// /////////////////////////////////////////////////////////////////////////////
// BaseHw
// /////////////////////////////////////////////////////////////////////////////
typedef void (*osg_BaseHwApplication)(void);

struct osg_BaseHwConfig;
typedef struct osg_BaseHwConfig osg_BaseHwConfig;

// /////////////////////////////////////////////////////////////////////////////
// Message Queue
// /////////////////////////////////////////////////////////////////////////////
#if OSG_OS_USE_MESSAGES == 1
enum osg_MessageQueueAttributes;
typedef enum osg_MessageQueueAttributes osg_MessageQueueAttributes;

enum osg_MessageQueueStatus;
typedef enum osg_MessageQueueStatus osg_MessageQueueStatus;

struct osg_MessageQueueConfig;
typedef struct osg_MessageQueueConfig osg_MessageQueueConfig;

struct osg_MessageQueue;
typedef struct osg_MessageQueue osg_MessageQueue;
#endif

// /////////////////////////////////////////////////////////////////////////////
// Mutex
// /////////////////////////////////////////////////////////////////////////////
#if OSG_OS_USE_MUTEX == 1
enum osg_MutexAttributes;
typedef enum osg_MutexAttributes osg_MutexAttributes;

enum osg_MutexStatus;
typedef enum osg_MutexStatus osg_MutexStatus;

struct osg_MutexConfig;
typedef struct osg_MutexConfig osg_MutexConfig;

struct osg_Mutex;
typedef struct osg_Mutex osg_Mutex;
#endif

// /////////////////////////////////////////////////////////////////////////////
// Thread
// /////////////////////////////////////////////////////////////////////////////
#if OSG_OS_USE_THREAD == 1
enum osg_ThreadState;
typedef enum osg_ThreadState osg_ThreadState;

struct osg_ThreadAttributes;
typedef struct osg_ThreadAttributes osg_ThreadAttributes;

struct osg_ThreadConfig;
typedef struct osg_ThreadConfig osg_ThreadConfig;

struct osg_Thread;
typedef struct osg_Thread osg_Thread;

typedef void (*osg_ThreadFunction)(void *);
#endif

// /////////////////////////////////////////////////////////////////////////////
// Timer
// /////////////////////////////////////////////////////////////////////////////
#if OSG_OS_USE_TIMER == 1
struct osg_TimerConfig;
typedef struct osg_TimerConfig osg_TimerConfig;

struct osg_Timer;
typedef struct osg_Timer osg_Timer;

enum osg_TimerType;
typedef enum osg_TimerType osg_TimerType;

typedef void (*osg_TimerCallback)(void *);
#endif

#ifdef __cplusplus
}
#endif

#endif
