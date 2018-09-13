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
#include "osg/BaseHw.h"
#include <board.h>
#ifdef OSG_USE_RTOS
#include <rtos.h>
#endif


Bool osg_baseHw_start(osg_BaseHwApplication application)
{
#ifdef OSG_USE_RTOS
    return osg_rtos_baseHw_start(application);
#else
    return osg_board_baseHw_start(application);
#endif
}

void osg_baseHw_wait(const uint32_t millis)
{
#ifdef OSG_USE_RTOS
    osg_rtos_baseHw_wait(millis);
#else
    osg_board_baseHw_wait(millis);
#endif
}
