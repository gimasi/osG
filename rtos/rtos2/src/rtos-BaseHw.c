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
#include "../include/rtos/rtos-BaseHw.h"
#include "../include/rtos/rtos-config.h"
#include <osg.h>
#include <board.h>

Bool osg_rtos_baseHw_start(osg_BaseHwApplication application)
{
    // Init board
    const Bool initOk = osg_board_baseHw_init();
    if (initOk != TRUE) return FALSE;
    // Initialize CMSIS-RTOS2
    const osStatus_t initKerOk = osKernelInitialize();
    if (initKerOk != osOK) return FALSE;
    // Create application main thread
    void * handler = osThreadNew((osThreadFunc_t)application, NULL, NULL);
    if (handler == NULL) return FALSE;
    // Start thread execution
    const osStatus_t startOk = osKernelStart();
    if (startOk != osOK) return FALSE;

    return TRUE;
}

void osg_rtos_baseHw_wait(const uint32_t millis)
{
    // @TODO
}
