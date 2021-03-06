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
#ifndef OSG_BOARD_DRIVERS_TIMERSUPPORT_H
#define OSG_BOARD_DRIVERS_TIMERSUPPORT_H

#include <fsl_pit.h>

#ifdef __cplusplus
extern "C" {
#endif


// ///////////////////////////////////////////////////////
// DATA TYPES DEFINES
// ///////////////////////////////////////////////////////
typedef pit_chnl_t osg_board_TimerChannel;
typedef PIT_Type osg_board_TimerInstance;
typedef PIT_Type osg_board_TimerHandler;
enum osg_board_TimerFlags;
typedef enum osg_board_TimerFlags osg_board_TimerFlags;
enum osg_board_TimerFlags
{
    OSG_TIMER_FLAG_DEFAULT    = 0x00,
    OSG_TIMER_FLAG_EXISTS     = 0x02,
    OSG_TIMER_FLAG_IN_USE     = 0x04,
    OSG_TIMER_FLAG_IS_RUNNING = 0x08
};
struct osg_board_Timer;
typedef struct osg_board_Timer osg_board_Timer;
struct osg_board_Timer
{
    osg_board_TimerHandler * tim_handler;
    osg_board_TimerChannel tim_channel;
    osg_TimerType tim_type;
    osg_board_TimerFlags flags;
    osg_TimerCallback tim_callback;
    void * tim_argument;
};

// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////
uint8_t _osg_board_Timer_getTimers(void);
uint8_t _osg_board_Timer_getTimerIndex(const osg_board_TimerInstance* timer, const osg_board_TimerChannel channel);
uint8_t _osg_board_Timer_getNextTimerIndex(void);
osg_board_Timer * _osg_board_Timer_getTimerByHandler(void * handler);
osg_board_Timer * _osg_board_Timer_getTimerByIndex(const uint8_t index);
osg_board_TimerInstance * _osg_board_Timer_getTimerInstance(const uint8_t index);
osg_board_TimerHandler * _osg_board_Timer_getTimerHandler(const uint8_t index);

// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////
bool _osg_board_Timer_checkIndex(const uint8_t index);
bool _osg_board_Timer_pitHasChannelInUse(const uint8_t timerIndex);

// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////
osg_board_TimerChannel _osg_board_Timer_getChannel(const uint8_t index);

// ///////////////////////////////////////////////////////
// ENABLE
// ///////////////////////////////////////////////////////
bool _osg_board_Timer_init(const osg_board_TimerInstance * timer, const osg_board_TimerChannel channel);

// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////
void _osg_board_Timer_periodElapsedCallback(const uint8_t index);


#ifdef __cplusplus
}
#endif

#endif
