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

#include <fsl_port.h>
#include <fsl_gpio.h>


typedef GPIO_Type osg_board_Gpio;
typedef PORT_Type osg_board_Port;
static osg_GpioInterruptCallback _osg_gpioCallbacks[OSG_GPIO_PORT_NUMBER];
port_mux_t _osg_board_I2c_getPinPortMuxForI2c(const osg_GpioId pin);
port_mux_t _osg_board_spi_getPinPortMuxForSpi(const osg_GpioId pin);
port_mux_t _osg_board_Uart_getPinPortMuxForUart(const osg_GpioId pin);

static osg_board_Gpio * _osg_board_gpio_getGpioHandler(const osg_GpioPort port)
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

osg_board_Port * _osg_board_gpio_getPortHandler(const osg_GpioPort port)
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

static gpio_pin_direction_t _osg_board_gpio_getDirection(const osg_GpioMode value)
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

static uint32_t _osg_board_gpio_getPull(const osg_GpioPull value)
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

static uint32_t _osg_board_gpio_getSpeed(const osg_GpioSpeed value)
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

static uint32_t _osg_board_gpio_getOpenDrain(const osg_GpioMode value)
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

static port_interrupt_t _osg_board_gpio_getInterruptEdge(const osg_GpioMode value)
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

static port_mux_t _osg_board_gpio_getAlternate(const osg_GpioAlternateFunction value, const osg_GpioId pin)
{
    switch (value)
    {
        case OSG_GPIO_ALTERNATE_DEFAULT: return kPORT_MuxAsGpio;
        case OSG_GPIO_ALTERNATE_I2C0:
        case OSG_GPIO_ALTERNATE_I2C1:
        case OSG_GPIO_ALTERNATE_I2C2:
        case OSG_GPIO_ALTERNATE_I2C3:
            return _osg_board_I2c_getPinPortMuxForI2c(pin);
        case OSG_GPIO_ALTERNATE_SPI1:
        case OSG_GPIO_ALTERNATE_SPI2:
        case OSG_GPIO_ALTERNATE_SPI3:
            return _osg_board_spi_getPinPortMuxForSpi(pin);
        case OSG_GPIO_ALTERNATE_UART0:
        case OSG_GPIO_ALTERNATE_UART1:
        case OSG_GPIO_ALTERNATE_UART2:
        case OSG_GPIO_ALTERNATE_UART3:
        case OSG_GPIO_ALTERNATE_UART4:
            return _osg_board_Uart_getPinPortMuxForUart(pin);
        default:
            osg_error("ERROR: Invalid GPIO Alternate Function.");
            break;
    }

    // suppress warning
    return kPORT_MuxAsGpio;
}

static uint32_t _osg_board_gpio_getBoardGpioPin(osg_GpioPin osgPin)
{
    return (uint32_t)osgPin;
}

static Bool _osg_board_gpio_isInterruptMode(const osg_GpioMode value)
{
    switch (value)
    {
        case OSG_GPIO_MODE_IT_RISING:
        case OSG_GPIO_MODE_IT_FALLING:
        case OSG_GPIO_MODE_IT_RISING_FALLING:
        case OSG_GPIO_MODE_IT_LOGIC_0:
        case OSG_GPIO_MODE_IT_LOGIC_1:
            return TRUE;
        default:
            return FALSE;
    }
}

static IRQn_Type _osg_board_gpio_getIrqType(const osg_GpioPort port)
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

void _osg_board_gpio_enablePort(osg_GpioPort port)
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

void osg_board_gpio_ctor(osg_Gpio * self,
                         const osg_GpioId id,
                         const osg_GpioMode mode,
                         const osg_GpioPull pull,
                         const osg_GpioSpeed speed,
                         const osg_GpioAlternateFunction alternate)
{
    const osg_GpioPort osg_port = osg_gpio_getPort(id);
    osg_board_Gpio * gpio = _osg_board_gpio_getGpioHandler(osg_port);
    osg_board_Port * port = _osg_board_gpio_getPortHandler(osg_port);
    uint32_t pin = _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(id));

    self->handler = gpio;

    _osg_board_gpio_enablePort(osg_port);

    if (alternate == OSG_GPIO_ALTERNATE_DEFAULT)
    {
        /* The GPIO pin configuration structure.
        *
        * Each pin can only be configured as either an output pin or an input pin at a time.
        * If configured as an input pin, leave the outputConfig unused.
        * Note that in some use cases, the corresponding port property should be configured in advance
        *        with the PORT_SetPinConfig().
        */
        gpio_pin_config_t gpioConfig;
        gpioConfig.pinDirection = _osg_board_gpio_getDirection(mode);
        gpioConfig.outputLogic = 0;
        
        PORT_SetPinMux(port, pin, kPORT_MuxAsGpio);
        GPIO_PinInit(gpio, pin, &gpioConfig);
    }
    else
    {
        // PORT pin configuration structure
        port_pin_config_t portConfig;
        portConfig.pullSelect = _osg_board_gpio_getPull(pull);
        portConfig.slewRate = _osg_board_gpio_getSpeed(speed);
        //portConfig.passiveFilterEnable;
        portConfig.openDrainEnable = _osg_board_gpio_getOpenDrain(mode);
        //portConfig.driveStrength;
        portConfig.mux = _osg_board_gpio_getAlternate(alternate, id);
        portConfig.lockRegister = kPORT_UnlockRegister;
        
        PORT_SetPinConfig(port, pin, &portConfig);
        if (_osg_board_gpio_isInterruptMode(mode) == TRUE)
            PORT_SetPinInterruptConfig(port, pin, _osg_board_gpio_getInterruptEdge(mode));
    }

}

void osg_board_gpio_dtor(osg_Gpio * self)
{
    osg_board_Port * port = _osg_board_gpio_getPortHandler(osg_gpio_getPort(self->id));
    PORT_SetPinMux(port, _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id)), kPORT_PinDisabledOrAnalog);
}

osg_PinMax osg_board_gpio_getMaxPinPerPort(void)
{
    return OSG_32_PIN_PER_PORT;
}

Bool osg_board_gpio_checkPin(const osg_GpioId id)
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
            return TRUE;
        default:
            return FALSE;
    }
}



Bool osg_board_gpio_read(osg_Gpio * self)
{
    uint32_t readState = GPIO_PinRead((osg_board_Gpio *)self->handler, (1u << _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id))));
    if (readState == 0)
        return FALSE;
    else
        return TRUE;
}

void osg_board_gpio_write(osg_Gpio * self, const Bool value)
{
    if (value == TRUE)
        GPIO_PortSet((osg_board_Gpio *)self->handler, (1u << _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id))));
    else
        GPIO_PortClear((osg_board_Gpio *)self->handler, (1u << _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id))));
}

void osg_board_gpio_toggle(osg_Gpio * self)
{
    GPIO_PortToggle((osg_board_Gpio *)self->handler, (1u << _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id))));
}

Bool osg_board_gpio_lock(osg_Gpio * self)
{
    // to validate
    port_pin_config_t portConfig;
    portConfig.lockRegister = kPORT_LockRegister;
    osg_board_Port * port = _osg_board_gpio_getPortHandler(osg_gpio_getPort(self->id));
    uint32_t pin = _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(self->id));
    PORT_SetPinConfig(port, pin, &portConfig);
    return TRUE;
}

void osg_board_gpio_setExtInterruptCallback(const osg_GpioId id, osg_GpioInterruptCallback callback, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPort port = osg_gpio_getPort(id);
    _osg_gpioCallbacks[port] = callback;

    if (callback != NULL)
    {
        osg_board_gpio_enableExtInterrupt(id, preemptionPriority, subPriority);
    }
    else
    {
        osg_board_gpio_disableExtInterrupt(id);
    }
}

void osg_board_gpio_enableExtInterrupt(const osg_GpioId id, const osg_IrqPriority preemptionPriority, const osg_IrqPriority subPriority)
{
    osg_GpioPort port = osg_gpio_getPort(id);
    if (_osg_gpioCallbacks[port] == NULL)
        osg_error("ERROR: Impossible to enable EXT Interrtupt; Callback not set.");

    IRQn_Type irq = _osg_board_gpio_getIrqType(port);
    EnableIRQ(irq);
}

void osg_board_gpio_disableExtInterrupt(const osg_GpioId id)
{
    IRQn_Type irq = _osg_board_gpio_getIrqType(osg_gpio_getPort(id));
    DisableIRQ(irq);
}
