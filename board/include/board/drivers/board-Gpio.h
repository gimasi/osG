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
#ifndef OSG_BOARD_DRIVERS_GPIO_H
#define OSG_BOARD_DRIVERS_GPIO_H

#include "../board-forwards.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Board-Gpio Board-Gpio
/// @ingroup Board-Drivers
/// Board methods for GPIO

/// @brief Pin constructor.
/// @ingroup Board-Gpio
/// @param self The object to initialize
/// @param id The pin id
/// @param mode The pin mode
/// @param pull The pin pull mode
/// @param speed The pin speed
/// @param alternate The pin alternate function
void osg_board_gpio_ctor(osg_Gpio * self, const osg_GpioId id, const osg_GpioMode mode, const osg_GpioPull pull, const osg_GpioSpeed speed, const osg_GpioAlternateFunction alternate);
/// @brief The pin destructor
/// @ingroup Board-Gpio
/// @param self The pin object
void osg_board_gpio_dtor(osg_Gpio * self);
/// @brief Gets the maximum allowed number of pins per port
/// @ingroup Board-Gpio
/// @return The pins per port
osg_PinMax osg_board_gpio_getMaxPinPerPort(void);
/// @brief CHecks whether the given PIN is valid
/// @ingroup Board-Gpio
/// @param id The pin id
/// @return True if valid
Bool osg_board_gpio_checkPin(const osg_GpioId id);
/// @brief Reads the pin value
/// @ingroup Board-Gpio
/// @param self The pin object
/// @return The pin value as boolean
Bool osg_board_gpio_read(osg_Gpio * self);
/// @brief Write the pin value
/// @ingroup Board-Gpio
/// @param self The pin object
/// @param value The value to set as boolean
void osg_board_gpio_write(osg_Gpio * self, const Bool value);
/// @brief Toggles the pin value
/// @ingroup Board-Gpio
/// @param self The pin object
void osg_board_gpio_toggle(osg_Gpio * self);
/// @brief Locks the pin.
/// @ingroup Board-Gpio
/// @param self The pin object
/// @return True on success
Bool osg_board_gpio_lock(osg_Gpio * self);
/// @brief Sets the interrupt callback for EXTI
/// @ingroup Board-Gpio
/// @param id The pin id
/// @param callback The callback
/// @param preemptionPriority The main priority to dispatch the callback
/// @param subPriority The secondary priority to dispatch the callback
void osg_board_gpio_setExtInterruptCallback(const osg_GpioId id, osg_GpioInterruptCallback callback, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority);
/// @brief Enables the use of EXTI interrupt
/// @ingroup Board-Gpio
/// @param id The pin id
/// @param preemptionPriority The main priority to dispatch the callback
/// @param subPriority The secondary priority to dispatch the callback
void osg_board_gpio_enableExtInterrupt(const osg_GpioId id, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority);
/// @brief Disables the use of EXTI interrupt
/// @ingroup Board-Gpio
/// @param id The pin id
void osg_board_gpio_disableExtInterrupt(const osg_GpioId id);

#ifdef __cplusplus
}
#endif

#endif
