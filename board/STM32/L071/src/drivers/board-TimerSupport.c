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

#ifdef TIM1
#define COUNT_TIM1 1
#else
#define COUNT_TIM1 0
#endif
#ifdef TIM2
#define COUNT_TIM2 1
#else
#define COUNT_TIM2 0
#endif
#ifdef TIM3
#define COUNT_TIM3 1
#else
#define COUNT_TIM3 0
#endif
#ifdef TIM4
#define COUNT_TIM4 1
#else
#define COUNT_TIM4 0
#endif
#ifdef TIM5
#define COUNT_TIM5 1
#else
#define COUNT_TIM5 0
#endif
#ifdef TIM6
#define COUNT_TIM6 1
#else
#define COUNT_TIM6 0
#endif
#ifdef TIM7
#define COUNT_TIM7 1
#else
#define COUNT_TIM7 0
#endif
#ifdef TIM8
#define COUNT_TIM8 1
#else
#define COUNT_TIM8 0
#endif
#ifdef TIM9
#define COUNT_TIM9 1
#else
#define COUNT_TIM9 0
#endif
#ifdef TIM10
#define COUNT_TIM10 1
#else
#define COUNT_TIM10 0
#endif
#ifdef TIM11
#define COUNT_TIM11 1
#else
#define COUNT_TIM11 0
#endif
#ifdef TIM12
#define COUNT_TIM12 1
#else
#define COUNT_TIM12 0
#endif
#ifdef TIM13
#define COUNT_TIM13 1
#else
#define COUNT_TIM13 0
#endif
#ifdef TIM14
#define COUNT_TIM14 1
#else
#define COUNT_TIM14 0
#endif
#ifdef TIM15
#define COUNT_TIM15 1
#else
#define COUNT_TIM15 0
#endif
#ifdef TIM16
#define COUNT_TIM16 1
#else
#define COUNT_TIM16 0
#endif
#ifdef TIM17
#define COUNT_TIM17 1
#else
#define COUNT_TIM17 0
#endif
#ifdef TIM18
#define COUNT_TIM18 1
#else
#define COUNT_TIM18 0
#endif
#ifdef TIM19
#define COUNT_TIM19 1
#else
#define COUNT_TIM19 0
#endif
#ifdef TIM20
#define COUNT_TIM20 1
#else
#define COUNT_TIM20 0
#endif
#ifdef TIM21
#define COUNT_TIM21 1
#else
#define COUNT_TIM21 0
#endif
#ifdef TIM22
#define COUNT_TIM22 1
#else
#define COUNT_TIM22 0
#endif

#define TOT_TIM (COUNT_TIM1 + COUNT_TIM2 + COUNT_TIM3 + COUNT_TIM4 + COUNT_TIM5 \
                + COUNT_TIM6 + COUNT_TIM7 + COUNT_TIM8 + COUNT_TIM9 + COUNT_TIM10 \
                + COUNT_TIM11 + COUNT_TIM12 + COUNT_TIM13 + COUNT_TIM14 + COUNT_TIM15 \
                + COUNT_TIM16 + COUNT_TIM17 + COUNT_TIM18 + COUNT_TIM19 + COUNT_TIM20 \
                + COUNT_TIM21 + COUNT_TIM22)

static osg_board_Timer _osg_board_Timer_timers[TOT_TIM];


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

uint8_t _osg_board_Timer_getTimers()
{
    return (uint8_t)TOT_TIM;
}

uint8_t _osg_board_Timer_getTimerIndex(const osg_board_TimerInstance * timer)
{
    uint8_t targetTimer = 0;
#ifdef TIM1
    if (timer == TIM1)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM2
    if (timer == TIM2)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM3
    if (timer == TIM3)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM4
    if (timer == TIM4)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM5
    if (timer == TIM5)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM6
    if (timer == TIM6)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM7
    if (timer == TIM7)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM8
    if (timer == TIM8)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM9
    if (timer == TIM9)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM10
    if (timer == TIM10)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM11
    if (timer == TIM11)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM12
    if (timer == TIM12)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM13
    if (timer == TIM13)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM14
    if (timer == TIM14)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM15
    if (timer == TIM15)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM16
    if (timer == TIM16)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM17
    if (timer == TIM17)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM18
    if (timer == TIM18)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM19
    if (timer == TIM19)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM20
    if (timer == TIM20)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM21
    if (timer == TIM21)
        return targetTimer;
    targetTimer++;
#endif
#ifdef TIM22
    if (timer == TIM22)
        return targetTimer;
    targetTimer++;
#endif

    return 0xFF;
}

uint8_t _osg_board_Timer_getNextTimerIndex()
{
    uint8_t max = _osg_board_Timer_getTimers();
    for (uint8_t index=0; index<max; index++)
    {
        if (!(_osg_board_Timer_timers[index].flags & OSG_TIMER_FLAG_IN_USE))
            return index;
    }

    return 0xFF;
}

osg_board_Timer * _osg_board_Timer_getTimerByHandler(void * handler)
{
    for (int i = 0; i < TOT_TIM; ++i)
    {
        if (&_osg_board_Timer_timers[i].tim_handler == handler) return &_osg_board_Timer_timers[i];
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
    uint8_t targetTimer = 0;
#ifdef TIM1
    if (targetTimer == index)
        return TIM1;
    targetTimer++;
#endif
#ifdef TIM2
    if (targetTimer == index)
        return TIM2;
    targetTimer++;
#endif
#ifdef TIM3
    if (targetTimer == index)
        return TIM3;
    targetTimer++;
#endif
#ifdef TIM4
    if (targetTimer == index)
        return TIM4;
    targetTimer++;
#endif
#ifdef TIM5
    if (targetTimer == index)
        return TIM5;
    targetTimer++;
#endif
#ifdef TIM6
    if (targetTimer == index)
        return TIM6;
    targetTimer++;
#endif
#ifdef TIM7
    if (targetTimer == index)
        return TIM7;
    targetTimer++;
#endif
#ifdef TIM8
    if (targetTimer == index)
        return TIM8;
    targetTimer++;
#endif
#ifdef TIM9
    if (targetTimer == index)
        return TIM9;
    targetTimer++;
#endif
#ifdef TIM10
    if (targetTimer == index)
        return TIM10;
    targetTimer++;
#endif
#ifdef TIM11
    if (targetTimer == index)
        return TIM11;
    targetTimer++;
#endif
#ifdef TIM12
    if (targetTimer == index)
        return TIM12;
    targetTimer++;
#endif
#ifdef TIM13
    if (targetTimer == index)
        return TIM13;
    targetTimer++;
#endif
#ifdef TIM14
    if (targetTimer == index)
        return TIM14;
    targetTimer++;
#endif
#ifdef TIM15
    if (targetTimer == index)
        return TIM15;
    targetTimer++;
#endif
#ifdef TIM16
    if (targetTimer == index)
        return TIM16;
    targetTimer++;
#endif
#ifdef TIM17
    if (targetTimer == index)
        return TIM17;
    targetTimer++;
#endif
#ifdef TIM18
    if (targetTimer == index)
        return TIM18;
    targetTimer++;
#endif
#ifdef TIM19
    if (targetTimer == index)
        return TIM19;
    targetTimer++;
#endif
#ifdef TIM20
    if (targetTimer == index)
        return TIM20;
    targetTimer++;
#endif
#ifdef TIM21
    if (targetTimer == index)
        return TIM21;
    targetTimer++;
#endif
#ifdef TIM22
    if (targetTimer == index)
        return TIM22;
    targetTimer++;
#endif

    return NULL;
}

osg_board_TimerHandler * _osg_board_Timer_getTimerHandler(const uint8_t index)
{
    return &_osg_board_Timer_timers[index].tim_handler;
}


// ///////////////////////////////////////////////////////
// ENABLE
// ///////////////////////////////////////////////////////

bool _osg_board_Timer_enable(const osg_board_TimerInstance * timerId)
{
    /// @todo: TIMx_IRQn name NOT standard: can be different for other boards.
#ifdef TIM1
    if (timerId == TIM1)
    {
        __TIM1_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM2
        if (timerId == TIM2)
    {
        __TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM3
        if (timerId == TIM3)
    {
        __TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM4
        if (timerId == TIM4)
    {
        __TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM5
        if (timerId == TIM5)
    {
        __TIM5_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM5_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM6
        if (timerId == TIM6)
    {
        __TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM7
        if (timerId == TIM7)
    {
        __TIM7_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM7_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM8
        if (timerId == TIM8)
    {
        __TIM8_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM8_UP_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM9
        if (timerId == TIM9)
    {
        __TIM9_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM9_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM9_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM10
        if (timerId == TIM10)
    {
        __TIM10_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM10_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM10_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM11
        if (timerId == TIM11)
    {
        __TIM11_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM11_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM11_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM12
        if (timerId == TIM12)
    {
        __TIM12_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM12_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM12_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM13
        if (timerId == TIM13)
    {
        __TIM13_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM13_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM13_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM14
        if (timerId == TIM14)
    {
        __TIM14_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM14_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM14_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM15
        if (timerId == TIM15)
    {
        __TIM15_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM16
        if (timerId == TIM16)
    {
        __TIM16_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM17
        if (timerId == TIM17)
    {
        __TIM17_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM18
        if (timerId == TIM18)
    {
        __TIM18_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM18_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM18_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM19
        if (timerId == TIM19)
    {
        __TIM19_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM19_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM19_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM20
        if (timerId == TIM20)
    {
        __TIM20_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM20_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM20_IRQn);
        return true;
    }
#endif
#ifdef TIM21
        if (timerId == TIM21)
    {
        __TIM21_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM21_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM21_IRQn);
        return true;
    }
    else
#endif
#ifdef TIM22
        if (timerId == TIM22)
    {
        __TIM22_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM22_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM22_IRQn);
        return true;
    }
#endif
        return false;
}


// ///////////////////////////////////////////////////////
// STM INTERNAL CALLBACKS
// ///////////////////////////////////////////////////////

void TIM2_IRQHandler(void)
{
    uint8_t index = _osg_board_Timer_getTimerIndex(TIM2);
    HAL_TIM_IRQHandler(&_osg_board_Timer_timers[index].tim_handler);
}

void TIM3_IRQHandler(void)
{
    uint8_t index = _osg_board_Timer_getTimerIndex(TIM3);
    HAL_TIM_IRQHandler(&_osg_board_Timer_timers[index].tim_handler);
}

void TIM6_IRQHandler(void)
{
    uint8_t index = _osg_board_Timer_getTimerIndex(TIM6);
    HAL_TIM_IRQHandler(&_osg_board_Timer_timers[index].tim_handler);
}

void TIM7_IRQHandler(void)
{
    uint8_t index = _osg_board_Timer_getTimerIndex(TIM7);
    HAL_TIM_IRQHandler(&_osg_board_Timer_timers[index].tim_handler);
}

void TIM21_IRQHandler(void)
{
    uint8_t index = _osg_board_Timer_getTimerIndex(TIM21);
    HAL_TIM_IRQHandler(&_osg_board_Timer_timers[index].tim_handler);
}

void TIM22_IRQHandler(void)
{
    uint8_t index = _osg_board_Timer_getTimerIndex(TIM22);
    HAL_TIM_IRQHandler(&_osg_board_Timer_timers[index].tim_handler);
}

// @brief  Period elapsed callback in non blocking mode
// @param  htim : TIM handle
// @retval None
void HAL_TIM_PeriodElapsedCallback(osg_board_TimerHandler *htim)
{
    /* Retrieve the correct index of array structure */
    uint8_t index = _osg_board_Timer_getTimerIndex(htim->Instance);
    osg_assert(index < _osg_board_Timer_getTimers(), "ERROR: Out of range timer index.");

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
        HAL_TIM_Base_Stop_IT(&_osg_board_Timer_timers[index].tim_handler);
    }
}
