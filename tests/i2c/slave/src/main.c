#include "osgConfig.h"
#include <osg-all-in-one.h>
#include <string.h>

// ////////////////////////////////////
// SLAVE
// ////////////////////////////////////
#define ADDR 0x08

static bool send = false;
static bool recv = false;

void handleI2cErrors(void)
{
    osg_error("ERROR: I2C communication error.");
}

void handleI2cTx(void)
{
    send = true;
}

void handleI2cRx(void)
{
    recv = true;
}


static void testSlaveBlockingReceive(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[10];
    memset(buff, 0, sizeof(buff));
    const bool recvOk = osg_I2c_receiveBlocking(i2c, ADDR, buff, 10, 5000);
    if (recvOk == true)
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_turnOn(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(greenLed);
    }
    else
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_startBlinking(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(redLed);
    }
}

static void nbRxClbk(void)
{
    recv = true;
}

static void testSlaveNonBlockingReceive(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[10];
    memset(buff, 0, sizeof(buff));
    osg_I2c_setNbCallbacks(i2c, NULL, nbRxClbk);

    recv = false;
    const bool recvOk = osg_I2c_receiveNonBlocking(i2c, ADDR, buff, 10);
    if (recvOk == true)
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_turnOn(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(blueLed);
    }
    else
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_startBlinking(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(redLed);

        return;
    }

    while (recv == false)
    {
    }

    if (MCU_NAME == STM32_L476)
    {
        osg_Led_turnOff(greenLed);
        osg_Led_setNumberOfBlinks(greenLed, 1);
        osg_Led_startBlinking(greenLed);
        while (osg_Led_isBlinking(greenLed) == true)
        {
        }
        osg_Led_turnOn(greenLed);
    }
    else if (MCU_NAME == NXP_MK28F15)
    {
        osg_Led_turnOff(blueLed);
        osg_Led_startBlinking(greenLed);
    }
}

static void bufRxClbk(void)
{
    recv = true;
}

static void testSlaveBufferedReceive(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t message[10];
    memset(message, 0, sizeof(message));
    osg_I2c_setBufferedCallbacks(i2c, NULL, bufRxClbk);

    recv = false;
    osg_I2c_startReceiveBuffered(i2c, ADDR);
    if (MCU_NAME == STM32_L476)
        osg_Led_turnOn(greenLed);
    else if (MCU_NAME == NXP_MK28F15)
        osg_Led_startBlinking(blueLed);

    while (recv == false)
    {
    }

    const Size recvOk = osg_I2c_receiveBuffered(i2c, message, 1);
    if (recvOk == 1)
    {
        if (MCU_NAME == STM32_L476)
        {
            osg_Led_turnOff(greenLed);
            osg_Led_setNumberOfBlinks(greenLed, 1);
            osg_Led_startBlinking(greenLed);
            while (osg_Led_isBlinking(greenLed) == true)
            {
            }
            osg_Led_turnOn(greenLed);
        }
        else if (MCU_NAME == NXP_MK28F15)
        {
            osg_Led_turnOff(blueLed);
            osg_Led_startBlinking(greenLed);
        }
    }
    else
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_startBlinking(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
        {
            osg_Led_turnOff(blueLed);
            osg_Led_startBlinking(redLed);
        }
    }

    osg_I2c_stopReceiveBuffered(i2c);

}

static void testSlaveBlockingSend(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[] = "Hello osG!";
    const bool sendOk = osg_I2c_sendBlocking(i2c, ADDR, buff, 10, 5000);
    if (sendOk == true)
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_turnOn(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(greenLed);
    }
    else
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_startBlinking(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(redLed);
    }
}

static void nbTxClbk(void)
{
    send = true;
}

static void testSlaveNonBlockingSend(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[] = "Hello osG!";
    osg_I2c_setNbCallbacks(i2c, nbTxClbk, NULL);

    send = false;
    const bool sendOk = osg_I2c_sendNonBlocking(i2c, ADDR, buff, 10);
    if (sendOk == true)
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_turnOn(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(blueLed);
    }
    else
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_startBlinking(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(redLed);

        return;
    }

    while (send == false)
    {
    }

    if (MCU_NAME == STM32_L476)
    {
        osg_Led_turnOff(greenLed);
        osg_Led_setNumberOfBlinks(greenLed, 1);
        osg_Led_startBlinking(greenLed);
        while (osg_Led_isBlinking(greenLed) == true)
        {
        }
        osg_Led_turnOn(greenLed);
    }
    else if (MCU_NAME == NXP_MK28F15)
    {
        osg_Led_turnOff(blueLed);
        osg_Led_startBlinking(greenLed);
    }
}

static void bufTxClbk(void)
{
    send = true;
}

static void testSlaveBufferedSend(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t message[] = "Hello osG!";
    osg_I2c_setBufferedCallbacks(i2c, bufTxClbk, NULL);

    send = false;
    const bool sendOk = osg_I2c_sendBuffered(i2c, ADDR, message, 10);
    if (sendOk == true)
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_turnOn(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(blueLed);
    }
    else
    {
        if (MCU_NAME == STM32_L476)
            osg_Led_startBlinking(greenLed);
        else if (MCU_NAME == NXP_MK28F15)
            osg_Led_startBlinking(redLed);

        return;
    }

    while (send == false)
    {
    }

    if (MCU_NAME == STM32_L476)
    {
        osg_Led_turnOff(greenLed);
        osg_Led_setNumberOfBlinks(greenLed, 1);
        osg_Led_startBlinking(greenLed);
        while (osg_Led_isBlinking(greenLed) == true)
        {
        }
        osg_Led_turnOn(greenLed);
    }
    else if (MCU_NAME == NXP_MK28F15)
    {
        osg_Led_turnOff(blueLed);
        osg_Led_startBlinking(greenLed);
    }
}

static void applicationUserCode(void)
{
    osg_I2c i2c;
    osg_Led greenLed;
    osg_Led redLed;
    osg_Led blueLed;
    uint8_t bufferTx[20];
    uint8_t bufferRx[20];
    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));

    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 500);
    if (MCU_NAME == NXP_MK28F15)
    {
        ledConfig.ledId = OSG_LED_RED;
        osg_Led_ctor(&redLed, &ledConfig);
        osg_Led_setBlinkingPeriod(&greenLed, 200);
        ledConfig.ledId = OSG_LED_BLUE;
        osg_Led_ctor(&blueLed, &ledConfig);
        osg_Led_setBlinkingPeriod(&greenLed, 500);
    }

    if (MCU_NAME == STM32_L476)
    {
        // STM32L476 init
        osg_I2cConfig i2cConfig;
        memset(&i2cConfig, 0, sizeof(i2cConfig));

        i2cConfig.id = OSG_I2C1;
        i2cConfig.mode = OSG_I2C_SLAVE_MODE;
        i2cConfig.clockSpeed = 0x00000D15;
        i2cConfig.addressingMode = OSG_I2C_7BIT_ADDRESS;
        i2cConfig.enableGeneralCall = false;
        i2cConfig.ownAddress = ADDR;
        i2cConfig.sdaPin = OSG_GPIO_PB9;
        i2cConfig.sclPin = OSG_GPIO_PB8;
        i2cConfig.alternate = OSG_USE_I2C1_AF;
        osg_I2c_ctor(&i2c, &i2cConfig);
    }
    else if (MCU_NAME == NXP_MK28F15)
    {
        // NXP MK28F15 init
        osg_I2cConfig i2cConfig;
        memset(&i2cConfig, 0, sizeof(i2cConfig));

        i2cConfig.id = OSG_I2C0;
        i2cConfig.mode = OSG_I2C_SLAVE_MODE;
        i2cConfig.clockSpeed = 100000;
        i2cConfig.addressingMode = OSG_I2C_7BIT_ADDRESS;
        i2cConfig.enableGeneralCall = false;
        i2cConfig.ownAddress = ADDR;
        i2cConfig.sdaPin = OSG_GPIO_PB3;
        i2cConfig.sclPin = OSG_GPIO_PB2;
        i2cConfig.alternate = OSG_USE_I2C0_AF;
        osg_I2c_ctor(&i2c, &i2cConfig);
    }
    else
    {
        osg_error("ERROR: Unknown board!");
    }

    testSlaveBlockingReceive(&i2c, &greenLed, &redLed, &blueLed);

    //testSlaveNonBlockingReceive(&i2c, &greenLed, &redLed, &blueLed);

    //testSlaveBufferedReceive(&i2c, &greenLed, &redLed, &blueLed);

    //testSlaveBlockingSend(&i2c, &greenLed, &redLed, &blueLed);

    //testSlaveNonBlockingSend(&i2c, &greenLed, &redLed, &blueLed);

    //testSlaveBufferedSend(&i2c, &greenLed, &redLed, &blueLed);

    for (;;);

    osg_I2c_dtor(&i2c);
    osg_Led_dtor(&greenLed);
    if (MCU_NAME == NXP_MK28F15)
    {
        osg_Led_dtor(&redLed);
        osg_Led_dtor(&blueLed);
    }
}

int main(void)
{
    const bool res = osg_baseHw_start(applicationUserCode);
    osg_assert(res == true, "ASSERT FAILED: HW base initialization failed.");

    return 0;
}
