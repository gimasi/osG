#include "osg-all-in-one.h"

#include "packages/osg_package.c"
#include "packages/board_package.c"

#if defined OSG_OS_NAME && OSG_OS_NAME
    #include "packages/rtos_package.c"
#endif
