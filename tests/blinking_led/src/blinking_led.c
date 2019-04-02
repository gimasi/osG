#include "osgConfig.h"
#include <string.h>
#include <osg-all-in-one.h>

static void applicationUserCode(void)
{
    osg_Led green;
    osg_Led blue;
    osg_Led red;
    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));

    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&green, &ledConfig);
    if (MCU_NAME == NXP_MK28F15)
    {
        ledConfig.ledId = OSG_LED_BLUE;
        osg_Led_ctor(&blue, &ledConfig);
        ledConfig.ledId = OSG_LED_RED;
        osg_Led_ctor(&red, &ledConfig);
    }

    // SINGLE LED TESTS

    // test 1: turn on
    osg_Led_turnOn(&green);

    // test 2: check status
    if (osg_Led_isOn(&green) != true)
        osg_error("Error 2 in turn on led.");

    osg_baseHw_wait(1000);

    // test 3: turn off
    osg_Led_turnOff(&green);

    // test 4: check status
    if (osg_Led_isOn(&green) != false)
        osg_error("Error 4 in turn off led.");

    osg_baseHw_wait(1000);

    // test 5: simply blinking with default values
    if (osg_Led_startBlinking(&green) == false)
        osg_error("Error 5 in start default blinking led.");

    osg_baseHw_wait(5000);

    // test 6: stop blinking
    if (osg_Led_stopBlinking(&green) == false)
        osg_error("Error 6 in stop default blinking led.");

    // test 7: blinking with custom settings
    osg_Led_setBlinkingPeriod(&green, 200);
    osg_Led_setNumberOfBlinks(&green, 3);
    if (osg_Led_startBlinking(&green) == false)
        osg_error("Error 7 in custom blinking led.");

    osg_baseHw_wait(2000);

    // test 8: restart the blinking
    if (osg_Led_startBlinking(&green) == false)
        osg_error("Error 8 in restart custom blinking led.");

    osg_baseHw_wait(1100);

    // test 9: interrupt blinking with on
    if (osg_Led_startBlinking(&green) == false)
        osg_error("Error 9 in interrupt blinking led.");
    osg_baseHw_wait(100);
    osg_Led_turnOn(&green);

    osg_baseHw_wait(5000);

    // test 10: change settings and interrupt with off
    osg_Led_setBlinkingPeriod(&green, 1000);
    osg_Led_setNumberOfBlinks(&green, 10);
    if (osg_Led_startBlinking(&green) == false)
        osg_error("Error 10 in change blinking led.");
    osg_baseHw_wait(5000);
    osg_Led_turnOff(&green);

    if (osg_Led_isOn(&green) != false)
        osg_error("Error in turn off led.");

    // test 11: last blinking
    //if (osg_Led_toggleBlinking(&green) == false)
    //    osg_error("Error 11 in last blinking led.");
    while (osg_Led_isBlinking(&green) == true)
    {
    }

    // MULTIPLE LED TESTS
    if (MCU_NAME == NXP_MK28F15)
    {
        osg_Led_setBlinkingPeriod(&green, 100);
        osg_Led_setBlinkingPeriod(&blue, 50);
        osg_Led_setBlinkingPeriod(&red, 250);
        osg_Led_setNumberOfBlinks(&red, 5);
        osg_Led_startBlinking(&green);
        osg_Led_startBlinking(&blue);
        osg_Led_startBlinking(&red);
        while (osg_Led_isBlinking(&red) == true)
        {
        }

        osg_Led_turnOff(&blue);
        osg_Led_turnOn(&green);

        osg_Led_setNumberOfBlinks(&blue, 100);
        osg_Led_startBlinking(&blue);
        while (osg_Led_isBlinking(&blue) == true)
        {
        }

        osg_Led_turnOff(&green);

        osg_Led_setNumberOfBlinks(&red, 0);
        osg_Led_startBlinking(&red);
        osg_Led_startBlinking(&green);
        while (osg_Led_isBlinking(&green) == true)
        {
        }

        osg_Led_toggleBlinking(&red);
        osg_Led_setBlinkingPeriod(&green, 2000);
        osg_Led_setNumberOfBlinks(&green, 3);
        osg_Led_setBlinkingPeriod(&blue, 750);
        osg_Led_setNumberOfBlinks(&blue, 8);
        osg_Led_setBlinkingPeriod(&red, 127);
        osg_Led_setNumberOfBlinks(&red, 50);
        osg_Led_startBlinking(&red);
        osg_Led_startBlinking(&blue);
        osg_Led_startBlinking(&green);
        while (osg_Led_isBlinking(&green) == true
            || osg_Led_isBlinking(&blue) == true
            || osg_Led_isBlinking(&red) == true)
        {
        }
    }

    osg_Led_dtor(&green);
    osg_Led_dtor(&blue);
    osg_Led_dtor(&red);

    osg_baseHw_wait(2000);
    
    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&green, &ledConfig);
    osg_Led_setBlinkingPeriod(&green, 500);
    osg_Led_startBlinking(&green);

    for (;;) { /* ntd */ }
}

int main(void)
{
    const bool res = osg_baseHw_start(applicationUserCode);
    osg_assert(res == true, "HW base initialization failed");

    return 0;
}
