#include "osgConfig.h"
#include <osg-all-in-one.h>
#include <string.h>

static void testSlaveSpi(void);
static void testBlockingRecv(osg_Spi * spi, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed);

int main()
{
    if (osg_baseHw_start(testSlaveSpi) == false)
        osg_error("ERROR: HW init failed.");

    return 1;
}

static void testSlaveSpi()
{
    osg_Spi spi;
    osg_Led greenLed;
    osg_Led redLed;
    osg_Led blueLed;

    osg_GpioId ss[] = { OSG_GPIO_PD15 };
    osg_SpiConfig spiConfig;
	memset(&spiConfig, 0, sizeof(spiConfig));
    spiConfig.id = OSG_SPI2;
    spiConfig.mode = OSG_SPI_SLAVE_MODE;
    spiConfig.clockSpeed = 10000000;
    spiConfig.polarity = OSG_SPI_POLARITY_LOW;
    spiConfig.phase = OSG_SPI_PHASE_1ST;
    spiConfig.endianess = OSG_SPI_MSB_FIRST;
    spiConfig.dataSize = OSG_SPI_DATA_8_BITS;
    spiConfig.ssMode = OSG_SPI_SLAVE_SELECT_AUTO;
    spiConfig.sclkPin = OSG_GPIO_PD12;
    spiConfig.mosiPin = OSG_GPIO_PD14;
    spiConfig.misoPin = OSG_GPIO_PD13;
    spiConfig.ssPinList = ss;
    spiConfig.ssPinListSize = 1;
    spiConfig.alternateFunction = OSG_USE_SPI2_AF;
    osg_Spi_ctor(&spi, &spiConfig);

    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));

    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 500);
    ledConfig.ledId = OSG_LED_RED;
    osg_Led_ctor(&redLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 200);
    ledConfig.ledId = OSG_LED_BLUE;
    osg_Led_ctor(&blueLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 500);

    testBlockingRecv(&spi, &greenLed, &redLed, &blueLed);

    osg_Led_dtor(&greenLed);
    osg_Led_dtor(&redLed);
    osg_Led_dtor(&blueLed);
    osg_Spi_dtor(&spi);
    for (;;);
}

static void testBlockingRecv(osg_Spi * spi, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[10];
    memset(buff, 0, sizeof(buff));
    const bool recvOk = osg_Spi_receiveBlocking(spi, 0, buff, 10, 10000);
    if (recvOk == true)
    {
        osg_Led_startBlinking(greenLed);
    }
    else
    {
        osg_Led_startBlinking(redLed);
    }

    for(;;);
}
