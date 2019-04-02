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
#ifndef OSG_BOARD_DRIVERS_H
#define OSG_BOARD_DRIVERS_H

#include "drivers/board-Gpio.h"
#if OSG_USE_LED == 1
#include "drivers/board-Led.h"
#endif
#if OSG_USE_TIMER == 1
#include "drivers/board-Timer.h"
#endif
#if OSG_USE_I2C == 1
#include "drivers/board-I2c.h"
#endif
#if OSG_USE_SPI == 1
#include "drivers/board-Spi.h"
#endif
#if OSG_USE_UART == 1
#include "drivers/board-Uart.h"
#endif

#endif
