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
#ifndef OSG_DRIVERS_GPIO_H
#define OSG_DRIVERS_GPIO_H

#include "../osgDataTypes.h"
#include "GpioDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup Gpio Gpio
/// @ingroup Drivers
/// Methods for GPIO

/// @brief Convenience typedef for pins interupt callbacks
/// @ingroup Gpio
/// @memberof Gpio
typedef void (*osg_GpioInterruptCallback)(const osg_GpioPin);

/// @brief Pin constructor.
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The object to initialize
/// @param id The pin id
/// @param mode The pin mode
/// @param pull The pin pull mode
/// @param speed The pin speed
/// @param alternate The pin alternate function
void osg_Gpio_ctor(
    osg_Gpio * self,
    const osg_GpioId id,
    const osg_GpioMode mode,
    const osg_GpioPull pull,
    const osg_GpioSpeed speed,
    const osg_GpioAlternateFunction alternate
    );
/// @brief The pin destructor
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The pin object
void osg_Gpio_dtor(osg_Gpio * self);
/// @brief Maps a GPIO ID to its port.
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param id The ID.
/// @return The port.
osg_GpioPort osg_gpio_getPort(const osg_GpioId id);
/// @brief Maps a GPIO ID to its pin.
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param id The ID.
/// @return The port.
osg_GpioPin osg_gpio_getPin(const osg_GpioId id);
/// @brief Calculates the GPIO ID from its port and pin.
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param port The port.
/// @param pin The pin.
/// @return The id.
osg_GpioId osg_gpio_getId(const osg_GpioPort port, const osg_GpioPin pin);
/// @brief Returns the low level pin handler
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The pin object
void * osg_Gpio_getHandler(osg_Gpio * self);
/// @brief Reads the pin value
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The pin object
/// @return The pin value as boolean
Bool osg_Gpio_read(osg_Gpio * self);
/// @brief Write the pin value
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The pin object
/// @param value The value to set as boolean
void osg_Gpio_write(osg_Gpio * self, const Bool value);
/// @brief Toggles the pin value
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The pin object
void osg_Gpio_toggle(osg_Gpio * self);
/// @brief Locks the pin.
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param self The pin object
/// @return True on success
Bool osg_Gpio_lock(osg_Gpio * self);

/// @brief Sets the interrupt callback for EXTI
/// Note that the callback is set for all the matching pin of all ports.
/// Passing NULL as callback will disable the interrupt.
/// Note that pins are also grouped to EXTI interrupts as follows:
/// * EXTI0_IRQn: OSG_GPIO_PIN_0
/// * EXTI1_IRQn: OSG_GPIO_PIN_1
/// * EXTI2_IRQn: OSG_GPIO_PIN_2
/// * EXTI3_IRQn: OSG_GPIO_PIN_3
/// * EXTI4_IRQn: OSG_GPIO_PIN_4
/// * EXTI9_5_IRQn: from OSG_GPIO_PIN_5 to OSG_GPIO_PIN_9
/// * EXTI15_10_IRQn: from OSG_GPIO_PIN_10 to OSG_GPIO_PIN_10
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @param id The pin id
/// @param callback The callback
/// @param preemptionPriority The main priority to dispatch the callback
/// @param subPriority The secondary priority to dispatch the callback
void osg_gpio_setExtInterruptCallback(
    const osg_GpioId id,
    osg_GpioInterruptCallback callback,
    const osg_IrqPriority preemptionPriority,
    const osg_IrqPriority subPriority);

#ifdef __cplusplus
}
#endif

#endif
