#include "osgConfig.h"
#include <osg-all-in-one.h>
#include <string.h>

static void testMasterSpi(void);
static void testBlockingSend(osg_Spi * spi, osg_Led * greenLed);

int main()
{
    if (osg_baseHw_start(testMasterSpi) == false)
        osg_error("ERROR while start application");

    return 1;
}

static void testMasterSpi()
{
    osg_Spi spi;
    osg_Led greenLed;
    osg_GpioId ss[] = { OSG_GPIO_PA15 };

    osg_SpiConfig spiConfig;
	memset(&spiConfig, 0, sizeof(spiConfig));
    spiConfig.id = OSG_SPI3;
    spiConfig.mode = OSG_SPI_MASTER_MODE;
    spiConfig.clockSpeed = 10000000;
    spiConfig.polarity = OSG_SPI_POLARITY_LOW;
    spiConfig.phase = OSG_SPI_PHASE_1ST;
    spiConfig.endianess = OSG_SPI_MSB_FIRST;
    spiConfig.dataSize = OSG_SPI_DATA_8_BITS;
    spiConfig.ssMode = OSG_SPI_SLAVE_SELECT_AUTO;
    spiConfig.sclkPin = OSG_GPIO_PC10;
    spiConfig.mosiPin = OSG_GPIO_PC12;
    spiConfig.misoPin = OSG_GPIO_PC11;
    spiConfig.ssPinList = ss;
    spiConfig.ssPinListSize = 1;
    spiConfig.alternateFunction = OSG_USE_SPI3_AF;
    osg_Spi_ctor(&spi, &spiConfig);

    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));
    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 500);

    testBlockingSend(&spi, &greenLed);

    osg_Spi_dtor(&spi);
    osg_Led_dtor(&greenLed);
    for (;;);
}

static void testBlockingSend(osg_Spi * spi, osg_Led * greenLed)
{
    uint8_t message[] = "Hello osG!";
    const bool sendOk = osg_Spi_sendBlocking(spi, 0, message, 10, 5000);
    if (sendOk == true)
    {
        osg_Led_turnOn(greenLed);
    }
    else
    {
        osg_Led_startBlinking(greenLed);
    }

    for(;;);
}
