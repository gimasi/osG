#include <osg.h>

static void applicationUserCode(void)
{
    osg_Led led;
    osg_Led_ctor(&led, OSG_LED_GREEN);

    if (osg_Led_startBlinking(&led, 500) == FALSE)
        osg_error("Error in blinking led.");
    
    osg_baseHw_wait(5000);
    
    osg_Led_stopBlinking(&led);
    osg_Led_dtor(&led);

    for (;;) { /* ntd */ }
}

int main(void)
{
    const Bool res = osg_baseHw_start(applicationUserCode);
    osg_assert(res == TRUE, "HW base initialization failed");

    return 0;
}
