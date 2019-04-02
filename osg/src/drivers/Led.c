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
#include <string.h>
#include "../../include/osg/drivers/Led.h"
#include "../../include/osg/drivers/Gpio.h"
#include "../../include/osg/drivers/Timer.h"
#include "../../include/osg/utils.h"
#include "../../../board/include/board.h"

static void timersCallback (void *argument);

static void _osg_Led_initDefaultValues(osg_Led * self)
{
    // reset flag
    self->flags = OSG_LED_FLAG_DEFAULT;
    // set custom/default blinking frequesncy
    self->millis = 500;
    // blinking forever
    self->totalNumberOfBlinks = 0;
    // reset remaining
    self->remainingNumberOfBlinks = self->totalNumberOfBlinks;
}

void osg_Led_ctor(osg_Led * self, const osg_LedConfig * const config)
{
    _osg_Led_initDefaultValues(self);
    osg_GpioId gpioId = config->gpioId;
    if (gpioId == OSG_GPIO_NONE)
    {
        gpioId = osg_board_Led_getGpioId(config->ledId);
    }
    
    osg_GpioConfig gpioConfig;
    memset(&gpioConfig, 0, sizeof(gpioConfig));
    gpioConfig.id = gpioId;
    gpioConfig.mode = OSG_GPIO_MODE_OUTPUT_PP;
    gpioConfig.pull = OSG_GPIO_PULLUP;
    gpioConfig.speed = OSG_GPIO_SPEED_FREQ_VERY_HIGH;
    gpioConfig.alternate = 0;
    osg_Gpio_ctor(&self->gpio, &gpioConfig);
}

void osg_Led_dtor(osg_Led * self)
{
    if (self == NULL)
        return;

    _osg_Led_initDefaultValues(self);
    osg_Timer_dtor(&self->timer);
    osg_Gpio_dtor(&self->gpio);
}

void osg_Led_turnOn(osg_Led * self)
{
    if (osg_Led_isBlinking(self) == true)
        osg_Led_stopBlinking(self);

    osg_Gpio_write(&self->gpio, true);
}

bool osg_Led_isOn(osg_Led * self)
{
    return osg_Gpio_read(&self->gpio);
}

void osg_Led_turnOff(osg_Led * self)
{
    if (osg_Led_isBlinking(self) == true)
        osg_Led_stopBlinking(self);

    osg_Gpio_write(&self->gpio, false);
}

void osg_Led_toggle(osg_Led * self)
{
    osg_Gpio_toggle(&self->gpio);
}

void osg_Led_setBlinkingPeriod(osg_Led * self, uint32_t millis)
{
    osg_assert(millis > 0, "ASSERT FAILED: Invalid value for blinking period. Only value > 0.");
    self->millis = millis;
}

void osg_Led_setNumberOfBlinks(osg_Led * self, uint32_t numberOfBlinks)
{
    self->totalNumberOfBlinks = numberOfBlinks;
}

uint32_t osg_Led_getBlinkingPeriod(osg_Led * self)
{
    return self->millis;
}

uint32_t osg_Led_getNumberOfBlinks(osg_Led * self)
{
    return self->totalNumberOfBlinks;
}

bool osg_Led_toggleBlinking(osg_Led * self)
{
    bool status = osg_Led_isBlinking(self);
    if (status == true)
       return osg_Led_stopBlinking(self);
    else
       return osg_Led_startBlinking(self);
}

bool osg_Led_startBlinking(osg_Led * self)
{
    if (!(self->flags & OSG_LED_FLAG_TIMER_CREATED))
    {
        osg_TimerConfig timerConfig;
        memset(&timerConfig, 0, sizeof(timerConfig));
        timerConfig.type = OSG_TIMER_PERIODIC;
        timerConfig.callback = timersCallback;
        timerConfig.argument = (void *)self;
        osg_Timer_ctor(&self->timer, &timerConfig);
        self->flags |= OSG_LED_FLAG_TIMER_CREATED;
    }

    self->flags |= OSG_LED_FLAG_BLINKING_ENABLED;
    self->remainingNumberOfBlinks = self->totalNumberOfBlinks;
    return osg_Timer_start(&self->timer, self->millis);
}

bool osg_Led_stopBlinking(osg_Led * self)
{
    self->flags &= ~OSG_LED_FLAG_BLINKING_ENABLED;
    bool ret = osg_Timer_stop(&self->timer);
    osg_Led_turnOff(self);
    return ret;
}

bool osg_Led_isBlinking(osg_Led * self)
{
    return osg_bool(self->flags & OSG_LED_FLAG_BLINKING_ENABLED);
}

osg_Gpio * osg_Led_toGpio(osg_Led * self)
{
    return &self->gpio;
}

osg_GpioId osg_Led_getGpioId(const osg_LedId id)
{
    return osg_board_Led_getGpioId(id);
}

static void timersCallback (void *argument)
{
    osg_Led * led = (osg_Led *)argument;
    osg_assert((led != NULL), "ERROR: Invalid argument");
    osg_Led_toggle(led);

    if (led->totalNumberOfBlinks > 0
        && led->remainingNumberOfBlinks == 0)
    {
        osg_Led_stopBlinking(led);
    }

    if (osg_Gpio_read(&led->gpio) == true)
        led->remainingNumberOfBlinks--;
}
