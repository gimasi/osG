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
    /* Get next timer index from array */
    uint8_t nextTimerIndex = _osg_board_Timer_getNextTimerIndex();
    osg_assert((nextTimerIndex < _osg_board_Timer_getTimers()), "ERROR: No timers available");
    /* Retrieve the correct board's timer */
    osg_board_TimerInstance * tim = _osg_board_Timer_getTimerInstance(nextTimerIndex);
    osg_assert((tim != NULL), "ERROR: Fails to retrieve timer.");
    /* Enable the correct peripheral clock */
    const bool enabled = _osg_board_Timer_enable(tim);
    osg_assert((enabled == true), "ERROR: Fails to enable timer clock.");

    self->handler = _osg_board_Timer_getTimerHandler(nextTimerIndex);
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    osg_board_TimerHandler * handler = &timer->tim_handler;
    timer->flags = OSG_TIMER_FLAG_DEFAULT;

    handler->Instance = tim;
    handler->Init.Prescaler = (uint32_t)(SystemCoreClock / 10000) - 1;
    handler->Init.CounterMode = TIM_COUNTERMODE_UP;
    handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handler->Init.RepetitionCounter = 0;
    const HAL_StatusTypeDef init = HAL_TIM_Base_Init(handler);
    osg_assert((init == HAL_OK), "ERROR: Unable to init timer.");

    timer->flags |= OSG_TIMER_FLAG_IN_USE;
    timer->tim_type = config->type;
    timer->tim_callback = config->callback;
    timer->tim_argument = config->argument;
}

void osg_board_Timer_dtor(osg_Timer * self)
{
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    if (timer == NULL)
        return;

    HAL_TIM_Base_DeInit((osg_board_TimerHandler *)self->handler);
    timer->flags = OSG_TIMER_FLAG_DEFAULT;
    timer->tim_type = OSG_TIMER_ONE_SHOT;
    timer->tim_callback = NULL;
    timer->tim_argument = NULL;
    self->handler = NULL;
}

bool osg_board_Timer_start(osg_Timer * self, const uint32_t millis)
{
    osg_assert(millis > 0, "ERROR: millis <= 0");
    osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
    osg_board_TimerHandler * handler = &timer->tim_handler;
    handler->Init.Period = millis * 10 - 1;
    const HAL_StatusTypeDef init = HAL_TIM_Base_Init(handler);
    osg_assert((init == HAL_OK), "ERROR: Unable to re-init timer.");

    if (HAL_TIM_Base_Start_IT(handler) == HAL_OK)
    {
        timer->flags |= OSG_TIMER_FLAG_IS_RUNNING;
        return true;
    }

    return false;
}

bool osg_board_Timer_stop(osg_Timer * self)
{
    if (HAL_TIM_Base_Stop_IT((osg_board_TimerHandler *)self->handler) == HAL_OK)
    {
        osg_board_Timer * timer = _osg_board_Timer_getTimerByHandler(self->handler);
        timer->flags &= ~OSG_TIMER_FLAG_IS_RUNNING;
        return true;
    }

    return false;
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
