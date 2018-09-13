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

#ifdef __cplusplus
extern "C" {
#endif

// /////////////////////////////////////////////////////////////////////////////
// Data types
// /////////////////////////////////////////////////////////////////////////////

enum Bool;
typedef enum Bool Bool;

enum osg_IrqPriority;
typedef enum osg_IrqPriority osg_IrqPriority;

enum osg_PinMax;
typedef enum osg_PinMax osg_PinMax;

// /////////////////////////////////////////////////////////////////////////////
// BaseHw
// /////////////////////////////////////////////////////////////////////////////

typedef void (*osg_BaseHwApplication)(void);


// /////////////////////////////////////////////////////////////////////////////
// Timer
// /////////////////////////////////////////////////////////////////////////////

struct osg_Timer;
typedef struct osg_Timer osg_Timer;

enum osg_TimerType;
typedef enum osg_TimerType osg_TimerType;

typedef void (*osg_TimerCallback)(void *);

// /////////////////////////////////////////////////////////////////////////////
// Thread
// /////////////////////////////////////////////////////////////////////////////

enum osg_ThreadState;
typedef enum osg_ThreadState osg_ThreadState;

struct osg_Thread;
typedef struct osg_Thread osg_Thread;

typedef void (*osg_ThreadFunction)(void *);


#ifdef __cplusplus
}
#endif

#endif
