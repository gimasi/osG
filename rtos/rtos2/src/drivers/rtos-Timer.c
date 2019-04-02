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
#include "../../../include/rtos/drivers/rtos-Timer.h"
#include "../../include/rtos/rtos-config.h"
#include "../../../../osg/include/osg.h"

static osTimerAttr_t _oneShot = {
    .name = "0-OSG_TIMER"
};
static osTimerAttr_t _periodic = {
    .name = "1-OSG_TIMER"
};

static osTimerType_t _osg_rtos_timer_getTimerType(const osg_TimerType osgType)
{
    switch (osgType)
    {
        case OSG_TIMER_ONE_SHOT: return osTimerOnce;
        case OSG_TIMER_PERIODIC: return osTimerPeriodic;
        default:
            osg_error("ERROR: Invalid osG Timer type.");
    }

    // suppress warning
    return osTimerOnce;
}

void osg_rtos_Timer_ctor(osg_Timer * self, const osg_TimerConfig * const config)
{
    if (self == NULL)
        return;

    osTimerAttr_t * attr;
    if (config->type == OSG_TIMER_ONE_SHOT)
    {
        attr = &_oneShot;
    }
    else
    {
        attr = &_periodic;
    }
    self->handler = osTimerNew(config->callback, _osg_rtos_timer_getTimerType(config->type), config->argument, attr);
    osg_assert(self->handler != NULL, "ERROR: Cannot initialize timer.");
}

void osg_rtos_Timer_dtor(osg_Timer * self)
{
    if (self == NULL || self-> handler == NULL)
        return;

    osTimerDelete((osTimerId_t)self->handler);
    self->handler = NULL;
}

bool osg_rtos_Timer_start(osg_Timer * self, const uint32_t millis)
{
    if (self == NULL || self-> handler == NULL)
        return false;

    return osg_bool(osTimerStart((osTimerId_t)self->handler, millis) == osOK);
}

bool osg_rtos_Timer_stop(osg_Timer * self)
{
    if (self == NULL || self-> handler == NULL)
        return false;

    return osg_bool(osTimerStop((osTimerId_t)self->handler) == osOK);
}

bool osg_rtos_Timer_isRunning(osg_Timer * self)
{
    if (self == NULL || self-> handler == NULL)
        return false;

    return osg_bool(osTimerIsRunning((osTimerId_t)self->handler) == 1);
}

osg_TimerType osg_rtos_Timer_getType(osg_Timer * self)
{
    if (osTimerGetName((osTimerId_t)self->handler)[0] == '0')
        return OSG_TIMER_ONE_SHOT;
    else
        return OSG_TIMER_PERIODIC;
}
