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
#include "../../include/osg/utils/CircularFifo.h"

#define INC(var) (((var) + 1) % self->config.size)
#define SELF_INC(var) ((var) = INC(var))
#define ADD(var,pos) ((var) + (pos)) % self->config.size)

void osg_CircularFifo_ctor(osg_CircularFifo * self,
                           const osg_CircularFifoConfig * const config)
{
    self->config = *config;
    self->readPosition = 0;
    self->writePosition = 0;
    self->currentSize = 0;

    if (config->size == 0 || config->buffer == NULL)
    {
        self->config.buffer = NULL;
        self->config.size = 0;
    }
}

void osg_CircularFifo_dtor(osg_CircularFifo * self)
{
    self->config.buffer = NULL;
    self->config.size = 0;
    self->readPosition = 0;
    self->writePosition = 0;
    // self->config.behavior = OSG_FIFO_OVERWRITE;
    self->currentSize = 0;
}

bool osg_CircularFifo_isEmpty(const osg_CircularFifo * self)
{
    return osg_bool(self->currentSize == 0);
}

bool osg_CircularFifo_isFull(const osg_CircularFifo * self)
{
    return osg_bool(self->currentSize == self->config.size);
}

bool osg_CircularFifo_isNull(const osg_CircularFifo * self)
{
    return osg_bool(self->config.buffer == NULL);
}

Size osg_CircularFifo_getSize(const osg_CircularFifo * self)
{
    return self->currentSize;
}

Size osg_CircularFifo_getFreeSize(const osg_CircularFifo * self)
{
    return self->config.size - self->currentSize;
}

void osg_CircularFifo_clear(osg_CircularFifo * self)
{
    self->readPosition = 0;
    self->writePosition = 0;
    self->currentSize = 0;
}

bool osg_CircularFifo_pushByte(osg_CircularFifo * self, const Byte byte)
{
    const bool isFull = osg_CircularFifo_isFull(self);
    if (isFull)
    {
        switch (self->config.behavior)
        {
            case OSG_FIFO_OVERWRITE: if (self->config.size < 1) return false; break;
            case OSG_FIFO_OVERWRITE_NO_ERROR: break;
            case OSG_FIFO_ERROR: return false;
            case OSG_FIFO_FILL: return false;
        }
    }

    self->config.buffer[self->writePosition] = byte;
    SELF_INC(self->writePosition);
    if (isFull)
        SELF_INC(self->readPosition);
    else
        self->currentSize += 1;

    return true;
}

bool osg_CircularFifo_popByte(osg_CircularFifo * self, Byte * byte)
{
    if (osg_CircularFifo_isEmpty(self)) return false;

    *byte = self->config.buffer[self->readPosition];
    self->currentSize -= 1;
    SELF_INC(self->readPosition);
    return true;
}

Size osg_CircularFifo_pushBuffer(osg_CircularFifo * self, const void * buffer, const Size size)
{
    if (osg_CircularFifo_isNull(self)) return 0;
    Size bytesToWrite = size;
    const Byte * buf = buffer;
    const Size freeSlots = self->config.size - self->currentSize;
    const bool isOverwrite = size > freeSlots ? true : false;
    if (isOverwrite)
    {
        switch (self->config.behavior)
        {
            case OSG_FIFO_OVERWRITE:
                if (size > self->config.size) return 0;
                break;
            case OSG_FIFO_OVERWRITE_NO_ERROR:
                if (size > self->config.size)
                {
                    bytesToWrite = self->config.size;
                    buf = buf + (size - self->config.size);
                }
                break;
            case OSG_FIFO_ERROR:
                return 0;
            case OSG_FIFO_FILL:
                bytesToWrite = freeSlots;
                break;
        }
    }

    for (Size i = 0; i < bytesToWrite; ++i)
    {
        self->config.buffer[self->writePosition] = buf[i];
        SELF_INC(self->writePosition);
    }

    if (isOverwrite)
    {
        self->readPosition = self->writePosition;
        self->currentSize = self->config.size;
    }
    else
    {
        self->currentSize += size;
    }

    return bytesToWrite;
}

Size osg_CircularFifo_popBuffer(osg_CircularFifo * self, void * buffer, const Size size)
{
    const bool isFullRead = size >= self->currentSize ? true : false;
    const Size bytesToRead = isFullRead? self->currentSize : size;
    Byte * buf = buffer;

    for (Size i = 0; i < bytesToRead; ++i)
    {
        buf[i] = self->config.buffer[self->readPosition];
        SELF_INC(self->readPosition);
    }

    self->currentSize -= bytesToRead;

    return bytesToRead;
}

Size osg_CircularFifo_popBytes(osg_CircularFifo * self, const Size size)
{
    Size ret = 0;
    Byte b = 0;
    for (ret = 0; ret < size; ++ret)
    {
        const bool isRead = osg_CircularFifo_popByte(self, &b);
        if (!isRead) return ret;
    }

    return ret;
}

void osg_CircularFifo_getFilledBuffer(osg_CircularFifo * self, void ** buffer, Size * size)
{
    if (osg_CircularFifo_isEmpty(self))
    {
        *buffer = NULL;
        *size = 0;
        return;
    }

    *buffer = self->config.buffer + self->readPosition;
    if (self->writePosition >= self->readPosition)
    {
        *size = self->writePosition - self->readPosition;
    }
    else
    {
        *size = self->config.size - self->readPosition;
    }
}
