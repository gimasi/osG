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
#include "board-TimerSupport.h"

#define TOT_TIM ((FSL_FEATURE_SOC_PIT_COUNT) * (FSL_FEATURE_PIT_TIMER_COUNT))

static const int pitIrqs[][4] = PIT_IRQS;
static osg_board_Timer _osg_board_Timer_timers[TOT_TIM];


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

uint8_t _osg_board_Timer_getTimers()
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
                    _osg_board_Timer_timers[i + j].flags &= OSG_TIMER_FLAG_EXISTS;
                }
                else
                {
                    _osg_board_Timer_timers[i + j].flags |= OSG_TIMER_FLAG_EXISTS;
                    _osg_board_Timer_timers[i + j].flags &= ~OSG_TIMER_FLAG_IN_USE;
                    res += 1;
                }
            }
        }
    }

    return res;
}

uint8_t _osg_board_Timer_getTimerIndex(const osg_board_TimerInstance* timer, const osg_board_TimerChannel channel)
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

uint8_t _osg_board_Timer_getNextTimerIndex()
{
    uint8_t max = _osg_board_Timer_getTimers();
    for (uint8_t index=0; index<max; index++)
    {
        if (_osg_board_Timer_timers[index].flags & OSG_TIMER_FLAG_EXISTS
            && !(_osg_board_Timer_timers[index].flags & OSG_TIMER_FLAG_IN_USE))
            return index;
    }

    return 0xFF;
}

osg_board_Timer * _osg_board_Timer_getTimerByHandler(void * handler)
{
    for (int i = 0; i < TOT_TIM; ++i)
    {
        if (&_osg_board_Timer_timers[i].tim_handler == handler)
            return &_osg_board_Timer_timers[i];
    }

    return NULL;
}

osg_board_Timer * _osg_board_Timer_getTimerByIndex(const uint8_t index)
{
    if (index < TOT_TIM)
        return &_osg_board_Timer_timers[index];
    
    return NULL;
}

osg_board_TimerInstance * _osg_board_Timer_getTimerInstance(const uint8_t index)
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

osg_board_TimerHandler * _osg_board_Timer_getTimerHandler(const uint8_t index)
{
    return _osg_board_Timer_timers[index].tim_handler;
}


// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////

bool _osg_board_Timer_checkIndex(const uint8_t index)
{
    return osg_bool(_osg_board_Timer_timers[index].flags & OSG_TIMER_FLAG_EXISTS);
}

bool _osg_board_Timer_pitHasChannelInUse(const uint8_t timerIndex)
{
    uint8_t startPitIndex = timerIndex - timerIndex % 4;
    bool ret = false;
    for (int i=startPitIndex; i<startPitIndex+4; ++i)
    {
        if (_osg_board_Timer_timers[i].flags == OSG_TIMER_FLAG_EXISTS
            && _osg_board_Timer_timers[i].flags == OSG_TIMER_FLAG_IN_USE)
        {
            ret = true;
            break;
        }
    }

    return ret;
}


// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////

osg_board_TimerChannel _osg_board_Timer_getChannel(const uint8_t index)
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


// ///////////////////////////////////////////////////////
// ENABLE
// ///////////////////////////////////////////////////////

bool _osg_board_Timer_init(const osg_board_TimerInstance * timer, const osg_board_TimerChannel channel)
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
        return true;
    }
#endif
#ifdef PIT1
    else if (timerId == PIT1)
    {
        PIT_Init(PIT1, &pitConfig);
        PIT_EnableInterrupts(PIT1, channel, kPIT_TimerInterruptEnable);
        const int32_t enableOk = EnableIRQ(PIT1CH0_IRQn);
        osg_assert(enableOk == kStatus_Success, "ERROR: Impossible to enable PIT1CH0_IRQn interrupt.");
        return true;
    }
    else
#endif
        return false;
}


// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////

void _osg_board_Timer_periodElapsedCallback(const uint8_t index)
{
    /* Retrieve the correct index of array structure */
    osg_assert(_osg_board_Timer_checkIndex(index), "ERROR: Timer does not exists on this device.");

    if (_osg_board_Timer_timers[index].tim_callback != NULL)
    {
        if (_osg_board_Timer_timers[index].tim_argument != NULL)
        {
            _osg_board_Timer_timers[index].tim_callback(_osg_board_Timer_timers[index].tim_argument);
        }
        else
        {
            _osg_board_Timer_timers[index].tim_callback(NULL);
        }
    }

    if (_osg_board_Timer_timers[index].tim_type == OSG_TIMER_ONE_SHOT)
    {
        PIT_StopTimer(_osg_board_Timer_timers[index].tim_handler,
                      _osg_board_Timer_timers[index].tim_channel);
    }
}


// ///////////////////////////////////////////////////////
// NXP INTERNAL CALLBACKS
// ///////////////////////////////////////////////////////

#ifdef PIT0_IRQHandler
void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_0, kPIT_TimerFlag);
    uint8_t index = _osg_board_Timer_getTimerIndex(PIT0, kPIT_Chnl_0);
    _osg_board_Timer_periodElapsedCallback(index);
}
#endif
#ifdef PIT1_IRQHandler
void PIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_1, kPIT_TimerFlag);
    uint8_t index = _osg_board_Timer_getTimerIndex(PIT0, kPIT_Chnl_1);
    _osg_board_Timer_periodElapsedCallback(index);
}
#endif
#ifdef PIT2_IRQHandler
void PIT2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_2, kPIT_TimerFlag);
    uint8_t index = _osg_board_Timer_getTimerIndex(PIT0, kPIT_Chnl_2);
    _osg_board_Timer_periodElapsedCallback(index);
}
#endif
#ifdef PIT3_IRQHandler
void PIT3_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0, kPIT_Chnl_3, kPIT_TimerFlag);
    uint8_t index = _osg_board_Timer_getTimerIndex(PIT0, kPIT_Chnl_3);
    _osg_board_Timer_periodElapsedCallback(index);
}
#endif
