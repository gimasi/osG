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
#include <string.h>
#include "../../include/osg/drivers/Spi.h"
#include "../../include/osg/drivers/Gpio.h"
#include "../../include/osg/utils.h"
#include "../../../board/include/board.h"

void osg_Spi_ctor(osg_Spi * self, const osg_SpiConfig * const config)
{
    osg_assert(osg_board_Gpio_checkPin(config->sclkPin) == true, "ASSERT FAILED: SPI SCLK Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_Gpio_checkPin(config->mosiPin) == true, "ASSERT FAILED: SPI MOSI Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_Gpio_checkPin(config->misoPin) == true, "ASSERT FAILED: SPI MISO Pin is NOT available on this board. Please check the data sheet.");
    if (config->ssPinList != NULL)
    {
        for (Size i = 0; i < config->ssPinListSize; ++i)
        {
            osg_assert(osg_board_Gpio_checkPin(config->ssPinList[i]) == true, "ASSERT FAILED: SPI SS Pin is NOT available on this board. Please check the data sheet.");
        }
    }

    self->id = config->id;

    osg_GpioConfig gpioConfig;
    memset(&gpioConfig, 0, sizeof(gpioConfig));
    
    gpioConfig.id = config->sclkPin;
    gpioConfig.mode = OSG_GPIO_MODE_AF_PP;
    gpioConfig.pull = OSG_GPIO_NOPULL;
    gpioConfig.speed = OSG_GPIO_SPEED_FREQ_VERY_HIGH;
    gpioConfig.alternate = config->alternateFunction;
    osg_Gpio_ctor(&self->sclkPin, &gpioConfig);
    gpioConfig.id = config->mosiPin;
    osg_Gpio_ctor(&self->mosiPin, &gpioConfig);
    gpioConfig.id = config->misoPin;
    osg_Gpio_ctor(&self->misoPin, &gpioConfig);
    
    self->ssPinList = config->ssPinList;
    self->ssPinListSize = config->ssPinListSize;

    osg_board_Spi_ctor(self, config);
}

void osg_Spi_dtor(osg_Spi * self)
{
    osg_Gpio_dtor(&self->sclkPin);
    osg_Gpio_dtor(&self->mosiPin);
    osg_Gpio_dtor(&self->misoPin);

    osg_board_Spi_dtor(self);

    self->ssPinList = NULL;
    self->ssPinListSize = 0;
}

void osg_Spi_setBuffers(osg_Spi * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize)
{
    osg_board_Spi_setBuffers(self, circularTxBuffer, txBufferSize, circularRxBuffer, rxBufferSize);
}

void osg_Spi_setBufferedCallbacks(osg_Spi * self, const osg_SpiCallback tx, const osg_SpiCallback rx)
{
    osg_board_Spi_setBufferedCallbacks(self, tx, rx);
}

void osg_Spi_setNbCallbacks(osg_Spi * self, const osg_SpiCallback tx, const osg_SpiCallback rx)
{
    osg_board_Spi_setNbCallbacks(self, tx, rx);
}

bool osg_Spi_sendBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize, const uint32_t timeout)
{
    return osg_board_Spi_sendBlocking(self, ssPinIndex, buffer, bufferSize, timeout);
}

bool osg_Spi_receiveBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize, const uint32_t timeout)
{
    return osg_board_Spi_receiveBlocking(self, ssPinIndex, buffer, bufferSize, timeout);
}

bool osg_Spi_sendNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    return osg_board_Spi_sendNonBlocking(self, ssPinIndex, buffer, bufferSize);
}

bool osg_Spi_receiveNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    return osg_board_Spi_receiveNonBlocking(self, ssPinIndex, buffer, bufferSize);
}

bool osg_Spi_sendBuffered(osg_Spi * self, const uint16_t ssPinIndex, const void * buffer, const Size bufferSize)
{
    return osg_board_Spi_sendBuffered(self, ssPinIndex, buffer, bufferSize);
}

Size osg_Spi_receiveBuffered(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    return osg_board_Spi_receiveBuffered(self, ssPinIndex, buffer, bufferSize);
}

void osg_Spi_startReceiveBuffered(osg_Spi * self, const uint16_t ssPinIndex)
{
    osg_board_Spi_startReceiveBuffered(self, ssPinIndex);
}

void osg_Spi_stopReceiveBuffered(osg_Spi * self)
{
    osg_board_Spi_stopReceiveBuffered(self);
}

bool osg_Spi_isReceiveBufferedEnabled(osg_Spi * self)
{
    return osg_board_Spi_isReceiveBufferedEnabled(self);
}

bool osg_Spi_sendReceiveBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * bufferTx, void * bufferRx, const Size transferSize, const uint32_t timeout)
{
    return osg_board_Spi_sendReceiveBlocking(self, ssPinIndex, bufferTx, bufferRx, transferSize, timeout);
}

uint8_t osg_Spi_countSpis()
{
    return osg_board_Spi_countSpis();
}
