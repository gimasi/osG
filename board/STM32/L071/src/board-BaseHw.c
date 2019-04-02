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
#include "../../../include/board/board-BaseHw.h"
#include "../include/board/board-config.h"
#include "../../../../osg/include/osg.h"

#include <stm32l0xx_hal.h>

bool osg_board_baseHw_start(osg_BaseHwApplication application)
{
    const bool initOk = osg_board_baseHw_init();
    if (initOk != true) return false;
    application();
    return true;
}

bool osg_board_baseHw_init(void)
{
    const HAL_StatusTypeDef initStatus = HAL_Init();
    if (initStatus != HAL_OK) return false;
    const bool initOk = osg_board_baseHw_systemClockConfig();
    if (initOk == true) return true;
    return false;
}

void osg_board_baseHw_wait(const uint32_t millis)
{
    HAL_Delay(millis);
}

uint32_t osg_board_baseHw_getTick()
{
    return HAL_GetTick();
}

#if !(defined(OSG_OS_NAME)) || (OSG_OS_NAME == OSG_OS_NONE)
// @brief  This function handles SysTick Handler.
// @param  None
// @retval None
void SysTick_Handler(void)
{
    HAL_IncTick();
}
#endif

// F100:
// @brief  System Clock Configuration
//         The system Clock is configured as follow :
//            System Clock source            = PLL (HSE)
//            SYSCLK(Hz)                     = 24000000
//            HCLK(Hz)                       = 24000000
//            AHB Prescaler                  = 1
//            APB1 Prescaler                 = 1
//            APB2 Prescaler                 = 1
//            HSE Frequency(Hz)              = 8000000
//            HSE PREDIV1                    = 2
//            PLLMUL                         = 6
//            Flash Latency(WS)              = 0
// Nucleo L4:
// @brief  System Clock Configuration
//          The system Clock is configured as follows :
//             System Clock source            = PLL (MSI)
//             SYSCLK(Hz)                     = 80000000
//             HCLK(Hz)                       = 80000000
//             AHB Prescaler                  = 1
//             APB1 Prescaler                 = 1
//             APB2 Prescaler                 = 1
//             MSI Frequency(Hz)              = 4000000
//             PLL_M                          = 1
//             PLL_N                          = 40
//             PLL_R                          = 2
//             PLL_P                          = 7
//             PLL_Q                          = 4
//             Flash Latency(WS)              = 4
__weak bool osg_board_baseHw_systemClockConfig(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /**Configure the main internal regulator output voltage 
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
     return false;
    }
    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
     return false;
    }
    
    return true;
}
