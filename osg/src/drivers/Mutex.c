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
#include "../../include/osg/drivers/Mutex.h"
#include "../../include/osg/utils.h"

#ifdef OSG_HAS_CONFIG_FILE
    #include <osgConfig.h>
#else
    #include "../../../templates/osgConfig.h"
#endif

#if !(OSG_OS_NAME == OSG_OS_NONE)
#include "../../../rtos/include/rtos.h"
#endif

void osg_Mutex_ctor(osg_Mutex * self, const osg_MutexConfig * const config)
{
#if !(OSG_OS_NAME == OSG_OS_NONE)
    osg_rtos_Mutex_ctor(self, config);
#endif
}

void osg_Mutex_dtor(osg_Mutex * self)
{
#if !(OSG_OS_NAME == OSG_OS_NONE)
    osg_rtos_Mutex_dtor(self);
#endif
}

bool osg_Mutex_acquire(osg_Mutex * self, const uint32_t timeout)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_Mutex_acquire(self, timeout);
#endif
}

bool osg_Mutex_release(osg_Mutex * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return false;
#else
    return osg_rtos_Mutex_release(self);
#endif
}

void * osg_Mutex_getOwner(osg_Mutex * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return NULL;
#else
    return osg_rtos_Mutex_getOwner(self);
#endif
}

const char * osg_Mutex_getName(osg_Mutex * self)
{
#if (OSG_OS_NAME == OSG_OS_NONE)
    return NULL;
#else
    return osg_rtos_Mutex_getName(self);
#endif
}
