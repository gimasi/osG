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
/// @param config The configuration
void osg_board_Gpio_ctor(osg_Gpio * self, const osg_GpioConfig * const config);

/// @brief The pin destructor
/// @ingroup Board-Gpio
/// @param self The pin object
void osg_board_Gpio_dtor(osg_Gpio * self);

/// @brief Gets the maximum allowed number of pins per port
/// @ingroup Board-Gpio
/// @return The pins per port
osg_PinMax osg_board_Gpio_getMaxPinPerPort(void);

/// @brief CHecks whether the given PIN is valid
/// @ingroup Board-Gpio
/// @param id The pin id
/// @return True if valid
bool osg_board_Gpio_checkPin(const osg_GpioId id);

/// @brief Reads the pin value
/// @ingroup Board-Gpio
/// @param self The pin object
/// @return The pin value as boolean
bool osg_board_Gpio_read(osg_Gpio * self);

/// @brief Write the pin value
/// @ingroup Board-Gpio
/// @param self The pin object
/// @param value The value to set as boolean
void osg_board_Gpio_write(osg_Gpio * self, const bool value);

/// @brief Toggles the pin value
/// @ingroup Board-Gpio
/// @param self The pin object
void osg_board_Gpio_toggle(osg_Gpio * self);

/// @brief Locks the pin.
/// @ingroup Board-Gpio
/// @param self The pin object
/// @return True on success
bool osg_board_Gpio_lock(osg_Gpio * self);

/// @brief Sets the interrupt callback for EXTI
/// @ingroup Board-Gpio
/// @param id The pin id
/// @param callback The callback
/// @param preemptionPriority The main priority to dispatch the callback
/// @param subPriority The secondary priority to dispatch the callback
void osg_board_Gpio_setExtInterruptCallback(const osg_GpioId id, osg_GpioInterruptCallback callback, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority);

/// @brief Enables the use of EXTI interrupt
/// @ingroup Board-Gpio
/// @param id The pin id
/// @param preemptionPriority The main priority to dispatch the callback
/// @param subPriority The secondary priority to dispatch the callback
void osg_board_Gpio_enableExtInterrupt(const osg_GpioId id, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority);

/// @brief Disables the use of EXTI interrupt
/// @ingroup Board-Gpio
/// @param id The pin id
void osg_board_Gpio_disableExtInterrupt(const osg_GpioId id);

#ifdef __cplusplus
}
#endif

#endif
