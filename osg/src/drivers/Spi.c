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
#include "osg/drivers/Spi.h"
#include "osg/drivers/Gpio.h"
#include "osg/utils.h"
#include <board.h>

osg_GpioAlternateFunction _osg_board_spi_getGpioAlternateFunction(const osg_SpiId spiId);

void osg_Spi_ctor(osg_Spi * self, osg_SpiId id, osg_SpiMode mode, uint32_t clockSpeed, osg_SpiPolarity polarity, osg_SpiPhase phase, osg_SpiEndianess endianess, osg_SpiDataSize dataSize, osg_SpiSlaveSelectMode ssMode, osg_GpioId sclkPin, osg_GpioId mosiPin, osg_GpioId misoPin, osg_GpioId * ssPinList, Size ssPinListSize, void * circularTxBuffer, Size txBufferSize, void * circularRxBuffer, Size rxBufferSize)
{
    osg_assert(osg_board_gpio_checkPin(sclkPin) == TRUE, "ASSERT FAILED: SPI SCLK Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_gpio_checkPin(mosiPin) == TRUE, "ASSERT FAILED: SPI MOSI Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_gpio_checkPin(misoPin) == TRUE, "ASSERT FAILED: SPI MISO Pin is NOT available on this board. Please check the data sheet.");
    if (ssPinList != NULL)
    {
        for (Size i = 0; i < ssPinListSize; ++i)
        {
            osg_assert(osg_board_gpio_checkPin(ssPinList[i]) == TRUE, "ASSERT FAILED: SPI SS Pin is NOT available on this board. Please check the data sheet.");
        }
    }

    self->id = id;
    self->mode = mode;

    osg_Gpio_ctor(&self->sclkPin, sclkPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_PULLDOWN, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_spi_getGpioAlternateFunction(id));
    osg_Gpio_ctor(&self->mosiPin, mosiPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_PULLDOWN, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_spi_getGpioAlternateFunction(id));
    osg_Gpio_ctor(&self->misoPin, misoPin, OSG_GPIO_MODE_AF_PP, OSG_GPIO_PULLDOWN, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_spi_getGpioAlternateFunction(id));
    self->ssPinList = ssPinList;
    self->ssPinListSize = ssPinListSize;

    if (circularRxBuffer != NULL && rxBufferSize > 0)
        osg_CircularFifo_ctor(&self->rxBuffer, circularRxBuffer, rxBufferSize, OSG_FIFO_OVERWRITE_NO_ERROR);

    if (circularTxBuffer != NULL && txBufferSize > 0)
        osg_CircularFifo_ctor(&self->txBuffer, circularTxBuffer, txBufferSize, OSG_FIFO_ERROR);

    osg_board_spi_ctor(self, clockSpeed, polarity, phase, endianess, dataSize, ssMode);
}

void osg_Spi_dtor(osg_Spi * self)
{
    osg_Gpio_dtor(&self->sclkPin);
    osg_Gpio_dtor(&self->mosiPin);
    osg_Gpio_dtor(&self->misoPin);

    osg_board_spi_dtor(self);

    osg_CircularFifo_dtor(&self->rxBuffer);
    osg_CircularFifo_dtor(&self->txBuffer);

    self->isConfigured = FALSE;
}

Bool osg_Spi_sendBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout)
{
    return osg_board_spi_sendBlocking(self, ssPinIndex, buffer, bufferSize, timeout);
}

Bool osg_Spi_receiveBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout)
{
    return osg_board_spi_receiveBlocking(self, ssPinIndex, buffer, bufferSize, timeout);
}

Bool osg_Spi_sendNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    return osg_board_spi_sendNonBlocking(self, ssPinIndex, buffer, bufferSize, callback);
}

Bool osg_Spi_receiveNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    return osg_board_spi_receiveNonBlocking(self, ssPinIndex, buffer, bufferSize, callback);
}

Size osg_Spi_sendBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    return osg_board_spi_sendBuffered(self, ssPinIndex, buffer, bufferSize, callback);
}

Size osg_Spi_receivedBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize)
{
    return osg_board_spi_receivedBuffered(self, ssPinIndex, buffer, bufferSize);
}

Size osg_Spi_getSizeOfReceivedData(osg_Spi * self)
{
    return osg_board_spi_getSizeOfReceivedData(self);
}
