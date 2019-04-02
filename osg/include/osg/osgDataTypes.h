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
#ifndef OSG_OSGDATATYPES_H
#define OSG_OSGDATATYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Convenience size type
/// @ingroup Base
typedef size_t Size;

/// @brief Convenience byte type
/// @ingroup Base
typedef uint8_t Byte;

enum osg_IrqPriority;
typedef enum osg_IrqPriority osg_IrqPriority;
/// @brief Interrupt prioirty.
/// OSG_IRQ_PRIORITY_0 is the highest priority.
/// @ingroup Base
enum osg_IrqPriority
{
    OSG_IRQ_PRIORITY_0,
    OSG_IRQ_PRIORITY_1,
    OSG_IRQ_PRIORITY_2,
    OSG_IRQ_PRIORITY_3,
    OSG_IRQ_PRIORITY_4,
    OSG_IRQ_PRIORITY_5,
    OSG_IRQ_PRIORITY_6,
    OSG_IRQ_PRIORITY_7,
    OSG_IRQ_PRIORITY_8,
    OSG_IRQ_PRIORITY_9,
    OSG_IRQ_PRIORITY_10,
    OSG_IRQ_PRIORITY_11,
    OSG_IRQ_PRIORITY_12,
    OSG_IRQ_PRIORITY_13,
    OSG_IRQ_PRIORITY_14,
    OSG_IRQ_PRIORITY_15
};

enum osg_PinMax;
typedef enum osg_PinMax osg_PinMax;
/// @brief Max Pin enumeration of a single port.
/// @ingroup Base
enum osg_PinMax
{
    OSG_16_PIN_PER_PORT = 16,
    OSG_32_PIN_PER_PORT = 32
};

#ifdef __cplusplus
}
#endif

#endif
