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
#include "osg/drivers/Uart.h"
#include "osg/utils.h"
#include "osg/drivers/Gpio.h"
#include <board.h>

osg_GpioAlternateFunction _osg_board_Uart_getGpioAlternateFunction(const osg_UartId id);

void osg_Uart_ctor(osg_Uart * self,
    const osg_UartId id,
    const osg_UartBaudRate baud,
    const osg_UartParity parity,
    const osg_UartBits bits,
    const osg_UartStopBits stopBits,
    const osg_UartFlowControl flow,
    void * circularTxBuffer,
    Size txBufferSize,
    void * circularRxBuffer,
    Size rxBufferSize,
    const osg_GpioId txPin,
    const osg_GpioId rxPin,
    const osg_GpioId ctsPin,
    const osg_GpioId rtsPin
    )
{
    osg_assert(osg_board_gpio_checkPin(txPin) == TRUE, "ASSERT FAILED: Selected UART TX Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_gpio_checkPin(rxPin) == TRUE, "ASSERT FAILED: Selected UART RX Pin is NOT available on this board. Please check the data sheet.");

    osg_Gpio_ctor(&self->txPin, txPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_NOPULL, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_Uart_getGpioAlternateFunction(id));
    osg_Gpio_ctor(&self->rxPin, rxPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_NOPULL, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_Uart_getGpioAlternateFunction(id));

    if (flow == OSG_UART_FLOWCONTROL_RTS
        || flow == OSG_UART_FLOWCONTROL_RTS_CTS)
    {
        osg_assert(osg_board_gpio_checkPin(rtsPin) == TRUE, "ASSERT FAILED: Selected UART RTS Pin is NOT available on this board. Please check the data sheet.");
        osg_Gpio_ctor(&self->rtsPin, rtsPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_PULLUP, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_Uart_getGpioAlternateFunction(id));
    }
    else
    {
        self->rtsPin.handler = NULL;
    }
    
    if (flow == OSG_UART_FLOWCONTROL_CTS
        || flow == OSG_UART_FLOWCONTROL_RTS_CTS)
    {
        osg_assert(osg_board_gpio_checkPin(ctsPin) == TRUE, "ASSERT FAILED: Selected UART CTS Pin is NOT available on this board. Please check the data sheet.");
        osg_Gpio_ctor(&self->ctsPin, ctsPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_PULLUP, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_Uart_getGpioAlternateFunction(id));
    }
    else
    {
        self->ctsPin.handler = NULL;
    }

    self->id = id;

    osg_board_Uart_ctor(self,
        baud,
        parity,
        bits,
        stopBits,
        flow,
        circularTxBuffer,
        txBufferSize,
        circularRxBuffer,
        rxBufferSize
    );
}

void osg_Uart_dtor(osg_Uart * self)
{
    osg_board_Uart_dtor(self);
    
    osg_Gpio_dtor(&self->txPin);
    osg_Gpio_dtor(&self->rxPin);
    
    if (self->ctsPin.handler != NULL)
        osg_Gpio_dtor(&self->ctsPin);
    
    if (self->rtsPin.handler != NULL)
        osg_Gpio_dtor(&self->rtsPin);
}

Bool osg_Uart_sendBlocking(osg_Uart * self, const void * buffer, const size_t size, const uint32_t timeout)
{
    return osg_board_Uart_sendBlocking(self, buffer, size, timeout);
}

Bool osg_Uart_sendNonBlocking(osg_Uart * self, const void * buffer, const size_t size)
{
    return osg_board_Uart_sendNonBlocking(self, buffer, size);
}

Bool osg_Uart_sendBuffered(osg_Uart * self, const void * buffer, const Size bufferSize)
{
    return osg_board_Uart_sendBuffered(self, buffer, bufferSize);
}

void osg_Uart_setBufferedCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx)
{
    osg_board_Uart_setBufferedCallbacks(self, tx, rx);
}

void osg_Uart_setNbCallbacks(osg_Uart * self, osg_UartCallback tx, osg_UartCallback rx)
{
    osg_board_Uart_setNbCallbacks(self, tx, rx);
}

Bool osg_Uart_receiveBlocking(osg_Uart * self, void * buffer, const Size size, const uint32_t timeout)
{
    return osg_board_Uart_receiveBlocking(self, buffer, size, timeout);
}

Bool osg_Uart_receiveNonBlocking(osg_Uart * self, void * buffer, const Size size)
{
    return osg_board_Uart_receiveNonBlocking(self, buffer, size);
}

Size osg_Uart_receiveBuffered(osg_Uart * self, void * buffer, const Size bufferSize)
{
    return osg_board_Uart_receiveBuffered(self, buffer, bufferSize);
}

void osg_Uart_startReceiveBuffered(osg_Uart * self)
{
    osg_board_Uart_startReceiveBuffered(self);
}

void osg_Uart_stopReceiveBuffered(osg_Uart * self)
{
    osg_board_Uart_stopReceiveBuffered(self);
}

Bool osg_Uart_isReceiveBufferedEnabled(osg_Uart * self)
{
    return osg_board_Uart_isReceiveBufferedEnabled(self);
}

Size osg_Uart_getReceivedCharsRxBuffer(osg_Uart * self)
{
    return osg_board_Uart_getReceivedCharsRxBuffer(self);
}
