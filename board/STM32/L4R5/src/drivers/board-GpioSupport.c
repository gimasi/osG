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
#include "board-GpioSupport.h"

static osg_GpioInterruptCallback _osg_board_Gpio_callbacks[OSG_GPIO_PIN_NUMBER];


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

osg_board_Gpio * _osg_board_Gpio_getPortHandler(const osg_GpioPort port)
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


// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////

uint32_t _osg_board_Gpio_decodeMode(const osg_GpioMode value)
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

uint32_t _osg_board_Gpio_decodePull(const osg_GpioPull value)
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

uint32_t _osg_board_Gpio_decodeSpeed(const osg_GpioSpeed value)
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

//uint32_t _osg_board_Gpio_decodeAlternate(const osg_GpioAlternateFunction value)
//{
//    switch (value)
//    {
//        case OSG_GPIO_ALTERNATE_DEFAULT: return 0;
//        case OSG_GPIO_ALTERNATE_I2C1: return GPIO_AF4_I2C1;
//        case OSG_GPIO_ALTERNATE_I2C2: return GPIO_AF4_I2C2;
//        case OSG_GPIO_ALTERNATE_I2C3: return GPIO_AF4_I2C3;
//        case OSG_GPIO_ALTERNATE_SPI1_CLK:
//        case OSG_GPIO_ALTERNATE_SPI1_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI1_MISO:
//        case OSG_GPIO_ALTERNATE_SPI1_SS:
//            return GPIO_AF5_SPI1;
//        case OSG_GPIO_ALTERNATE_SPI2_CLK:
//        case OSG_GPIO_ALTERNATE_SPI2_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI2_MISO:
//        case OSG_GPIO_ALTERNATE_SPI2_SS:
//            return GPIO_AF5_SPI2;
//        case OSG_GPIO_ALTERNATE_SPI3_CLK:
//        case OSG_GPIO_ALTERNATE_SPI3_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI3_MISO:
//        case OSG_GPIO_ALTERNATE_SPI3_SS:
//            return GPIO_AF6_SPI3;
//        case OSG_GPIO_ALTERNATE_UART0: return GPIO_AF8_LPUART1;
//        case OSG_GPIO_ALTERNATE_UART1: return GPIO_AF7_USART1;
//        case OSG_GPIO_ALTERNATE_UART2: return GPIO_AF7_USART2;
//        case OSG_GPIO_ALTERNATE_UART3: return GPIO_AF7_USART3;
//        case OSG_GPIO_ALTERNATE_UART4: return GPIO_AF8_UART4;
//        case OSG_GPIO_ALTERNATE_UART5: return GPIO_AF8_UART5;
//        default:
//            osg_error("ERROR: Invalid GPIO Alternate Function.");
//            break;
//    }
//
//    return 0xFFFFFFFF;
//}

uint16_t _osg_board_Gpio_decodeGpioPin(const osg_GpioPin osgPin)
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

IRQn_Type _osg_board_Gpio_decodeIrqType(const osg_GpioPin pin)
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


// ///////////////////////////////////////////////////////
// ENCODE FROM STM TO OSG
// ///////////////////////////////////////////////////////

osg_GpioPin _osg_board_Gpio_encodeGpioPin(const uint16_t stm32Pin)
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


// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////

void _osg_board_Gpio_enablePort(const osg_GpioPort port)
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


// ///////////////////////////////////////////////////////
// SUPPORT TO USER CALLBACK
// ///////////////////////////////////////////////////////

osg_GpioInterruptCallback _osg_board_Gpio_getCallback(const osg_GpioPin pin)
{
    return _osg_board_Gpio_callbacks[pin];
}

void _osg_board_Gpio_setCallback(const osg_GpioPin pin, const osg_GpioInterruptCallback callback)
{
    if (callback != NULL)
        _osg_board_Gpio_callbacks[pin] = callback;
}

void _osg_board_Gpio_callback(const uint16_t pin)
{
    if (_osg_board_Gpio_callbacks[pin] != NULL)
        _osg_board_Gpio_callbacks[pin]((osg_GpioPin)pin);
}


// /////////////////////////////////////////////////////////////////////////////
// HAL INTERNAL CALLBACKS
// /////////////////////////////////////////////////////////////////////////////

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    _osg_board_Gpio_callback(_osg_board_Gpio_encodeGpioPin(GPIO_Pin));
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}
