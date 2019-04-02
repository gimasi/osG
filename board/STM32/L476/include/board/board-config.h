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
#ifndef OSG_BOARD_CONFIG_H
#define OSG_BOARD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define USE_HAL_DRIVER
#define USE_STM32L4XX_NUCLEO

#include <stdint.h>

#include <stm32l4xx.h>
#include <stm32l4xx_hal.h>

#include "stm32l4xx_hal_conf.h"

/* @todo: investigate if we must implements these functions:
 * void NMI_Handler(void);
 * void HardFault_Handler(void);
 * void MemManage_Handler(void);
 * void BusFault_Handler(void);
 * void UsageFault_Handler(void);
 * void SVC_Handler(void);
 * void DebugMon_Handler(void);
 * void PendSV_Handler(void);
 * void SysTick_Handler(void);
 */

 #ifdef __cplusplus
}
#endif

#endif
