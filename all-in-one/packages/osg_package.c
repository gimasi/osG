#include "../../osg/src/BaseHw.c"

#include "../../osg/src/drivers/Gpio.c"

#if OSG_USE_I2C == 1
#include "../../osg/src/drivers/I2c.c"
#endif

#if OSG_USE_LED == 1
#include "../../osg/src/drivers/Led.c"
#endif

#if OSG_OS_USE_MESSAGES == 1
#include "../../osg/src/drivers/MessageQueue.c"
#endif

#if OSG_OS_USE_MUTEX == 1
#include "../../osg/src/drivers/Mutex.c"
#endif

#if OSG_USE_SPI == 1
#include "../../osg/src/drivers/Spi.c"
#endif

#if OSG_OS_USE_THREAD == 1
#include "../../osg/src/drivers/Thread.c"
#endif

#if (OSG_USE_TIMER == 1) || (OSG_OS_USE_TIMER == 1)
#include "../../osg/src/drivers/Timer.c"
#endif

#if OSG_USE_UART == 1
#include "../../osg/src/drivers/Uart.c"
#endif

#include "../../osg/src/utils/CircularFifo.c"

#include "../../osg/src/utils/Log.c"
