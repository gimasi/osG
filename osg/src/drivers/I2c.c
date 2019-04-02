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
#include "../../include/osg/drivers/I2c.h"
#include "../../include/osg/drivers/Gpio.h"
#include "../../include/osg/utils.h"
#include "../../../board/include/board.h"

void osg_I2c_ctor(osg_I2c * self, const osg_I2cConfig * const config)
{
    osg_assert(osg_board_Gpio_checkPin(config->sdaPin) == true, "ASSERT FAILED: I2C SDA Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_Gpio_checkPin(config->sclPin) == true, "ASSERT FAILED: I2C SCL Pin is NOT available on this board. Please check the data sheet.");

    self->id = config->id;
    self->mode = config->mode;

	osg_GpioConfig gpioConfig;
	memset(&gpioConfig, 0, sizeof(gpioConfig));
	gpioConfig.mode = OSG_GPIO_MODE_AF_OD;
	gpioConfig.pull = OSG_GPIO_PULLUP;
	gpioConfig.speed = OSG_GPIO_SPEED_FREQ_VERY_HIGH;
	gpioConfig.alternate = config->alternate;
		
	gpioConfig.id = config->sclPin;
    osg_Gpio_ctor(&self->sclPin, &gpioConfig);
	gpioConfig.id = config->sdaPin;
    osg_Gpio_ctor(&self->sdaPin, &gpioConfig);

    osg_board_I2c_ctor(self, config);
}

void osg_I2c_dtor(osg_I2c * self)
{
    osg_Gpio_dtor(&self->sclPin);
    osg_Gpio_dtor(&self->sdaPin);

    osg_board_I2c_dtor(self);
}

void osg_I2c_setBuffers(osg_I2c * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize)
{
    osg_board_I2c_setBuffers(self, circularTxBuffer, txBufferSize, circularRxBuffer, rxBufferSize);
}

void osg_I2c_setBufferedCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx)
{
    osg_board_I2c_setBufferedCallbacks(self, tx, rx);
}

void osg_I2c_setNbCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx)
{
    osg_board_I2c_setNbCallbacks(self, tx, rx);
}

bool osg_I2c_sendBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_masterSendBlocking(self, slaveAddress, buffer, size, timeout);
    }
    else
    {
        return osg_board_I2c_slaveSendBlocking(self, buffer, size, timeout);
    }
}

bool osg_I2c_sendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_masterSendNonBlocking(self, slaveAddress, buffer, size);
    }
    else
    {
        return osg_board_I2c_slaveSendNonBlocking(self, buffer, size);
    }
}

bool osg_I2c_sendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_masterSendBuffered(self, slaveAddress, buffer, bufferSize);
    }
    else
    {
        return osg_board_I2c_slaveSendBuffered(self, buffer, bufferSize);
    }
}

bool osg_I2c_receiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_masterReceiveBlocking(self, slaveAddress, buffer, size, timeout);
    }
    else
    {
        return osg_board_I2c_slaveReceiveBlocking(self, buffer, size, timeout);
    }
}

bool osg_I2c_receiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_masterReceiveNonBlocking(self, slaveAddress, buffer, size);
    }
    else
    {
        return osg_board_I2c_slaveReceiveNonBlocking(self, buffer, size);
    }
}

Size osg_I2c_receiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_masterReceiveBuffered(self, buffer, bufferSize);
    }
    else
    {
        return osg_board_I2c_slaveReceiveBuffered(self, buffer, bufferSize);
    }
}

void osg_I2c_startReceiveBuffered(osg_I2c * self, const uint16_t slaveAddress)
{
    osg_board_I2c_startReceiveBuffered(self, slaveAddress);
}

void osg_I2c_stopReceiveBuffered(osg_I2c * self)
{
    osg_board_I2c_stopReceiveBuffered(self);
}

bool osg_I2c_isReceiveBufferedEnabled(osg_I2c * self)
{
    return osg_board_I2c_isReceiveBufferedEnabled(self);
}

bool osg_I2c_writeMemBlocking(osg_I2c * self, const uint16_t slaveAddress, const uint16_t memAddress, const uint16_t memAddrSize, void * buffer, const Size size, const uint32_t timeout)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_writeMemBlocking(self, slaveAddress, memAddress, memAddrSize, buffer, size, timeout);
    }
    else
    {
        return false;
    }

}

bool osg_I2c_readMemBlocking(osg_I2c * self, const uint16_t slaveAddress, const uint16_t memAddress, const uint16_t memAddrSize, void * buffer, const Size size, const uint32_t timeout)
{
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        return osg_board_I2c_readMemBlocking(self, slaveAddress, memAddress, memAddrSize, buffer, size, timeout);
    }
    else
    {
        return false;
    }

}

uint8_t osg_I2c_countI2cs()
{
    return osg_board_I2c_countI2cs();
}
