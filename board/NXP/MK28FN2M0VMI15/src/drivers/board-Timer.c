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
#include <osg.h>
#include "board-config.h"
#include <fsl_pit.h>

typedef pit_chnl_t osg_board_TimerId;
typedef PIT_Type osg_board_TimerInstance;

static const int pitIrqs[][4] = PIT_IRQS;

struct osg_board_Timer;
typedef struct osg_board_Timer osg_board_Timer;
struct osg_board_Timer
{
    osg_board_TimerInstance * tim_handler;
    osg_board_TimerId tim_channel;
    osg_TimerType tim_type;
    Bool tim_inUse;
    Bool tim_exists;
    osg_TimerCallback tim_callback;
    void * tim_argument;
};

#define TOT_TIM ((FSL_FEATURE_SOC_PIT_COUNT) * (FSL_FEATURE_PIT_TIMER_COUNT))

static osg_board_Timer _osg_board_timer_timers[TOT_TIM];

uint8_t _osg_board_timer_getTimers()
{
    static uint8_t res = 0;
    if (res == 0)
    {
        for (size_t i = 0; i < FSL_FEATURE_SOC_PIT_COUNT; ++i)
        {
            for (size_t j = 0; j < FSL_FEATURE_PIT_TIMER_COUNT; ++j)
            {
                if (pitIrqs[i][j] == 0)
                {
                    _osg_board_timer_timers[i + j].tim_exists = FALSE;
                }
                else
                {
                    _osg_board_timer_timers[i + j].tim_exists = TRUE;
                    _osg_board_timer_timers[i + j].tim_inUse = FALSE;
                    res += 1;
                }
            }
        }
    }

    return res;
}

uint8_t _osg_board_timer_getNextTimerIndex()
{
    uint8_t max = _osg_board_timer_getTimers();
    for (uint8_t index=0; index<max; index++)
    {
        if (_osg_board_timer_timers[index].tim_exists == TRUE
            && _osg_board_timer_timers[index].tim_inUse == FALSE)
            return index;
    }

    return 0xFF;
}

Bool _osg_board_timer_checkIndex(const uint8_t index)
{
    return _osg_board_timer_timers[index].tim_exists;
}

osg_board_TimerInstance * _osg_board_timer_getTimer(const uint8_t index)
{
    uint8_t pitIndex = index / FSL_FEATURE_PIT_TIMER_COUNT;
    uint8_t targetTimer = 0;
#ifdef PIT0
    if (targetTimer == pitIndex)
        return PIT0;
    targetTimer++;
#endif
#ifdef PIT1
    if (targetTimer == pitIndex)
        return PIT1;
    targetTimer++;
#endif
    return NULL;
}

osg_board_TimerId _osg_board_timer_getChannel(const uint8_t index)
{
    uint8_t chIndex = index % FSL_FEATURE_PIT_TIMER_COUNT;
    switch (chIndex)
    {
        case 0:
            return kPIT_Chnl_0;
        case 1:
            return kPIT_Chnl_1;
        case 2:
            return kPIT_Chnl_2;
        case 3:
            return kPIT_Chnl_3;
        default:
            osg_error("ERROR: Wrong channel conversion.");
    }

    // suppress warning
    return kPIT_Chnl_0;
}

uint8_t _osg_board_timer_getIndex(const osg_board_TimerInstance* timer, const osg_board_TimerId channel)
{
    uint8_t targetTimer = 0;
#ifdef PIT0
    if (timer == PIT0)
    {
        targetTimer += channel;
        return targetTimer;
    }
    targetTimer += FSL_FEATURE_PIT_TIMER_COUNT;
#endif
#ifdef PIT1
    if (timer == PIT1)
    {
        targetTimer += channel;
        return targetTimer;
    }
    targetTimer += 4;
#endif
    return 0xFF;
}

Bool _osg_board_timer_init(const osg_board_TimerInstance * timer, const osg_board_TimerId channel)
{
    pit_config_t pitConfig;
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

#ifdef PIT0
    if (timer == PIT0)
    {
        PIT_Init(PIT0, &pitConfig);
        PIT_EnableInterrupts(PIT0, channel, kPIT_TimerInterruptEnable);
        switch (channel)
        {
            case kPIT_Chnl_0:
#ifdef PIT0_IRQn
            {
                const int32_t enableOk = EnableIRQ(PIT0_IRQn);
                osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT0CH0_IRQn interrupt.");
                break;
            }
#endif
            case kPIT_Chnl_1:
#ifdef PIT1_IRQn
            {
                const int32_t enableOk = EnableIRQ(PIT1_IRQn);
                osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT0CH1_IRQn interrupt.");
                break;
            }
#endif
            case kPIT_Chnl_2:
#ifdef PIT2_IRQn
            {
                const int32_t enableOk = EnableIRQ(PIT2_IRQn);
                osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT0CH2_IRQn interrupt.");
                break;
            }
#endif
            case kPIT_Chnl_3:
#ifdef PIT3_IRQn
            {
                const int32_t enableOk = EnableIRQ(PIT3_IRQn);
                osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT0CH3_IRQn interrupt.");
                break;
            }
#endif
            default:
                osg_error("ERROR: Impossible to initialize timer, wrong channel.");
        }
        const int32_t enableOk = EnableIRQ(PIT0_IRQn);
        osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT0CH0_IRQn interrupt.");
        return TRUE;
    }
#endif
#ifdef PIT1
    else if (timerId == PIT1)
    {
        PIT_Init(PIT1, &pitConfig);
        PIT_EnableInterrupts(PIT1, channel, kPIT_TimerInterruptEnable);
        const int32_t enableOk = EnableIRQ(PIT1CH0_IRQn);
        osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT1CH0_IRQn interrupt.");
        return TRUE;
    }
    else
#endif
        return FALSE;
}

Bool _osg_board_timer_pitHasChannelInUse(const uint8_t timerIndex)
{
    uint8_t startPitIndex = timerIndex - timerIndex % 4;
    Bool ret = FALSE;
    for (int i=startPitIndex; i<startPitIndex+4; ++i)
    {
        if (_osg_board_timer_timers[i].tim_exists == TRUE
            && _osg_board_timer_timers[i].tim_inUse == TRUE)
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

void osg_board_Timer_ctor(osg_Timer * self, const osg_TimerType type, osg_TimerCallback callback, void * argument)
{
    // Get next timer index from array
    uint8_t nextTimerIndex = _osg_board_timer_getNextTimerIndex();
    osg_assert((nextTimerIndex < TOT_TIM), "ERROR: No timers available");
    osg_assert((_osg_board_timer_checkIndex(nextTimerIndex) == TRUE), "ERROR: Timer not exists on this device.");
    // Retrieve the correct board's timer
    osg_board_TimerInstance * timer = _osg_board_timer_getTimer(nextTimerIndex);
    osg_assert((timer != NULL), "ERROR: Fails to retrieve timer.");
    osg_board_TimerId  chan = _osg_board_timer_getChannel(nextTimerIndex);
    // Enable the correct peripheral clock
    const Bool enabled = _osg_board_timer_init(timer, chan);
    osg_assert((enabled == TRUE), "ERROR: Fails to init timer.");

    _osg_board_timer_timers[nextTimerIndex].tim_handler = timer;
    _osg_board_timer_timers[nextTimerIndex].tim_channel = chan;
    _osg_board_timer_timers[nextTimerIndex].tim_type = type;
    _osg_board_timer_timers[nextTimerIndex].tim_inUse = TRUE;
    _osg_board_timer_timers[nextTimerIndex].tim_callback = callback;
    _osg_board_timer_timers[nextTimerIndex].tim_argument = argument;
    self->handler = &_osg_board_timer_timers[nextTimerIndex];
}

void osg_board_Timer_dtor(osg_Timer * self)
{
    uint8_t index = _osg_board_timer_getIndex(((osg_board_Timer *)self->handler)->tim_handler,
                                              ((osg_board_Timer *)self->handler)->tim_channel);
    _osg_board_timer_timers[index].tim_inUse = FALSE;
    _osg_board_timer_timers[index].tim_type = OSG_TIMER_ONE_SHOT;
    _osg_board_timer_timers[index].tim_callback = NULL;
    _osg_board_timer_timers[index].tim_argument = NULL;

    // WARNING: this function disable all channels
    if (_osg_board_timer_pitHasChannelInUse(index) == FALSE)
        PIT_Deinit(((osg_board_Timer *)self->handler)->tim_handler);

    _osg_board_timer_timers[index].tim_handler = NULL;
}

Bool osg_board_Timer_start(osg_Timer * self, const uint32_t millis)
{
    osg_assert(millis > 0, "ERROR: millis <= 0");
    PIT_SetTimerPeriod(((osg_board_Timer *)self->handler)->tim_handler,
                       ((osg_board_Timer *)self->handler)->tim_channel,
                       USEC_TO_COUNT((millis*1000U),
                       (CLOCK_GetFreq(kCLOCK_BusClk))));

    PIT_StartTimer(((osg_board_Timer *)self->handler)->tim_handler,
                   ((osg_board_Timer *)self->handler)->tim_channel);
    return TRUE;
}

Bool osg_board_Timer_stop(osg_Timer * self)
{
    PIT_StopTimer(((osg_board_Timer *)self->handler)->tim_handler,
                  ((osg_board_Timer *)self->handler)->tim_channel);
    return TRUE;
}

Bool osg_board_Timer_isRunning(osg_Timer * self)
{
    uint8_t index = _osg_board_timer_getIndex(((osg_board_Timer *)self->handler)->tim_handler,
                                              ((osg_board_Timer *)self->handler)->tim_channel);
    if (_osg_board_timer_checkIndex(index) == TRUE
        && _osg_board_timer_timers[index].tim_inUse == TRUE)
        return TRUE;

    return FALSE;
}

osg_TimerType osg_board_Timer_getType(osg_Timer * self)
{
    uint8_t index = _osg_board_timer_getIndex(((osg_board_Timer *)self->handler)->tim_handler,
                                              ((osg_board_Timer *)self->handler)->tim_channel);
    return _osg_board_timer_timers[index].tim_type;
}

// @brief  Period elapsed callback in non blocking mode
// @param  htim : TIM handle
// @retval None
void _periodElapsedCallback(uint8_t index)
{
    /* Retrieve the correct index of array structure */
    osg_assert(_osg_board_timer_checkIndex(index), "ERROR: Timer does not exists on this device.");

    if (_osg_board_timer_timers[index].tim_callback != NULL)
    {
        if (_osg_board_timer_timers[index].tim_argument != NULL)
        {
            _osg_board_timer_timers[index].tim_callback(_osg_board_timer_timers[index].tim_argument);
        }
        else
        {
            _osg_board_timer_timers[index].tim_callback(NULL);
        }
    }

    if (_osg_board_timer_timers[index].tim_type == OSG_TIMER_ONE_SHOT)
    {
        PIT_StopTimer(_osg_board_timer_timers[index].tim_handler,
                      _osg_board_timer_timers[index].tim_channel);
    }
}

// @brief  This functions handle TIM interrupt request.
// @param  None
// @retval None
#ifdef PIT0_IRQHandler
void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_0, kPIT_TimerFlag);
    uint8_t index = _osg_board_timer_getIndex(PIT0, kPIT_Chnl_0);
    _periodElapsedCallback(index);
}
#endif
#ifdef PIT1_IRQHandler
void PIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_1, kPIT_TimerFlag);
    uint8_t index = _osg_board_timer_getIndex(PIT0, kPIT_Chnl_1);
    _periodElapsedCallback(index);
}
#endif
#ifdef PIT2_IRQHandler
void PIT2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_2, kPIT_TimerFlag);
    uint8_t index = _osg_board_timer_getIndex(PIT0, kPIT_Chnl_2);
    _periodElapsedCallback(index);
}
#endif
#ifdef PIT3_IRQHandler
void PIT3_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_3, kPIT_TimerFlag);
    uint8_t index = _osg_board_timer_getIndex(PIT0, kPIT_Chnl_3);
    _periodElapsedCallback(index);
}
#endif
