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

void osg_board_Gpio_ctor(
    osg_Gpio * self,
    const osg_GpioConfig * const config)
{
    if (osg_board_Gpio_checkPin(config->id) == false)
    {
        osg_error("PIN Unavailable on this board.");
        return;
    }

    GPIO_InitTypeDef configG = {0};
    configG.Pin = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(config->id));
    configG.Mode = _osg_board_Gpio_decodeMode(config->mode);
    configG.Pull = _osg_board_Gpio_decodePull(config->pull);
    configG.Speed = _osg_board_Gpio_decodeSpeed(config->speed);
    if (config->alternate != 0)
        configG.Alternate = config->alternate;

    const osg_GpioPort port = osg_Gpio_getPort(config->id);
    self->handler = _osg_board_Gpio_getPortHandler(port);

    _osg_board_Gpio_enablePort(port);

    HAL_GPIO_Init(self->handler, &configG);
}

void osg_board_Gpio_dtor(osg_Gpio * self)
{
    HAL_GPIO_DeInit(self->handler, _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id)));
}

osg_PinMax osg_board_Gpio_getMaxPinPerPort(void)
{
    return OSG_16_PIN_PER_PORT;
}

bool osg_board_Gpio_checkPin(osg_GpioId id)
{
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
        case OSG_GPIO_PE14:
        case OSG_GPIO_PE15:
        case OSG_GPIO_PF0:
        case OSG_GPIO_PF1:
        case OSG_GPIO_PF2:
        case OSG_GPIO_PF3:
        case OSG_GPIO_PF4:
        case OSG_GPIO_PF5:
        case OSG_GPIO_PF6:
        case OSG_GPIO_PF7:
        case OSG_GPIO_PF8:
        case OSG_GPIO_PF9:
        case OSG_GPIO_PF10:
        case OSG_GPIO_PF11:
        case OSG_GPIO_PF12:
        case OSG_GPIO_PF13:
        case OSG_GPIO_PF14:
        case OSG_GPIO_PF15:
        case OSG_GPIO_PG0:
        case OSG_GPIO_PG1:
        case OSG_GPIO_PG2:
        case OSG_GPIO_PG3:
        case OSG_GPIO_PG4:
        case OSG_GPIO_PG5:
        case OSG_GPIO_PG6:
        case OSG_GPIO_PG7:
        case OSG_GPIO_PG8:
        case OSG_GPIO_PG9:
        case OSG_GPIO_PG10:
        case OSG_GPIO_PG11:
        case OSG_GPIO_PG12:
        case OSG_GPIO_PG13:
        case OSG_GPIO_PG14:
        case OSG_GPIO_PG15:
        case OSG_GPIO_PH0:
        case OSG_GPIO_PH1:
        case OSG_GPIO_PH2:
        case OSG_GPIO_PH3:
        case OSG_GPIO_PH4:
        case OSG_GPIO_PH5:
        case OSG_GPIO_PH6:
        case OSG_GPIO_PH7:
        case OSG_GPIO_PH8:
        case OSG_GPIO_PH9:
        case OSG_GPIO_PH10:
        case OSG_GPIO_PH11:
        case OSG_GPIO_PH12:
        case OSG_GPIO_PH13:
        case OSG_GPIO_PH14:
        case OSG_GPIO_PH15:
        case OSG_GPIO_PI0:
        case OSG_GPIO_PI1:
        case OSG_GPIO_PI2:
        case OSG_GPIO_PI3:
        case OSG_GPIO_PI4:
        case OSG_GPIO_PI5:
        case OSG_GPIO_PI6:
        case OSG_GPIO_PI7:
        case OSG_GPIO_PI8:
        case OSG_GPIO_PI9:
        case OSG_GPIO_PI10:
        case OSG_GPIO_PI11:
        case OSG_GPIO_PI12:
        case OSG_GPIO_PI13:
        case OSG_GPIO_PI14:
        case OSG_GPIO_PI15:
            return true;
        default:
            return false;
    }
}



bool osg_board_Gpio_read(osg_Gpio * self)
{
    GPIO_PinState readState = HAL_GPIO_ReadPin(self->handler, _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id)));
    return osg_bool(readState != GPIO_PIN_RESET);
}

void osg_board_Gpio_write(osg_Gpio * self, const bool value)
{
    GPIO_PinState writeState = GPIO_PIN_RESET;
    if (value == true)
        writeState = GPIO_PIN_SET;
    HAL_GPIO_WritePin(self->handler, _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id)), writeState);
}

void osg_board_Gpio_toggle(osg_Gpio * self)
{
    HAL_GPIO_TogglePin(self->handler, _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id)));
}

bool osg_board_Gpio_lock(osg_Gpio * self)
{
    if (HAL_GPIO_LockPin(self->handler, _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(self->id))) == HAL_OK)
        return true;

    return false;
}

void osg_board_Gpio_setExtInterruptCallback(const osg_GpioId id, osg_GpioInterruptCallback callback, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPin pin = osg_Gpio_getPin(id);
    _osg_board_Gpio_setCallback(pin, callback);

    if (callback != NULL)
    {
        osg_board_Gpio_enableExtInterrupt(id, preemptionPriority, subPriority);
    }
    else
    {
        bool remove = false;
        switch (pin)
        {
            case OSG_GPIO_PIN_0:
            case OSG_GPIO_PIN_1:
            case OSG_GPIO_PIN_2:
            case OSG_GPIO_PIN_3:
            case OSG_GPIO_PIN_4:
                remove = true;
                break;
            case OSG_GPIO_PIN_5:
            case OSG_GPIO_PIN_6:
            case OSG_GPIO_PIN_7:
            case OSG_GPIO_PIN_8:
            case OSG_GPIO_PIN_9:
                remove = osg_bool(_osg_board_Gpio_getCallback(OSG_GPIO_PIN_5) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_6) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_7) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_8) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_9) == NULL);
                break;
            case OSG_GPIO_PIN_10:
            case OSG_GPIO_PIN_11:
            case OSG_GPIO_PIN_12:
            case OSG_GPIO_PIN_13:
            case OSG_GPIO_PIN_14:
            case OSG_GPIO_PIN_15:
                remove = osg_bool(_osg_board_Gpio_getCallback(OSG_GPIO_PIN_10) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_11) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_12) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_13) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_14) == NULL
                                    && _osg_board_Gpio_getCallback(OSG_GPIO_PIN_15) == NULL);
                break;
            default:
                osg_error("ERROR: Invalid GPIO Pin in GPIO Interrupt initialization.");
                break;
        }
        if (remove == true) osg_board_Gpio_disableExtInterrupt(id);
    }
}

void osg_board_Gpio_enableExtInterrupt(const osg_GpioId id, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPin pin = osg_Gpio_getPin(id);
    if (_osg_board_Gpio_getCallback(pin) == NULL)
        osg_error("ERROR: Impossible to enable EXT Interrtupt; Callback not set.");

    IRQn_Type irq = _osg_board_Gpio_decodeIrqType(pin);

    HAL_NVIC_SetPriority(irq, preemptionPriority, subPriority);
    HAL_NVIC_EnableIRQ(irq);
}

void osg_board_Gpio_disableExtInterrupt(const osg_GpioId id)
{
    IRQn_Type irq = _osg_board_Gpio_decodeIrqType(osg_Gpio_getPin(id));
    HAL_NVIC_DisableIRQ(irq);
}
