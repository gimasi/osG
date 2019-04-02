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
#include "../../../../include/board/drivers/board-Gpio.h"
#include "board-GpioSupport.h"
#include "../../../../../osg/include/osg.h"

void osg_board_Gpio_ctor(osg_Gpio * self,
                         const osg_GpioConfig * const config)
{
    const osg_GpioPort osg_port = osg_Gpio_getPort(config->id);
    osg_board_Gpio * gpio = _osg_board_Gpio_getGpioHandler(osg_port);
    osg_board_Port * port = _osg_board_Gpio_getPortHandler(osg_port);
    uint32_t pin = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(config->id));

    self->handler = gpio;

    _osg_board_Gpio_enablePort(osg_port);

    if (config->alternate < kPORT_MuxAlt2)
    {
        /* The GPIO pin configuration structure.
        *
        * Each pin can only be configured as either an output pin or an input pin at a time.
        * If configured as an input pin, leave the outputConfig unused.
        * Note that in some use cases, the corresponding port property should be configured in advance
        *        with the PORT_SetPinConfig().
        */
        gpio_pin_config_t gpioConfig;
        gpioConfig.pinDirection = _osg_board_Gpio_decodeDirection(config->mode);
        gpioConfig.outputLogic = 0;

        PORT_SetPinMux(port, pin, kPORT_MuxAsGpio);
        GPIO_PinInit(gpio, pin, &gpioConfig);
    }
    else
    {
        // PORT pin configuration structure
        port_pin_config_t portConfig;
        portConfig.pullSelect = _osg_board_Gpio_decodePull(config->pull);
        portConfig.slewRate = _osg_board_Gpio_decodeSpeed(config->speed);
        //portConfig.passiveFilterEnable;
        portConfig.openDrainEnable = _osg_board_Gpio_decodeOpenDrain(config->mode);
        //portConfig.driveStrength;
        portConfig.mux = config->alternate;
        portConfig.lockRegister = kPORT_UnlockRegister;

        PORT_SetPinConfig(port, pin, &portConfig);
        if (_osg_board_Gpio_isInterruptMode(config->mode) == true)
            PORT_SetPinInterruptConfig(port, pin, _osg_board_Gpio_decodeInterruptEdge(config->mode));
    }

}

void osg_board_Gpio_dtor(osg_Gpio * self)
{
    osg_board_Port * port = _osg_board_Gpio_getPortHandler(osg_Gpio_getPort(self->id));
    PORT_SetPinMux(port, _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id)), kPORT_PinDisabledOrAnalog);
}

osg_PinMax osg_board_Gpio_getMaxPinPerPort(void)
{
    return OSG_32_PIN_PER_PORT;
}

bool osg_board_Gpio_checkPin(const osg_GpioId id)
{
    // From STM32L476xx data sheet:
    // DM00108832 (DocID025976 Rev 4), Figure 9, pag. 59/232
    switch (id)
    {
        case OSG_GPIO_PA0:
        case OSG_GPIO_PA1:
        case OSG_GPIO_PA2:
        case OSG_GPIO_PA3:
        case OSG_GPIO_PA4:
        case OSG_GPIO_PA5:
        case OSG_GPIO_PA6:
        case OSG_GPIO_PA7:
        case OSG_GPIO_PA8:
        case OSG_GPIO_PA9:
        case OSG_GPIO_PA10:
        case OSG_GPIO_PA11:
        case OSG_GPIO_PA12:
        case OSG_GPIO_PA13:
        case OSG_GPIO_PA14:
        case OSG_GPIO_PA15:
        case OSG_GPIO_PA16:
        case OSG_GPIO_PA17:
        case OSG_GPIO_PA18:
        case OSG_GPIO_PA19:
        case OSG_GPIO_PA20:
        case OSG_GPIO_PA21:
        case OSG_GPIO_PA22:
        case OSG_GPIO_PA23:
        case OSG_GPIO_PA24:
        case OSG_GPIO_PA25:
        case OSG_GPIO_PA26:
        case OSG_GPIO_PA27:
        case OSG_GPIO_PA28:
        case OSG_GPIO_PA29:
        case OSG_GPIO_PA30:
        case OSG_GPIO_PA31:
        case OSG_GPIO_PB0:
        case OSG_GPIO_PB1:
        case OSG_GPIO_PB2:
        case OSG_GPIO_PB3:
        case OSG_GPIO_PB4:
        case OSG_GPIO_PB5:
        case OSG_GPIO_PB6:
        case OSG_GPIO_PB7:
        case OSG_GPIO_PB8:
        case OSG_GPIO_PB9:
        case OSG_GPIO_PB10:
        case OSG_GPIO_PB11:
        case OSG_GPIO_PB12:
        case OSG_GPIO_PB13:
        case OSG_GPIO_PB14:
        case OSG_GPIO_PB15:
        case OSG_GPIO_PB16:
        case OSG_GPIO_PB17:
        case OSG_GPIO_PB18:
        case OSG_GPIO_PB19:
        case OSG_GPIO_PB20:
        case OSG_GPIO_PB21:
        case OSG_GPIO_PB22:
        case OSG_GPIO_PB23:
        case OSG_GPIO_PC0:
        case OSG_GPIO_PC1:
        case OSG_GPIO_PC2:
        case OSG_GPIO_PC3:
        case OSG_GPIO_PC4:
        case OSG_GPIO_PC5:
        case OSG_GPIO_PC6:
        case OSG_GPIO_PC7:
        case OSG_GPIO_PC8:
        case OSG_GPIO_PC9:
        case OSG_GPIO_PC10:
        case OSG_GPIO_PC11:
        case OSG_GPIO_PC12:
        case OSG_GPIO_PC13:
        case OSG_GPIO_PC14:
        case OSG_GPIO_PC15:
        case OSG_GPIO_PC16:
        case OSG_GPIO_PC17:
        case OSG_GPIO_PC18:
        case OSG_GPIO_PC19:
        case OSG_GPIO_PC24:
        case OSG_GPIO_PC25:
        case OSG_GPIO_PC26:
        case OSG_GPIO_PC27:
        case OSG_GPIO_PC28:
        case OSG_GPIO_PC29:
        case OSG_GPIO_PD0:
        case OSG_GPIO_PD1:
        case OSG_GPIO_PD2:
        case OSG_GPIO_PD3:
        case OSG_GPIO_PD4:
        case OSG_GPIO_PD5:
        case OSG_GPIO_PD6:
        case OSG_GPIO_PD7:
        case OSG_GPIO_PD8:
        case OSG_GPIO_PD9:
        case OSG_GPIO_PD10:
        case OSG_GPIO_PD11:
        case OSG_GPIO_PD12:
        case OSG_GPIO_PD13:
        case OSG_GPIO_PD14:
        case OSG_GPIO_PD15:
        case OSG_GPIO_PE0:
        case OSG_GPIO_PE1:
        case OSG_GPIO_PE2:
        case OSG_GPIO_PE3:
        case OSG_GPIO_PE4:
        case OSG_GPIO_PE5:
        case OSG_GPIO_PE6:
        case OSG_GPIO_PE7:
        case OSG_GPIO_PE8:
        case OSG_GPIO_PE9:
        case OSG_GPIO_PE10:
        case OSG_GPIO_PE11:
        case OSG_GPIO_PE12:
        case OSG_GPIO_PE13:
        case OSG_GPIO_PE16:
        case OSG_GPIO_PE17:
        case OSG_GPIO_PE18:
        case OSG_GPIO_PE19:
        case OSG_GPIO_PE20:
        case OSG_GPIO_PE21:
        case OSG_GPIO_PE22:
        case OSG_GPIO_PE23:
            return true;
        default:
            return false;
    }
}



bool osg_board_Gpio_read(osg_Gpio * self)
{
    uint32_t readState = GPIO_PinRead((osg_board_Gpio *)self->handler, (_osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id))));
    if (readState == 0)
        return false;
    else
        return true;
}

void osg_board_Gpio_write(osg_Gpio * self, const bool value)
{
    if (value == true)
        GPIO_PortSet((osg_board_Gpio *)self->handler, (1u << _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id))));
    else
        GPIO_PortClear((osg_board_Gpio *)self->handler, (1u << _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id))));
}

void osg_board_Gpio_toggle(osg_Gpio * self)
{
    GPIO_PortToggle((osg_board_Gpio *)self->handler, (1u << _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id))));
}

bool osg_board_Gpio_lock(osg_Gpio * self)
{
    // to validate
    port_pin_config_t portConfig;
    portConfig.lockRegister = kPORT_LockRegister;
    osg_board_Port * port = _osg_board_Gpio_getPortHandler(osg_Gpio_getPort(self->id));
    uint32_t pin = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id));
    PORT_SetPinConfig(port, pin, &portConfig);
    return true;
}

void osg_board_Gpio_setExtInterruptCallback(const osg_GpioId id, osg_GpioInterruptCallback callback, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPort port = osg_Gpio_getPort(id);
    _osg_board_Gpio_setCallback(port, callback);

    if (callback != NULL)
    {
        osg_board_Gpio_enableExtInterrupt(id, preemptionPriority, subPriority);
    }
    else
    {
        osg_board_Gpio_disableExtInterrupt(id);
    }
}

void osg_board_Gpio_enableExtInterrupt(const osg_GpioId id, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPort port = osg_Gpio_getPort(id);
    if (_osg_board_Gpio_getCallback(port) == NULL)
        osg_error("ERROR: Impossible to enable EXT Interrtupt; Callback not set.");

    IRQn_Type irq = _osg_board_Gpio_decodeIrqType(port);
    EnableIRQ(irq);
}

void osg_board_Gpio_disableExtInterrupt(const osg_GpioId id)
{
    IRQn_Type irq = _osg_board_Gpio_decodeIrqType(osg_Gpio_getPort(id));
    DisableIRQ(irq);
}
