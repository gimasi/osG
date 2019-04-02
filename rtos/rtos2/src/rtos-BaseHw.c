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
#include "../../include/rtos/rtos-BaseHw.h"
#include "../include/rtos/rtos-config.h"
#include "../../../osg/include/osg.h"
#include "../../../board/include/board.h"

bool osg_rtos_baseHw_start(osg_BaseHwApplication application)
{
    // Init board
    const bool initOk = osg_board_baseHw_init();
    if (initOk != true) return false;
    // Initialize CMSIS-RTOS2
    const osStatus_t initKerOk = osKernelInitialize();
    if (initKerOk != osOK) return false;
    // Create application main thread
    void * handler = osThreadNew((osThreadFunc_t)application, NULL, NULL);
    if (handler == NULL) return false;
    // Start thread execution
    const osStatus_t startOk = osKernelStart();
    if (startOk != osOK) return false;

    return true;
}

bool osg_rtos_baseHw_startCustom(osg_BaseHwApplication application, const osg_BaseHwConfig * attributes)
{
    // Init board
    const bool initOk = osg_board_baseHw_init();
    if (initOk != true) return false;
    // Initialize CMSIS-RTOS2
    const osStatus_t initKerOk = osKernelInitialize();
    if (initKerOk != osOK) return false;
    // cast attributes
    const osThreadAttr_t applAttr = {
        .stack_size = attributes->stackSize,
        .name = attributes->name
    };
    // Create application main thread
    void * handler = osThreadNew((osThreadFunc_t)application, NULL, &applAttr);
    if (handler == NULL) return false;
    // Start thread execution
    const osStatus_t startOk = osKernelStart();
    if (startOk != osOK) return false;

    return true;
}

void osg_rtos_baseHw_wait(const uint32_t millis)
{
    osStatus_t status = osDelay (millis);
}

uint32_t osg_rtos_baseHw_getTick()
{
    return osKernelGetTickCount();
}

#if defined(OSG_OS_NAME) && (OSG_OS_NAME == OSG_OS_CMSIS_RTOS2)
// Add HAL_GetTick function for STM32Cube HAL
uint32_t HAL_GetTick(void) {
    return ((uint32_t)osKernelGetTickCount ());
}
#endif
