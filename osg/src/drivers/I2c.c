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
#include "osg/drivers/I2c.h"
#include "osg/drivers/Gpio.h"
#include "osg/utils.h"
#include <board.h>

osg_GpioAlternateFunction _osg_board_I2c_getGpioAlternateFunction(const osg_I2cId i2cId);

void osg_I2c_ctor(osg_I2c * self,
                  const osg_I2cId id,
                  const osg_I2cMode mode,
                  const uint32_t clockSpeed,
                  const osg_I2cAddressingMode addressingMode,
                  const Bool enableGeneralCall,
                  const uint16_t ownAddress,
                  void * circularTxBuffer,
                  Size txBufferSize,
                  void * circularRxBuffer,
                  Size rxBufferSize,
                  const osg_GpioId sdaPin,
                  const osg_GpioId sclPin)
{
    osg_assert(osg_board_gpio_checkPin(sdaPin) == TRUE, "ASSERT FAILED: I2C SDA Pin is NOT available on this board. Please check the data sheet.");
    osg_assert(osg_board_gpio_checkPin(sclPin) == TRUE, "ASSERT FAILED: I2C SCL Pin is NOT available on this board. Please check the data sheet.");

    self->id = id;
    self->mode = mode;

    osg_Gpio_ctor(&self->sclPin, sclPin, OSG_GPIO_MODE_AF_OD, OSG_GPIO_PULLUP, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_I2c_getGpioAlternateFunction(id));
    osg_Gpio_ctor(&self->sdaPin, sdaPin, OSG_GPIO_MODE_AF_OD, OSG_GPIO_PULLUP, OSG_GPIO_SPEED_FREQ_VERY_HIGH, _osg_board_I2c_getGpioAlternateFunction(id));

    osg_board_I2c_ctor(self, clockSpeed, addressingMode, enableGeneralCall, ownAddress, circularTxBuffer, txBufferSize, circularRxBuffer, rxBufferSize);
}

void osg_I2c_dtor(osg_I2c * self)
{
    osg_Gpio_dtor(&self->sclPin);
    osg_Gpio_dtor(&self->sdaPin);

    osg_board_I2c_dtor(self);
}

Bool osg_I2c_sendBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout)
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

Bool osg_I2c_sendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
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

Bool osg_I2c_sendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize)
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

void osg_I2c_setBufferedCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx)
{
    osg_board_I2c_setBufferedCallbacks(self, tx, rx);
}

void osg_I2c_setNbCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx)
{
    osg_board_I2c_setNbCallbacks(self, tx, rx);
}

Bool osg_I2c_receiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout)
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

Bool osg_I2c_receiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
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

Bool osg_I2c_isReceiveBufferedEnabled(osg_I2c * self)
{
    return osg_board_I2c_isReceiveBufferedEnabled(self);
}
