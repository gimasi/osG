#include "osgConfig.h"
#include <osg-all-in-one.h>
#include <string.h>
#include "cmsis_os2.h"
#include "boardDefines.h"

static void application()
{
    osg_I2c i2c;
    osg_I2cConfig i2cConfig;
    memset(&i2cConfig, 0, sizeof(i2cConfig));

    i2cConfig.id = OSG_I2C1;
    i2cConfig.mode = OSG_I2C_MASTER_MODE;
    i2cConfig.clockSpeed = 0x00303D5B;
    i2cConfig.addressingMode = OSG_I2C_7BIT_ADDRESS;
    i2cConfig.enableGeneralCall = false;
    i2cConfig.ownAddress = 0;
    i2cConfig.sdaPin = ACC_I2C1_SDA_PIN;
    i2cConfig.sclPin = ACC_I2C1_SCL_PIN;
    i2cConfig.alternate = OSG_USE_I2C1_AF;
    osg_I2c_ctor(&i2c,&i2cConfig);

    uint16_t slaveAddr = 0x19;
    uint16_t regAddress = 0x0F;
    uint8_t bufferRxSplit[1] = {0};
    uint8_t bufferRxMem[1] = {0};

    if (osg_I2c_sendBlocking(&i2c, slaveAddr, &regAddress, 1, 5000) == false)
        osg_error("ERROR: message not sent");

    if (osg_I2c_receiveBlocking(&i2c, slaveAddr, bufferRxSplit, sizeof(bufferRxSplit)/sizeof(bufferRxSplit[0]), 5000) == false)
        osg_error("ERROR: message not received");

    if (osg_I2c_readMemBlocking(&i2c, slaveAddr, regAddress, 1, bufferRxMem, sizeof(bufferRxMem)/sizeof(bufferRxMem[0]), 1000) == false)
        osg_error("ERROR: I2C communication error.");

    if (bufferRxSplit[0] != bufferRxMem[0])
        osg_error("ERROR: Received different values.");

    osg_Led led1;
    osg_Led led2;
    osg_Led led3;
    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));

    ledConfig.gpioId = LED1_PIN;
    osg_Led_ctor(&led1, &ledConfig);
    ledConfig.gpioId = LED2_PIN;
    osg_Led_ctor(&led2, &ledConfig);
    ledConfig.gpioId = LED3_PIN;
    osg_Led_ctor(&led3, &ledConfig);

    osg_Led_setBlinkingPeriod(&led1, 500);
    osg_Led_setBlinkingPeriod(&led2, 750);
    osg_Led_setBlinkingPeriod(&led3, 1000);

    osg_Led_turnOn(&led1);
    osg_Led_turnOn(&led2);
    osg_Led_turnOn(&led3);

    osg_Led_startBlinking(&led1);
    osg_Led_startBlinking(&led2);
    osg_Led_startBlinking(&led3);

    for (;;);
}

int main()
{
    const osThreadAttr_t attr = {
        .stack_size = 1024          // Create the thread stack with a size of 1024 bytes
    };

    bool ret = osg_baseHw_startCustom(application, &attr);
    osg_assert(ret == true, "ERROR while start HW");

    return 1;
}
