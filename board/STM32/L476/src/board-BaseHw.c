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

#include <stm32l4xx_hal.h>

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
    // @todo Not in f100... what does it do?
    //HAL_PWR_EnableBkUpAccess();

    RCC_OscInitTypeDef oscinitstruct = {0};
    // @todo Make parameters
# if 0
    // From f100 example:
    // Enable HSE Oscillator and activate PLL with HSE as source
    oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    oscinitstruct.HSEState        = RCC_HSE_ON;
    oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
    oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
    oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
    oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL6;
#elif 0
    // From hglogger example:
    /* MSI is enabled after System reset, activate PLL with MSI as source */
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_MSI|RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.LSEState = RCC_LSE_ON;
    oscinitstruct.HSI48State = RCC_HSI48_ON;
    oscinitstruct.MSIState = RCC_MSI_ON;
    oscinitstruct.MSIClockRange = RCC_MSIRANGE_6;
    oscinitstruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    oscinitstruct.PLL.PLLState = RCC_PLL_ON;
    oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    oscinitstruct.PLL.PLLM = 1;
    oscinitstruct.PLL.PLLN = 40;
    oscinitstruct.PLL.PLLR = 2;
    oscinitstruct.PLL.PLLP = 7;
    oscinitstruct.PLL.PLLQ = 4;
#else
    // From nucleo l476rg example:
    /* MSI is enabled after System reset, activate PLL with MSI as source *
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    oscinitstruct.MSIState = RCC_MSI_ON;
    oscinitstruct.MSIClockRange = RCC_MSIRANGE_6;
    oscinitstruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    //oscinitstruct.PLL.PLLState = RCC_PLL_NONE;
    oscinitstruct.PLL.PLLState = RCC_PLL_ON;
    oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    oscinitstruct.PLL.PLLM = 1;
    oscinitstruct.PLL.PLLN = 40;
    oscinitstruct.PLL.PLLR = 2;
    oscinitstruct.PLL.PLLP = 7;
    oscinitstruct.PLL.PLLQ = 4;
    */
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    oscinitstruct.HSIState = RCC_HSI_ON;
    oscinitstruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    oscinitstruct.PLL.PLLState = RCC_PLL_ON;
    oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    oscinitstruct.PLL.PLLM = 1;
    oscinitstruct.PLL.PLLN = 10;
    oscinitstruct.PLL.PLLP = RCC_PLLP_DIV7;
    oscinitstruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    oscinitstruct.PLL.PLLR = RCC_PLLR_DIV2;
#endif

    if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
    {
        // Initialization Error
        return false;
    }

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitTypeDef clkinitstruct = {0};
    clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI/*RCC_SYSCLKSOURCE_PLLCLK*/;
    clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
    clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;

    // @todo For stm32cubef100 is FLASH_LATENCY_0
    if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_4)!= HAL_OK)
    {
        // Initialization Error
        return false;
    }

#if 0
    // @todo: from hglogger example:
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        return false;
    }
#endif

    //RCC_PeriphCLKInitTypeDef PeriphClkInit;
    //PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    //PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    //if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    //{
    //    // Initialization Error
    //    return false;
    //}

    // Configure the main internal regulator output voltage
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        return false;
    }

    // Configure the Systick interrupt time
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    // Configure the Systick
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // SysTick_IRQn interrupt configuration
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    SystemCoreClockUpdate();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    return true;
}
