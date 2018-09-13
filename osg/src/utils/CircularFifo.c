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
#include "osg/utils/CircularFifo.h"

#define INC(var) (((var) + 1) % self->size)
#define SELF_INC(var) ((var) = INC(var))
#define ADD(var,pos) ((var) + (pos)) % self->size)

void osg_CircularFifo_ctor(osg_CircularFifo * self,
                           void * buffer,
                           const Size size,
                           const osg_CircularFifoBehavior behavior)
{
    self->buffer = buffer;
    self->size = size;
    self->readPosition = 0;
    self->writePosition = 0;
    self->behavior = behavior;
    self->currentSize = 0;

    if (size == 0 || buffer == NULL)
    {
        self->buffer = NULL;
        self->size = 0;
    }
}

void osg_CircularFifo_dtor(osg_CircularFifo * self)
{
    self->buffer = NULL;
    self->size = 0;
    self->readPosition = 0;
    self->writePosition = 0;
    // self->behavior = OSG_FIFO_OVERWRITE;
    self->currentSize = 0;
}

Bool osg_CircularFifo_isEmpty(const osg_CircularFifo * self)
{
    return osg_bool(self->currentSize == 0);
}

Bool osg_CircularFifo_isFull(const osg_CircularFifo * self)
{
    return osg_bool(self->currentSize == self->size);
}

Bool osg_CircularFifo_isNull(const osg_CircularFifo * self)
{
    if (self->buffer != NULL)
        return FALSE;

    return TRUE;
}

Size osg_CircularFifo_getSize(const osg_CircularFifo * self)
{
    return self->currentSize;
}

Size osg_CircularFifo_getFreeSize(const osg_CircularFifo * self)
{
    return self->size - self->currentSize;
}

void osg_CircularFifo_clear(osg_CircularFifo * self)
{
    self->readPosition = 0;
    self->writePosition = 0;
    self->currentSize = 0;
}

Bool osg_CircularFifo_pushByte(osg_CircularFifo * self, const Byte byte)
{
    const Bool isFull = osg_CircularFifo_isFull(self);
    if (isFull)
    {
        switch (self->behavior)
        {
            case OSG_FIFO_OVERWRITE: if (self->size < 1) return FALSE; break;
            case OSG_FIFO_OVERWRITE_NO_ERROR: break;
            case OSG_FIFO_ERROR: return FALSE;
            case OSG_FIFO_FILL: return FALSE;
        }
    }

    self->buffer[self->writePosition] = byte;
    SELF_INC(self->writePosition);
    if (isFull)
        SELF_INC(self->readPosition);
    else
        self->currentSize += 1;

    return TRUE;
}

Bool osg_CircularFifo_popByte(osg_CircularFifo * self, Byte * byte)
{
    if (osg_CircularFifo_isEmpty(self)) return FALSE;

    *byte = self->buffer[self->readPosition];
    self->currentSize -= 1;
    SELF_INC(self->readPosition);
    return TRUE;
}

Size osg_CircularFifo_pushBuffer(osg_CircularFifo * self, const void * buffer, const Size size)
{
    if (osg_CircularFifo_isNull(self)) return 0;
    Size bytesToWrite = size;
    const Byte * buf = buffer;
    const Size freeSlots = self->size - self->currentSize;
    const Bool isOverwrite = size > freeSlots ? TRUE : FALSE;
    if (isOverwrite)
    {
        switch (self->behavior)
        {
            case OSG_FIFO_OVERWRITE:
                if (size > self->size) return 0;
                break;
            case OSG_FIFO_OVERWRITE_NO_ERROR:
                if (size > self->size)
                {
                    bytesToWrite = self->size;
                    buf = buf + (size - self->size);
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
        self->buffer[self->writePosition] = buf[i];
        SELF_INC(self->writePosition);
    }

    if (isOverwrite)
    {
        self->readPosition = self->writePosition;
        self->currentSize = self->size;
    }
    else
    {
        self->currentSize += size;
    }

    return bytesToWrite;
}

Size osg_CircularFifo_popBuffer(osg_CircularFifo * self, void * buffer, const Size size)
{
    const Bool isFullRead = size >= self->currentSize ? TRUE : FALSE;
    const Size bytesToRead = isFullRead? self->currentSize : size;
    Byte * buf = buffer;

    for (Size i = 0; i < bytesToRead; ++i)
    {
        buf[i] = self->buffer[self->readPosition];
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
        const Bool isRead = osg_CircularFifo_popByte(self, &b);
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

    *buffer = self->buffer + self->readPosition;
    if (self->writePosition >= self->readPosition)
    {
        *size = self->writePosition - self->readPosition;
    }
    else
    {
        *size = self->size - self->readPosition;
    }
}
