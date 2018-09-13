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
#ifndef OSG_UTILS_CIRCULARFIFO_H
#define OSG_UTILS_CIRCULARFIFO_H

#include "../osgDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup CircularFifo CircularFifo
/// @ingroup Utils

enum osg_CircularFifoBehavior;
typedef enum osg_CircularFifoBehavior osg_CircularFifoBehavior;
/// @brief The CircularFifo behavior in case of insertion into the FIFO.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
enum osg_CircularFifoBehavior
{
    /// In case required data cannot be completely inserted in the free space,
    /// overwrite already used slots. Please note that in case of insertion of
    /// buffers greater than the FIFO internal buffer, the insertion routine
    /// will fail nevertheless, since usually this denotes an application bug.
    OSG_FIFO_OVERWRITE,
    /// In case required data cannot be completely inserted in the free space,
    /// overwrite already used slots. Please note that in case of insertion of
    /// buffers greater than the FIFO internal buffer, the insertion routine
    /// will store only the last bytes of the buffer.
    OSG_FIFO_OVERWRITE_NO_ERROR,
    /// In case required data cannot be completely inserted in the free space,
    /// do not insert any data and return with error.
    OSG_FIFO_ERROR,
    /// In case required data cannot be completely inserted in the free space,
    /// writes only the data to fill the free slots
    OSG_FIFO_FILL
};

struct osg_CircularFifo;
typedef struct osg_CircularFifo osg_CircularFifo;

/// @brief The CircularFifo class.
/// Implements a FIFO, whose internal data is managed as a circular buffer.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
struct osg_CircularFifo
{
    /// @brief The internal buffer
    /// @private
    Byte * buffer;
    /// @brief The internal buffer size
    /// @private
    Size size;
    /// @brief The first available index for reading
    /// @private
    Size readPosition;
    /// @brief The first available index for writing
    /// @private
    Size writePosition;
    /// @brief The FIFO insertion behavior
    /// @private
    osg_CircularFifoBehavior behavior;
    /// @brief Tracks current data size.
    /// @private
    Size currentSize;
};
/// @brief Constructor.
/// Please note that the passed buffer must remain valid as long as
/// the CircularFifo instance is used.
/// This class does not have the buffer ownership.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param buffer The buffer
/// @param size The buffer size
/// @param behavior The preferred behavior in case of data insertion.
void osg_CircularFifo_ctor(osg_CircularFifo * self,
                           void * buffer,
                           const Size size,
                           const osg_CircularFifoBehavior behavior);
/// @brief Destructor.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
void osg_CircularFifo_dtor(osg_CircularFifo * self);
/// @brief Checks whether the FIFO is empty
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @return True if empty.
Bool osg_CircularFifo_isEmpty(const osg_CircularFifo * self);
/// @brief Checks whether the FIFO is full
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @return True if full.
Bool osg_CircularFifo_isFull(const osg_CircularFifo * self);
/// @brief Checks whether the FIFO buffer is not initialized.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @return TRUE whether the buffer is NULL.
Bool osg_CircularFifo_isNull(const osg_CircularFifo * self);
/// @brief Gets the number of currently stored bytes
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @return The current FIFO size
Size osg_CircularFifo_getSize(const osg_CircularFifo * self);
/// @brief Gets the amount of free space.
Size osg_CircularFifo_getFreeSize(const osg_CircularFifo * self);
/// @brief Clears the FIFO content.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
void osg_CircularFifo_clear(osg_CircularFifo * self);
/// @brief Inserts a byte into the FIFO
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param byte The byte to be inserted
/// @return TRUE in case of successful insertion
Bool osg_CircularFifo_pushByte(osg_CircularFifo * self, const Byte byte);
/// @brief Extracts a byte from the FIFO
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param byte Where to store the extracted byte
/// @return TRUE in case of successful extraction
Bool osg_CircularFifo_popByte(osg_CircularFifo * self, Byte * byte);
/// @brief Inserts a buffer into the FIFO
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param buffer The buffer to be inserted
/// @param size The buffer size
/// @return The number of inserted bytes
Size osg_CircularFifo_pushBuffer(osg_CircularFifo * self, const void * buffer, const Size size);
/// @brief Extracts a sequence of bytes from the FIFO
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param buffer Where to save the extracted bytes
/// @param size The buffer size
/// @return The number of extracted bytes
Size osg_CircularFifo_popBuffer(osg_CircularFifo * self, void * buffer, const Size size);
/// @brief Remove a sequence of bytes from the FIFO.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param size The number of bytes to remove from FIFO.
/// @return The number of removed bytes.
Size osg_CircularFifo_popBytes(osg_CircularFifo * self, const Size size);
/// @brief Store in a buffer the first bytes without pop them.
/// @ingroup CircularFifo
/// @memberof osg_CircularFifo
/// @param self The object.
/// @param buffer A pointer to the first element available in the FIFO.
/// @param size The amount of bytes available.
void osg_CircularFifo_getFilledBuffer(osg_CircularFifo * self, void ** buffer, Size * size);


#ifdef __cplusplus
}
#endif

#endif
