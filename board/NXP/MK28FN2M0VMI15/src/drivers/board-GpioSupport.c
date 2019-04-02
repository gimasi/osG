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

static osg_GpioInterruptCallback _osg_board_Gpio_callbacks[OSG_GPIO_PORT_NUMBER];


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

osg_board_Port * _osg_board_Gpio_getPortHandler(const osg_GpioPort port)
{
    switch (port)
    {
#ifdef PORTA
        case OSG_GPIO_PORT_A: return PORTA;
#endif
#ifdef PORTB
        case OSG_GPIO_PORT_B: return PORTB;
#endif
#ifdef PORTC
        case OSG_GPIO_PORT_C: return PORTC;
#endif
#ifdef PORTD
        case OSG_GPIO_PORT_D: return PORTD;
#endif
#ifdef PORTE
        case OSG_GPIO_PORT_E: return PORTE;
#endif
#ifdef PORTF
        case OSG_GPIO_PORT_F: return PORTF;
#endif
#ifdef PORTG
        case OSG_GPIO_PORT_G: return PORTG;
#endif
#ifdef PORTH
        case OSG_GPIO_PORT_H: return PORTH;
#endif
#ifdef PORTI
        case OSG_GPIO_PORT_I: return PORTI;
#endif
        default:
            osg_error("ERROR: GPIO Port not recognized.");
            break;;
    }

    return NULL;
}

osg_board_Gpio * _osg_board_Gpio_getGpioHandler(const osg_GpioPort port)
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
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////

gpio_pin_direction_t _osg_board_Gpio_decodeDirection(const osg_GpioMode value)
{
    switch (value)
    {
        case OSG_GPIO_MODE_IT_RISING:
        case OSG_GPIO_MODE_IT_FALLING:
        case OSG_GPIO_MODE_IT_RISING_FALLING:
        case OSG_GPIO_MODE_IT_LOGIC_0:
        case OSG_GPIO_MODE_IT_LOGIC_1:
        case OSG_GPIO_MODE_INPUT:
            return kGPIO_DigitalInput;
        case OSG_GPIO_MODE_OUTPUT_PP:
        case OSG_GPIO_MODE_OUTPUT_OD:
            return kGPIO_DigitalOutput;
        default:
            osg_error("ERROR: Invalid GPIO Mode.");
            break;
    }

    return kGPIO_DigitalInput;
}

uint32_t _osg_board_Gpio_decodePull(const osg_GpioPull value)
{
    switch (value)
    {
        case OSG_GPIO_NOPULL: return kPORT_PullDisable;
        case OSG_GPIO_PULLUP: return kPORT_PullUp;
        case OSG_GPIO_PULLDOWN: return kPORT_PullDown;
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
        case OSG_GPIO_SPEED_FREQ_LOW: return kPORT_SlowSlewRate;
        case OSG_GPIO_SPEED_FREQ_VERY_HIGH: return kPORT_FastSlewRate;
        default:
            osg_error("ERROR: Invalid GPIO Speed. For this architecture use OSG_GPIO_SPEED_FREQ_LOW or OSG_GPIO_SPEED_FREQ_VERY_HIGH");
            break;

    }

    return 0xFFFFFFFF;
}

uint32_t _osg_board_Gpio_decodeOpenDrain(const osg_GpioMode value)
{
    switch (value)
    {
        case OSG_GPIO_MODE_OUTPUT_OD:
        case OSG_GPIO_MODE_AF_OD:
            return kPORT_OpenDrainEnable;
        default:
            return kPORT_OpenDrainDisable;
    }
}

//port_mux_t _osg_board_Gpio_decodeAlternate(const osg_GpioAlternateFunction value, const osg_GpioId pin)
//{
//    switch (value)
//    {
//        case OSG_GPIO_ALTERNATE_DEFAULT: return kPORT_MuxAsGpio;
//#if OSG_USE_I2C == 1
//        case OSG_GPIO_ALTERNATE_I2C0:
//        case OSG_GPIO_ALTERNATE_I2C1:
//        case OSG_GPIO_ALTERNATE_I2C2:
//        case OSG_GPIO_ALTERNATE_I2C3:
//            return _osg_board_I2c_decodePinMuxAlt(pin);
//#endif
//#if OSG_USE_SPI == 1
//        case OSG_GPIO_ALTERNATE_SPI0_CLK:
//        case OSG_GPIO_ALTERNATE_SPI0_MISO:
//        case OSG_GPIO_ALTERNATE_SPI0_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI0_SS:
//        case OSG_GPIO_ALTERNATE_SPI1_CLK:
//        case OSG_GPIO_ALTERNATE_SPI1_MISO:
//        case OSG_GPIO_ALTERNATE_SPI1_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI1_SS:
//        case OSG_GPIO_ALTERNATE_SPI2_CLK:
//        case OSG_GPIO_ALTERNATE_SPI2_MISO:
//        case OSG_GPIO_ALTERNATE_SPI2_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI2_SS:
//        case OSG_GPIO_ALTERNATE_SPI3_CLK:
//        case OSG_GPIO_ALTERNATE_SPI3_MISO:
//        case OSG_GPIO_ALTERNATE_SPI3_MOSI:
//        case OSG_GPIO_ALTERNATE_SPI3_SS:
//            return _osg_board_Spi_decodePinMuxAlt(pin, value);
//#endif
//#if OSG_USE_UART == 1
//        case OSG_GPIO_ALTERNATE_UART0:
//        case OSG_GPIO_ALTERNATE_UART1:
//        case OSG_GPIO_ALTERNATE_UART2:
//        case OSG_GPIO_ALTERNATE_UART3:
//        case OSG_GPIO_ALTERNATE_UART4:
//            return _osg_board_Uart_decodePinMuxAlt(pin);
//#endif
//        default:
//            osg_error("ERROR: Invalid GPIO Alternate Function.");
//            break;
//    }
//
//    // suppress warning
//    return kPORT_MuxAsGpio;
//}

uint32_t _osg_board_Gpio_decodeGpioPin(const osg_GpioPin osgPin)
{
    return (uint32_t)osgPin;
}

bool _osg_board_Gpio_isInterruptMode(const osg_GpioMode value)
{
    switch (value)
    {
        case OSG_GPIO_MODE_IT_RISING:
        case OSG_GPIO_MODE_IT_FALLING:
        case OSG_GPIO_MODE_IT_RISING_FALLING:
        case OSG_GPIO_MODE_IT_LOGIC_0:
        case OSG_GPIO_MODE_IT_LOGIC_1:
            return true;
        default:
            return false;
    }
}

IRQn_Type _osg_board_Gpio_decodeIrqType(const osg_GpioPort port)
{
    switch (port)
    {
#ifdef PORTA
        case OSG_GPIO_PORT_A: return PORTA_IRQn;
#endif
#ifdef PORTB
        case OSG_GPIO_PORT_B: return PORTB_IRQn;
#endif
#ifdef PORTC
        case OSG_GPIO_PORT_C: return PORTC_IRQn;
#endif
#ifdef PORTD
        case OSG_GPIO_PORT_D: return PORTD_IRQn;
#endif
#ifdef PORTE
        case OSG_GPIO_PORT_E: return PORTE_IRQn;
#endif
#ifdef PORTF
        case OSG_GPIO_PORT_F: return PORTF_IRQn;
#endif
#ifdef PORTG
        case OSG_GPIO_PORT_G: return PORTG_IRQn;
#endif
#ifdef PORTH
        case OSG_GPIO_PORT_H: return PORTH_IRQn;
#endif
#ifdef PORTI
        case OSG_GPIO_PORT_I: return PORTI_IRQn;
#endif
        default:
            osg_error("ERROR: GPIO Port not recognized.");
            break;;
    }

    // suppress warning
    return PORTA_IRQn;
}

port_interrupt_t _osg_board_Gpio_decodeInterruptEdge(const osg_GpioMode value)
{
    switch (value)
    {
        case OSG_GPIO_MODE_IT_RISING: return kPORT_InterruptRisingEdge;
        case OSG_GPIO_MODE_IT_FALLING: return kPORT_InterruptFallingEdge;
        case OSG_GPIO_MODE_IT_RISING_FALLING: return kPORT_InterruptEitherEdge;
        case OSG_GPIO_MODE_IT_LOGIC_0: return kPORT_InterruptLogicZero;
        case OSG_GPIO_MODE_IT_LOGIC_1: return kPORT_InterruptLogicOne;
        default:
            osg_error("ERROR: Invalid GPIO Interrupt Edge.");
            break;
    }

    // suppress warning
    return kPORT_InterruptRisingEdge;
}


// ///////////////////////////////////////////////////////
// ENABLE
// ///////////////////////////////////////////////////////

void _osg_board_Gpio_enablePort(const osg_GpioPort port)
{
    switch (port)
    {
#ifdef PORTA
        case OSG_GPIO_PORT_A: CLOCK_EnableClock(kCLOCK_PortA); break;
#endif
#ifdef PORTB
        case OSG_GPIO_PORT_B: CLOCK_EnableClock(kCLOCK_PortB); break;
#endif
#ifdef PORTC
        case OSG_GPIO_PORT_C: CLOCK_EnableClock(kCLOCK_PortC); break;
#endif
#ifdef PORTD
        case OSG_GPIO_PORT_D: CLOCK_EnableClock(kCLOCK_PortD); break;
#endif
#ifdef PORTE
        case OSG_GPIO_PORT_E: CLOCK_EnableClock(kCLOCK_PortE); break;
#endif
#ifdef PORTF
        case OSG_GPIO_PORT_F: CLOCK_EnableClock(kCLOCK_PortF); break;
#endif
#ifdef PORTG
        case OSG_GPIO_PORT_G: CLOCK_EnableClock(kCLOCK_PortG); break;
#endif
#ifdef PORTH
        case OSG_GPIO_PORT_H: CLOCK_EnableClock(kCLOCK_PortH); break;
#endif
#ifdef PORTI
        case OSG_GPIO_PORT_I: CLOCK_EnableClock(kCLOCK_PortI); break;
#endif
        default:
            osg_error("ERROR: Invalid GPIO Port in GPIO initialization.");
            break;
    }
}


// ///////////////////////////////////////////////////////
// SUPPORT TO USER CALLBACK
// ///////////////////////////////////////////////////////

osg_GpioInterruptCallback _osg_board_Gpio_getCallback(const osg_GpioPort port)
{
    return _osg_board_Gpio_callbacks[port];
}

void _osg_board_Gpio_setCallback(const osg_GpioPort port, const osg_GpioInterruptCallback callback)
{
    if (callback != NULL)
        _osg_board_Gpio_callbacks[port] = callback;
}
