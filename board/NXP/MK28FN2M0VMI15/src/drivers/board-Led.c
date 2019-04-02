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
#include "../../../../include/board/drivers/board-Led.h"
#include "../../../../../osg/include/osg.h"

osg_GpioId osg_board_Led_getGpioId(const osg_LedId id)
{
    /// @todo
    switch (id)
    {
        case OSG_LED_RED:
            return osg_Gpio_getId(OSG_GPIO_PORT_E, OSG_GPIO_PIN_6);
        case OSG_LED_GREEN:
            return osg_Gpio_getId(OSG_GPIO_PORT_E, OSG_GPIO_PIN_7);
        case OSG_LED_BLUE:
            return osg_Gpio_getId(OSG_GPIO_PORT_E, OSG_GPIO_PIN_8);
        default:
            osg_error("ERROR: Led not supported on this board.");
            break;
    }

    // never reached
    return OSG_GPIO_PA0;
}
