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
#include "osg/drivers/Led.h"
#include "osg/drivers/Gpio.h"
#include "osg/drivers/Timer.h"
#include "osg/utils.h"
#include <board.h>

static void timersCallback (void *argument);

void osg_Led_ctor(osg_Led * self, const osg_LedId id)
{
    osg_Gpio_ctor(&self->gpio,
        osg_board_led_getGpioId(id),
        OSG_GPIO_MODE_OUTPUT_PP,
        OSG_GPIO_PULLUP,
        OSG_GPIO_SPEED_FREQ_VERY_HIGH,
        OSG_GPIO_ALTERNATE_DEFAULT);

    osg_Timer_ctor(&self->timer,
        OSG_TIMER_PERIODIC,
        timersCallback,
        (void *)self);
}

void osg_Led_customCtor(osg_Led * self, const osg_GpioId id)
{
    osg_Gpio_ctor(&self->gpio,
        id,
        OSG_GPIO_MODE_OUTPUT_PP,
        OSG_GPIO_PULLUP,
        OSG_GPIO_SPEED_FREQ_VERY_HIGH,
        OSG_GPIO_ALTERNATE_DEFAULT);

    osg_Timer_ctor(&self->timer,
        OSG_TIMER_PERIODIC,
        timersCallback,
        (void *)self);
}

void osg_Led_dtor(osg_Led * self)
{
    osg_Gpio_dtor(&self->gpio);
    osg_Timer_dtor(&self->timer);
}

void osg_Led_turnOn(osg_Led * self)
{
    osg_Gpio_write(&self->gpio, TRUE);
}

void osg_Led_turnOff(osg_Led * self)
{
    osg_Gpio_write(&self->gpio, FALSE);
}

void osg_Led_toggle(osg_Led * self)
{
    osg_Gpio_toggle(&self->gpio);
}

Bool osg_Led_startBlinking(osg_Led * self, uint32_t millis)
{
    return osg_Timer_start(&self->timer, millis);
}

Bool osg_Led_stopBlinking(osg_Led * self)
{
    return osg_Timer_stop(&self->timer);
}

osg_Gpio * osg_Led_toGpio(osg_Led * self)
{
    return &self->gpio;
}

osg_GpioId osg_led_getGpioId(const osg_LedId id)
{
    return osg_board_led_getGpioId(id);
}

static void timersCallback (void *argument)
{
    osg_Led * arg = (osg_Led *)argument;
    osg_assert((arg != NULL), "ERROR: Invalid argument");
    osg_Led_toggle(arg);
}
