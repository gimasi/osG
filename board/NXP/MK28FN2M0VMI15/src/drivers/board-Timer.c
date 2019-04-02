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
#include "../../../../include/board/drivers/board-Timer.h"
#include "board-TimerSupport.h"
#include "../../../../../osg/include/osg.h"
#include "../../include/board/board-config.h"

void osg_board_Timer_ctor(osg_Timer * self, const osg_TimerConfig * const config)
{
    // Get next timer index from array
    uint8_t nextTimerIndex = _osg_board_Timer_getNextTimerIndex();
    osg_assert((nextTimerIndex < _osg_board_Timer_getTimers()), "ERROR: No timers available");
    osg_assert((_osg_board_Timer_checkIndex(nextTimerIndex) == true), "ERROR: Timer not exists on this device.");
    // Retrieve the correct board's timer
    osg_board_TimerInstance * tim = _osg_board_Timer_getTimerInstance(nextTimerIndex);
    osg_assert((tim != NULL), "ERROR: Fails to retrieve timer.");
    osg_board_TimerChannel chan = _osg_board_Timer_getChannel(nextTimerIndex);
    // Enable the correct peripheral clock
    const bool enabled = _osg_board_Timer_init(tim, chan);
    osg_assert((enabled == true), "ERROR: Fails to init timer.");

    osg_board_Timer * timer = _osg_board_Timer_getTimerByIndex(nextTimerIndex);
    timer->tim_handler = tim;
    timer->tim_channel = chan;
    timer->tim_type = config->type;
    timer->tim_callback = config->callback;
    timer->tim_argument = config->argument;
    timer->flags |= OSG_TIMER_FLAG_IN_USE;
    self->handler = &timer->tim_handler;
}

void osg_board_Timer_dtor(osg_Timer * self)
{
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    timer->tim_type = OSG_TIMER_ONE_SHOT;
    timer->tim_callback = NULL;
    timer->tim_argument = NULL;

    uint8_t index = _osg_board_Timer_getTimerIndex(timer->tim_handler, timer->tim_channel);
    // WARNING: this function disable all channels
    if (_osg_board_Timer_pitHasChannelInUse(index) == false)
        PIT_Deinit(timer->tim_handler);

    timer->flags = OSG_TIMER_FLAG_EXISTS;
    timer->tim_handler = NULL;
}

bool osg_board_Timer_start(osg_Timer * self, const uint32_t millis)
{
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    osg_assert(millis > 0, "ERROR: millis <= 0");
    PIT_SetTimerPeriod(timer->tim_handler,
                       timer->tim_channel,
                       USEC_TO_COUNT((millis*1000U),
                       (CLOCK_GetFreq(kCLOCK_BusClk))));

    PIT_StartTimer(timer->tim_handler, timer->tim_channel);
    timer->flags |= OSG_TIMER_FLAG_IS_RUNNING;
    return true;
}

bool osg_board_Timer_stop(osg_Timer * self)
{
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    PIT_StopTimer(timer->tim_handler, timer->tim_channel);
    timer->flags &= ~OSG_TIMER_FLAG_IS_RUNNING;
    return true;
}

bool osg_board_Timer_isRunning(osg_Timer * self)
{
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);

    return osg_bool(timer->flags & OSG_TIMER_FLAG_IS_RUNNING);
}

osg_TimerType osg_board_Timer_getType(osg_Timer * self)
{
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    return timer->tim_type;
}
