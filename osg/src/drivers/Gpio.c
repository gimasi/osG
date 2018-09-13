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
#include "osg/drivers/Gpio.h"
#include "osg/utils.h"
#include <board.h>

osg_GpioPort osg_gpio_getPort(const osg_GpioId id)
{
    return (osg_GpioPort) (id / 32);
}

osg_GpioPin osg_gpio_getPin(const osg_GpioId id)
{
    return (osg_GpioPin) (id % 32);
}

osg_GpioId osg_gpio_getId(const osg_GpioPort port, const osg_GpioPin pin)
{
    osg_GpioId id = (osg_GpioId) (port * 32 + pin);

    osg_assert(osg_board_gpio_checkPin(id), "ASSERT FAILED: Pin unavailable on this board.");

    return id;
}

void osg_Gpio_ctor(
    osg_Gpio * self,
    const osg_GpioId id,
    const osg_GpioMode mode,
    const osg_GpioPull pull,
    const osg_GpioSpeed speed,
    const osg_GpioAlternateFunction alternate
    )
{
    osg_assert(osg_board_gpio_checkPin(id), "ASSERT FAILED: Pin unavailable on this board.");
    self->id = id;
    osg_board_gpio_ctor(self, id, mode, pull, speed, alternate);
    osg_Gpio_write(self, FALSE);
}

void osg_Gpio_dtor(osg_Gpio * self)
{
    osg_board_gpio_dtor(self);
}

void * osg_Gpio_getHandler(osg_Gpio * self)
{
    return self->handler;
}

Bool osg_Gpio_read(osg_Gpio * self)
{
    return osg_board_gpio_read(self);
}

void osg_Gpio_write(osg_Gpio * self, const Bool value)
{
    osg_board_gpio_write(self, value);
}

void osg_Gpio_toggle(osg_Gpio * self)
{
    osg_board_gpio_toggle(self);
}

Bool osg_Gpio_lock(osg_Gpio * self)
{
    return osg_board_gpio_lock(self);
}

void osg_Gpio_setExtInterruptCallback(
    const osg_GpioId id,
    osg_GpioInterruptCallback callback,
    const osg_IrqPriority preemptionPriority,
    const osg_IrqPriority subPriority)
{
    osg_board_gpio_setExtInterruptCallback(id, callback, preemptionPriority, subPriority);
}
