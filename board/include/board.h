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
#ifndef OSG_BOARD_H
#define OSG_BOARD_H

#ifdef OSG_HAS_CONFIG_FILE
#include <osgConfig.h>
#else
#include "../../templates/osgConfig.h"
#endif

/// @defgroup Board Board
/// The internal low-level API

/// @defgroup Board-Base Board-Base
/// @ingroup Board
/// General data types and base HW methods
#include "board/board-forwards.h"
#include "board/board-BaseHw.h"
/// @defgroup Board-Drivers Board-Drivers
/// @ingroup Board
/// Hardware driver classes
#include "board/board-drivers.h"

// Select the correct board
#if defined MCU_NAME && MCU_NAME == STM32_L476
    #include "../STM32/L476/include/board/board-config.h"
    //#include "../STM32/L476/include/board/stm32l4xx_hal_conf.h"
    #include "../STM32/L476/src/drivers/board-GpioSupport.h"
    #if OSG_USE_I2C == 1
        #include "../STM32/L476/src/drivers/board-I2cSupport.h"
    #endif
    #if OSG_USE_SPI == 1
        #include "../STM32/L476/src/drivers/board-SpiSupport.h"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../STM32/L476/src/drivers/board-TimerSupport.h"
    #endif
    #if OSG_USE_UART == 1
        #include "../STM32/L476/src/drivers/board-UartSupport.h"
    #endif
#elif defined MCU_NAME && MCU_NAME == STM32_L4R5
    #include "../STM32/L4R5/include/board/board-config.h"
    #include "../STM32/L4R5/src/drivers/board-GpioSupport.h"
    #if OSG_USE_I2C == 1
        #include "../STM32/L4R5/src/drivers/board-I2cSupport.h"
    #endif
    #if OSG_USE_SPI == 1
        #include "../STM32/L4R5/src/drivers/board-SpiSupport.h"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../STM32/L4R5/src/drivers/board-TimerSupport.h"
    #endif
    #if OSG_USE_UART == 1
        #include "../STM32/L4R5/src/drivers/board-UartSupport.h"
    #endif
#elif defined MCU_NAME && MCU_NAME == STM32_L071
    #include "../STM32/L071/include/board/board-config.h"
    //#include "../STM32/L071/include/board/stm32l4xx_hal_conf.h"
    #include "../STM32/L071/src/drivers/board-GpioSupport.h"
    #if OSG_USE_I2C == 1
        #include "../STM32/L071/src/drivers/board-I2cSupport.h"
    #endif
    #if OSG_USE_SPI == 1
        #include "../STM32/L071/src/drivers/board-SpiSupport.h"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../STM32/L071/src/drivers/board-TimerSupport.h"
    #endif
    #if OSG_USE_UART == 1
        #include "../STM32/L071/src/drivers/board-UartSupport.h"
    #endif
#elif defined MCU_NAME && MCU_NAME == NXP_MK28F15
    #include "../NXP/MK28FN2M0VMI15/include/board/board-config.h"
    #include "../NXP/MK28FN2M0VMI15/src/drivers/board-GpioSupport.h"
    #if OSG_USE_I2C == 1
        #include "../NXP/MK28FN2M0VMI15/src/drivers/board-I2cSupport.h"
    #endif
    #if OSG_USE_SPI == 1
        #include "../NXP/MK28FN2M0VMI15/src/drivers/board-SpiSupport.h"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../NXP/MK28FN2M0VMI15/src/drivers/board-TimerSupport.h"
    #endif
    #if OSG_USE_UART == 1
        #include "../NXP/MK28FN2M0VMI15/src/drivers/board-UartSupport.h"
    #endif
#else
    #error "INVALID MCU. Check the 'osgConfig.h' file."
#endif

#endif
