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
#include "../../include/osg/drivers/Gpio.h"
#include "../../include/osg/utils.h"
#include "../../../board/include/board.h"

osg_GpioPort osg_Gpio_getPort(const osg_GpioId id)
{
    if (id == OSG_GPIO_NONE) return OSG_GPIO_PORT_NONE;
    return (osg_GpioPort) ((id - 1) / 32 + 1);
}

osg_GpioPin osg_Gpio_getPin(const osg_GpioId id)
{
    if (id == OSG_GPIO_NONE) return OSG_GPIO_PIN_NONE;
    return (osg_GpioPin) ((id - 1) % 32 + 1);
}

osg_GpioId osg_Gpio_getId(const osg_GpioPort port, const osg_GpioPin pin)
{
    if (port == OSG_GPIO_PORT_NONE || pin == OSG_GPIO_PIN_NONE) return OSG_GPIO_NONE;
    osg_GpioId id = (osg_GpioId) ((port - 1) * 32 + pin - 1 + 1);

    osg_assert(osg_board_Gpio_checkPin(id), "ASSERT FAILED: Pin unavailable on this board.");

    return id;
}

void osg_Gpio_ctor(osg_Gpio * self, const osg_GpioConfig * const config)
{
    osg_assert(osg_board_Gpio_checkPin(config->id), "ASSERT FAILED: Pin unavailable on this board.");
    self->id = config->id;
    self->altFunc = config->alternate;
    osg_board_Gpio_ctor(self, config);
    osg_Gpio_write(self, false);
}

void osg_Gpio_dtor(osg_Gpio * self)
{
    if (self == NULL)
        return;

    osg_board_Gpio_dtor(self);
}

void * osg_Gpio_getHandler(osg_Gpio * self)
{
    return self->handler;
}

bool osg_Gpio_read(osg_Gpio * self)
{
    return osg_board_Gpio_read(self);
}

void osg_Gpio_write(osg_Gpio * self, const bool value)
{
    osg_board_Gpio_write(self, value);
}

void osg_Gpio_toggle(osg_Gpio * self)
{
    osg_board_Gpio_toggle(self);
}

bool osg_Gpio_lock(osg_Gpio * self)
{
    return osg_board_Gpio_lock(self);
}

void osg_Gpio_setExtInterruptCallback(
    const osg_GpioId id,
    osg_GpioInterruptCallback callback,
    const osg_IrqPriority preemptionPriority,
    const osg_IrqPriority subPriority)
{
    osg_board_Gpio_setExtInterruptCallback(id, callback, preemptionPriority, subPriority);
}
