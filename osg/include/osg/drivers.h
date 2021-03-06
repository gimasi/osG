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
#ifndef OSG_DRIVERS_H
#define OSG_DRIVERS_H

#include "drivers/Gpio.h"

#if OSG_USE_I2C == 1
#include "drivers/I2c.h"
#endif

#if OSG_USE_LED == 1
#include "drivers/Led.h"
#endif

#if OSG_OS_USE_MESSAGES == 1
#include "drivers/MessageQueue.h"
#endif

#if OSG_OS_USE_MUTEX == 1
#include "drivers/Mutex.h"
#endif

#if OSG_USE_SPI == 1
#include "drivers/Spi.h"
#endif

#if OSG_OS_USE_THREAD == 1
#include "drivers/Thread.h"
#endif

#if (OSG_USE_TIMER == 1) || (OSG_OS_USE_TIMER == 1)
#include "drivers/Timer.h"
#endif

#if OSG_USE_UART == 1
#include "drivers/Uart.h"
#endif

#endif
