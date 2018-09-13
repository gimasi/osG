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
#include <osg.h>
#include <stm32l4xx_hal.h>

typedef GPIO_TypeDef osg_board_Gpio;
static osg_GpioInterruptCallback _osg_gpioCallbacks[OSG_GPIO_PIN_NUMBER];

osg_board_Gpio * _osg_board_gpio_getPortHandler(const osg_GpioPort port)
{
    switch (port)
    {
#ifdef GPIOA
        case OSG_GPIO_PORT_A: return GPIOA;
#endif
#ifdef GPIOB
        case OSG_GPIO_PORT_B: return GPIOB;
#endif
#ifdef GPIOC
        case OSG_GPIO_PORT_C: return GPIOC;
#endif
#ifdef GPIOD
        case OSG_GPIO_PORT_D: return GPIOD;
#endif
#ifdef GPIOE
        case OSG_GPIO_PORT_E: return GPIOE;
#endif
#ifdef GPIOF
        case OSG_GPIO_PORT_F: return GPIOF;
#endif
#ifdef GPIOG
        case OSG_GPIO_PORT_G: return GPIOG;
#endif
#ifdef GPIOH
        case OSG_GPIO_PORT_H: return GPIOH;
#endif
#ifdef GPIOI
        case OSG_GPIO_PORT_I: return GPIOI;
#endif
        default:
            osg_error("ERROR: GPIO Port not recognized.");
            break;;
    }

    return NULL;
}

uint32_t _osg_board_gpio_getMode(const osg_GpioMode value)
{
    switch (value)
    {
        case OSG_GPIO_MODE_INPUT: return GPIO_MODE_INPUT;
        case OSG_GPIO_MODE_OUTPUT_PP: return GPIO_MODE_OUTPUT_PP;
        case OSG_GPIO_MODE_OUTPUT_OD: return GPIO_MODE_OUTPUT_OD;
        case OSG_GPIO_MODE_AF_PP: return GPIO_MODE_AF_PP;
        case OSG_GPIO_MODE_AF_OD: return GPIO_MODE_AF_OD;
        case OSG_GPIO_MODE_ANALOG: return GPIO_MODE_ANALOG;
        case OSG_GPIO_MODE_ANALOG_ADC_CONTROL: return GPIO_MODE_ANALOG_ADC_CONTROL;
        case OSG_GPIO_MODE_IT_RISING: return GPIO_MODE_IT_RISING;
        case OSG_GPIO_MODE_IT_FALLING: return GPIO_MODE_IT_FALLING;
        case OSG_GPIO_MODE_IT_RISING_FALLING: return GPIO_MODE_IT_RISING_FALLING;
        case OSG_GPIO_MODE_EVT_RISING: return GPIO_MODE_EVT_RISING;
        case OSG_GPIO_MODE_EVT_FALLING: return GPIO_MODE_EVT_FALLING;
        case OSG_GPIO_MODE_EVT_RISING_FALLING: return GPIO_MODE_EVT_RISING_FALLING;
        default:
            osg_error("ERROR: Invalid GPIO Mode.");
            break;
    }

    return 0xFFFFFFFF;
}

uint32_t _osg_board_gpio_getPull(const osg_GpioPull value)
{
    switch (value)
    {
        case OSG_GPIO_NOPULL: return GPIO_NOPULL;
        case OSG_GPIO_PULLUP: return GPIO_PULLUP;
        case OSG_GPIO_PULLDOWN: return GPIO_PULLDOWN;
        default:
            osg_error("ERROR: Invalid GPIO Pull.");
            break;
    }

    return 0xFFFFFFFF;
}

uint32_t _osg_board_gpio_getSpeed(const osg_GpioSpeed value)
{
    switch (value)
    {
        case OSG_GPIO_SPEED_FREQ_LOW: return GPIO_SPEED_FREQ_LOW;
        case OSG_GPIO_SPEED_FREQ_MEDIUM: return GPIO_SPEED_FREQ_MEDIUM;
        case OSG_GPIO_SPEED_FREQ_HIGH: return GPIO_SPEED_FREQ_HIGH;
        case OSG_GPIO_SPEED_FREQ_VERY_HIGH: return GPIO_SPEED_FREQ_VERY_HIGH;
        default:
            osg_error("ERROR: Invalid GPIO Speed.");
            break;

    }

    return 0xFFFFFFFF;
}

uint32_t _osg_board_gpio_getAlternate(const osg_GpioAlternateFunction value)
{
    switch (value)
    {
        case OSG_GPIO_ALTERNATE_DEFAULT: return 0;
        case OSG_GPIO_ALTERNATE_I2C1: return GPIO_AF4_I2C1;
        case OSG_GPIO_ALTERNATE_I2C2: return GPIO_AF4_I2C2;
        case OSG_GPIO_ALTERNATE_I2C3: return GPIO_AF4_I2C3;
        case OSG_GPIO_ALTERNATE_SPI1: return GPIO_AF5_SPI1;
        case OSG_GPIO_ALTERNATE_SPI2: return GPIO_AF5_SPI2;
        case OSG_GPIO_ALTERNATE_SPI3: return GPIO_AF6_SPI3;
        case OSG_GPIO_ALTERNATE_UART0: return GPIO_AF8_LPUART1;
        case OSG_GPIO_ALTERNATE_UART1: return GPIO_AF7_USART1;
        case OSG_GPIO_ALTERNATE_UART2: return GPIO_AF7_USART2;
        case OSG_GPIO_ALTERNATE_UART3: return GPIO_AF7_USART3;
        case OSG_GPIO_ALTERNATE_UART4: return GPIO_AF8_UART4;
        case OSG_GPIO_ALTERNATE_UART5: return GPIO_AF8_UART5;
        default:
            osg_error("ERROR: Invalid GPIO Alternate Function.");
            break;
    }

    return 0xFFFFFFFF;
}

uint16_t _osg_board_gpio_getBoardGpioPin(osg_GpioPin osgPin)
{
    switch (osgPin)
    {
        case OSG_GPIO_PIN_0: return GPIO_PIN_0;
        case OSG_GPIO_PIN_1: return GPIO_PIN_1;
        case OSG_GPIO_PIN_2: return GPIO_PIN_2;
        case OSG_GPIO_PIN_3: return GPIO_PIN_3;
        case OSG_GPIO_PIN_4: return GPIO_PIN_4;
        case OSG_GPIO_PIN_5: return GPIO_PIN_5;
        case OSG_GPIO_PIN_6: return GPIO_PIN_6;
        case OSG_GPIO_PIN_7: return GPIO_PIN_7;
        case OSG_GPIO_PIN_8: return GPIO_PIN_8;
        case OSG_GPIO_PIN_9: return GPIO_PIN_9;
        case OSG_GPIO_PIN_10: return GPIO_PIN_10;
        case OSG_GPIO_PIN_11: return GPIO_PIN_11;
        case OSG_GPIO_PIN_12: return GPIO_PIN_12;
        case OSG_GPIO_PIN_13: return GPIO_PIN_13;
        case OSG_GPIO_PIN_14: return GPIO_PIN_14;
        case OSG_GPIO_PIN_15: return GPIO_PIN_15;
        default:
            osg_error("ERROR: Invalid osG Pin number.");
            break;
    }

    return 0xFFFF;
}

osg_GpioPin _osg_board_gpio_getOsgGpioPin(uint16_t stm32Pin)
{
    switch (stm32Pin)
    {
        case GPIO_PIN_0: return OSG_GPIO_PIN_0;
        case GPIO_PIN_1: return OSG_GPIO_PIN_1;
        case GPIO_PIN_2: return OSG_GPIO_PIN_2;
        case GPIO_PIN_3: return OSG_GPIO_PIN_3;
        case GPIO_PIN_4: return OSG_GPIO_PIN_4;
        case GPIO_PIN_5: return OSG_GPIO_PIN_5;
        case GPIO_PIN_6: return OSG_GPIO_PIN_6;
        case GPIO_PIN_7: return OSG_GPIO_PIN_7;
        case GPIO_PIN_8: return OSG_GPIO_PIN_8;
        case GPIO_PIN_9: return OSG_GPIO_PIN_9;
        case GPIO_PIN_10: return OSG_GPIO_PIN_10;
        case GPIO_PIN_11: return OSG_GPIO_PIN_11;
        case GPIO_PIN_12: return OSG_GPIO_PIN_12;
        case GPIO_PIN_13: return OSG_GPIO_PIN_13;
        case GPIO_PIN_14: return OSG_GPIO_PIN_14;
        case GPIO_PIN_15: return OSG_GPIO_PIN_15;
        default:
            osg_error("ERROR: Invalid STM32 Pin number.");
            break;
    }

    return OSG_GPIO_PIN_0;
}

IRQn_Type _osg_board_gpio_getIrqType(const osg_GpioPin pin)
{
    switch (pin)
    {
        case OSG_GPIO_PIN_0:
	    	return EXTI0_IRQn;
        case OSG_GPIO_PIN_1:
            return EXTI1_IRQn;
        case OSG_GPIO_PIN_2:
	    	return EXTI2_IRQn;
        case OSG_GPIO_PIN_3:
            return EXTI3_IRQn;
        case OSG_GPIO_PIN_4:
            return EXTI4_IRQn;
        case OSG_GPIO_PIN_5:
        case OSG_GPIO_PIN_6:
        case OSG_GPIO_PIN_7:
        case OSG_GPIO_PIN_8:
        case OSG_GPIO_PIN_9:
            return EXTI9_5_IRQn;
        case OSG_GPIO_PIN_10:
        case OSG_GPIO_PIN_11:
        case OSG_GPIO_PIN_12:
        case OSG_GPIO_PIN_13:
        case OSG_GPIO_PIN_14:
        case OSG_GPIO_PIN_15:
            return EXTI15_10_IRQn;
        default:
            osg_error("ERROR: Impossible to enable EXT Interrtupt; invalid Pin.");
            break;
    }

    // suppress warning
    return EXTI0_IRQn;
}

void _osg_board_gpio_enablePort(osg_GpioPort port)
{
    switch (port)
    {
#ifdef GPIOA
        case OSG_GPIO_PORT_A: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
#endif
#ifdef GPIOB
        case OSG_GPIO_PORT_B: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
#endif
#ifdef GPIOC
        case OSG_GPIO_PORT_C: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
#endif
#ifdef GPIOD
        case OSG_GPIO_PORT_D: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
#endif
#ifdef GPIOE
        case OSG_GPIO_PORT_E: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
#endif
#ifdef GPIOF
        case OSG_GPIO_PORT_F: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
#endif
#ifdef GPIOG
        case OSG_GPIO_PORT_G: __HAL_RCC_GPIOG_CLK_ENABLE(); break;
#endif
#ifdef GPIOH
        case OSG_GPIO_PORT_H: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
#endif
#ifdef GPIOI
        case OSG_GPIO_PORT_I: __HAL_RCC_GPIOI_CLK_ENABLE(); break;
#endif
        default:
            osg_error("ERROR: Invalid GPIO Port in GPIO initialization.");
            break;
    }
}

void osg_board_gpio_ctor(
    osg_Gpio * self,
    const osg_GpioId id,
    const osg_GpioMode mode,
    const osg_GpioPull pull,
    const osg_GpioSpeed speed,
    const osg_GpioAlternateFunction alternate)
{
    if (osg_board_gpio_checkPin(id) == FALSE)
    {
        osg_error("PIN Unavailable on this board.");
        return;
    }

    GPIO_InitTypeDef config = {0};
    config.Pin = _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(id));
    config.Mode = _osg_board_gpio_getMode(mode);
    config.Pull = _osg_board_gpio_getPull(pull);
    config.Speed = _osg_board_gpio_getSpeed(speed);
    // @todo Check in debug that alternate is valid for given pin
    config.Alternate = _osg_board_gpio_getAlternate(alternate);

    const osg_GpioPort port = osg_gpio_getPort(id);
    self->handler = _osg_board_gpio_getPortHandler(port);

    _osg_board_gpio_enablePort(port);

    HAL_GPIO_Init((osg_board_Gpio *)self->handler, &config);
}

void osg_board_gpio_dtor(osg_Gpio * self)
{
    HAL_GPIO_DeInit(self->handler, _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id)));
}

osg_PinMax osg_board_gpio_getMaxPinPerPort(void)
{
    return OSG_16_PIN_PER_PORT;
}

Bool osg_board_gpio_checkPin(osg_GpioId id)
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
        case OSG_GPIO_PD2:
        case OSG_GPIO_PH0:
        case OSG_GPIO_PH1:
            return TRUE;
        default:
            return FALSE;
    }
}



Bool osg_board_gpio_read(osg_Gpio * self)
{
    GPIO_PinState readState = HAL_GPIO_ReadPin((osg_board_Gpio *)self->handler, _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id)));
    if (readState == GPIO_PIN_RESET)
        return FALSE;
    else
        return TRUE;
}

void osg_board_gpio_write(osg_Gpio * self, const Bool value)
{
    GPIO_PinState writeState = GPIO_PIN_RESET;
    if (value == TRUE)
        writeState = GPIO_PIN_SET;
    HAL_GPIO_WritePin((osg_board_Gpio *)self->handler, _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id)), writeState);
}

void osg_board_gpio_toggle(osg_Gpio * self)
{
    HAL_GPIO_TogglePin((osg_board_Gpio *)self->handler, _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id)));
}

Bool osg_board_gpio_lock(osg_Gpio * self)
{
    if (HAL_GPIO_LockPin((osg_board_Gpio *)self->handler, _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id))) == HAL_OK)
        return TRUE;

    return FALSE;
}

void osg_board_gpio_setExtInterruptCallback(const osg_GpioId id, osg_GpioInterruptCallback callback, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPin pin = osg_gpio_getPin(id);
    _osg_gpioCallbacks[pin] = callback;

    if (callback != NULL)
    {
        osg_board_gpio_enableExtInterrupt(id, preemptionPriority, subPriority);
    }
    else
    {
        int remove = FALSE;
        switch (pin)
        {
            case OSG_GPIO_PIN_0:
            case OSG_GPIO_PIN_1:
            case OSG_GPIO_PIN_2:
            case OSG_GPIO_PIN_3:
            case OSG_GPIO_PIN_4:
                remove = TRUE;
                break;
            case OSG_GPIO_PIN_5:
            case OSG_GPIO_PIN_6:
            case OSG_GPIO_PIN_7:
            case OSG_GPIO_PIN_8:
            case OSG_GPIO_PIN_9:
                remove = _osg_gpioCallbacks[OSG_GPIO_PIN_5] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_6] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_7] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_8] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_9] == NULL;
                break;
            case OSG_GPIO_PIN_10:
            case OSG_GPIO_PIN_11:
            case OSG_GPIO_PIN_12:
            case OSG_GPIO_PIN_13:
            case OSG_GPIO_PIN_14:
            case OSG_GPIO_PIN_15:
                remove = _osg_gpioCallbacks[OSG_GPIO_PIN_10] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_11] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_12] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_13] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_14] == NULL
                    && _osg_gpioCallbacks[OSG_GPIO_PIN_15] == NULL;
                break;
            default:
                osg_error("ERROR: Invalid GPIO Pin in GPIO Interrupt initialization.");
                break;
        }
        if (remove) osg_board_gpio_disableExtInterrupt(id);
    }
}

void osg_board_gpio_enableExtInterrupt(const osg_GpioId id, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPin pin = osg_gpio_getPin(id);
    if (_osg_gpioCallbacks[pin] == NULL)
        osg_error("ERROR: Impossible to enable EXT Interrtupt; Callback not set.");

    IRQn_Type irq = _osg_board_gpio_getIrqType(pin);

    HAL_NVIC_SetPriority(irq, preemptionPriority, subPriority);
    HAL_NVIC_EnableIRQ(irq);
}

void osg_board_gpio_disableExtInterrupt(const osg_GpioId id)
{
    IRQn_Type irq = _osg_board_gpio_getIrqType(osg_gpio_getPin(id));
    HAL_NVIC_DisableIRQ(irq);
}

void osg_gpioCallback(uint16_t pin)
{
    if (_osg_gpioCallbacks[pin] != NULL)
        _osg_gpioCallbacks[pin]((osg_GpioPin)pin);
}

// /////////////////////////////////////////////////////////////////////////////
// HAL internal callbacks
// /////////////////////////////////////////////////////////////////////////////

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    osg_gpioCallback(_osg_board_gpio_getOsgGpioPin(GPIO_Pin));
}
