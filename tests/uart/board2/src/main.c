#include <osg-all-in-one.h>

// ///////////////
// NXP MK28F15
//  - UART2
//     o PA24 -> TX
//     o PA25 -> RX
// ////////////////

// Support variables and functions
bool sent;
bool received;
void blinkingGreenLed(void);
void blinkingRedLed(void);
void nonBlockingSendCallback(void);
void nonBlockingReceiveCallback(void);
void bufferedSendCallback(void);
void bufferedReceiveCallback(void);

// Test functions
void uartBlockingSend(void);
void uartBlockingReceive(void);
void uartNonBlockingSend(void);
void uartNonBlockingReceive(void);
void uartBufferedSend(void);
void uartBufferedReceive(void);
void uartAllTest(void);

int main()
{
    const bool startOk = osg_baseHw_start(uartAllTest);
    osg_assert(startOk == true, "ASSERT FAILED: Starting osG application.");

    for (;;);

	return 0;
}

void uartBlockingSend()
{
    osg_Uart uart2;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART2;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PA24;
    uartConfig.rxPin = OSG_GPIO_PA25;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART2_AF;
    osg_Uart_ctor(&uart2, &uartConfig);

    uint8_t message[] = "Hello osG!\r\n";

    const bool res = osg_Uart_sendBlocking(&uart2, message, sizeof(message) / sizeof(message[0]), 2000);
    if (res == false)
    {
        blinkingRedLed();
    }

    blinkingGreenLed();

    for (;;);
}

void uartBlockingReceive()
{
    osg_Uart uart2;
        osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART2;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PA24;
    uartConfig.rxPin = OSG_GPIO_PA25;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART2_AF;
    osg_Uart_ctor(&uart2, &uartConfig);

    uint8_t message[12];

    const bool res = osg_Uart_receiveBlocking(&uart2, message, sizeof(message) / sizeof(message[0]), 10000);
    if (res == false)
    {
        blinkingRedLed();
    }

    blinkingGreenLed();

    for(;;);
}

void nonBlockingSendCallback(void)
{
    sent = true;
}

void nonBlockingReceiveCallback(void)
{
    received = true;
}

void uartNonBlockingSend()
{
    osg_Uart uart2;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART2;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PA24;
    uartConfig.rxPin = OSG_GPIO_PA25;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART2_AF;
    osg_Uart_ctor(&uart2, &uartConfig);

    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setNbCallbacks(&uart2, nonBlockingSendCallback, NULL);

    sent = false;
    const bool res = osg_Uart_sendNonBlocking(&uart2, message, sizeof(message) / sizeof(message[0]));
    if (res == false)
    {
        blinkingRedLed();
    }

    while (sent == false)
    {
    }

    blinkingGreenLed();

    for (;;);
}

void uartNonBlockingReceive()
{
    osg_Uart uart2;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART2;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PA24;
    uartConfig.rxPin = OSG_GPIO_PA25;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART2_AF;
    osg_Uart_ctor(&uart2, &uartConfig);

    uint8_t message[12];
    osg_Uart_setNbCallbacks(&uart2, NULL, nonBlockingReceiveCallback);

    received = false;
    const bool res = osg_Uart_receiveNonBlocking(&uart2, message, sizeof(message) / sizeof(message[0]));
    if (res == false)
    {
        blinkingRedLed();
    }

    while (received == false)
    {
    }

    blinkingGreenLed();

    for(;;);
}

void bufferedSendCallback(void)
{
    sent = true;
}

void bufferedReceiveCallback(void)
{
    received = true;
}

void uartBufferedSend()
{
    uint8_t sendBuffer[20];
    osg_Uart uart2;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART2;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PA24;
    uartConfig.rxPin = OSG_GPIO_PA25;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART2_AF;
    osg_Uart_ctor(&uart2, &uartConfig);

    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setBufferedCallbacks(&uart2, bufferedSendCallback, NULL);

    sent = false;
    const bool res = osg_Uart_sendBuffered(&uart2, message, sizeof(message) / sizeof(message[0]));
    if (res == false)
    {
        blinkingRedLed();
    }

    while (sent == false)
    {
    }

    blinkingGreenLed();

    for (;;);
}

void uartBufferedReceive()
{
    uint8_t receiveBuffer[20];
    osg_Uart uart2;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART2;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PA24;
    uartConfig.rxPin = OSG_GPIO_PA25;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART2_AF;
    osg_Uart_ctor(&uart2, &uartConfig);

    uint8_t message[12];
    int messageSize = sizeof(message) / sizeof(message[0]);
    osg_Uart_setBufferedCallbacks(&uart2, NULL, bufferedReceiveCallback);

    received = false;
    osg_Uart_startReceiveBuffered(&uart2);

    int charsReceived = osg_Uart_getReceivedCharsRxBuffer(&uart2);
    while (charsReceived != messageSize)
    {
        charsReceived = osg_Uart_getReceivedCharsRxBuffer(&uart2);
    }

    Size res = osg_Uart_receiveBuffered(&uart2, message, messageSize);
    if (res != charsReceived)
    {
        blinkingRedLed();
    }

    osg_Uart_stopReceiveBuffered(&uart2);

    blinkingGreenLed();

    for(;;);
}


static osg_Uart * _uart = NULL;
static uint8_t _nbRxChar = 0;
static uint8_t _nbCount = 0;
static bool _bufSent = false;
static uint8_t _bufCount = 0;

static void setUart(osg_Uart * uart)
{
    _uart = uart;
}

static osg_Uart * getUart(void)
{
    return _uart;
}

void nbRxCallback(void)
{
    osg_Uart * uart2 = getUart();
    const bool resSend = osg_Uart_sendNonBlocking(uart2, &_nbRxChar, 1);
    if (resSend == false)
    {
        blinkingGreenLed();
    }
}

void nbTxCallback(void)
{
    _nbCount++;

    if (_nbCount <= 10)
    {
        osg_Uart * uart2 = getUart();
        const bool resRecv = osg_Uart_receiveNonBlocking(uart2, &_nbRxChar, 1);
        if (resRecv == false)
        {
            blinkingGreenLed();
        }
    }
}

uint8_t buffer[5];
void bufRxCallback(void)
{
    osg_Uart * uart2 = getUart();
    if (osg_Uart_getReceivedCharsRxBuffer(uart2) > 0)
    {

        Size recv = osg_Uart_receiveBuffered(uart2, buffer, sizeof(buffer) / sizeof(buffer[0]));
        const bool resSend = osg_Uart_sendBuffered(uart2, buffer, recv);
        if (resSend == true)
            _bufCount += recv;
    }
}

void bufTxCallback(void)
{
    _bufSent = true;

    if (_bufCount >= 10)
    {
        osg_Uart * uart2 = getUart();
        osg_Uart_stopReceiveBuffered(uart2);
    }
}

void uartAllTest()
{
    osg_Uart uart2;
    int count = 0;
    osg_Led greenLed;
    osg_LedConfig ledConf;
    ledConf.gpioId = 0;
    ledConf.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConf);
    osg_Led_setBlinkingPeriod(&greenLed, 25);
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));

    for (;;)
    {
        // ///////////////////////
        // BLOCKING SECTION
        // ///////////////////////
        uartConfig.id = OSG_UART2;
	    uartConfig.baud = OSG_UART_BAUDRATE_9600;
        uartConfig.parity = OSG_UART_PARITY_NONE;
        uartConfig.bits = OSG_UART_BITS_8;
        uartConfig.stopBits = OSG_UART_STOPBITS_1;
        uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
        uartConfig.txPin = OSG_GPIO_PA24;
        uartConfig.rxPin = OSG_GPIO_PA25;
        uartConfig.ctsPin = OSG_GPIO_PA0;
        uartConfig.rtsPin = OSG_GPIO_PA0;
        uartConfig.alternateFunction = OSG_USE_UART2_AF;
        osg_Uart_ctor(&uart2, &uartConfig);
        uint8_t messageBlock[] = "\r\nType characters, they are received and re-sent (printed) with BLOCKING functions:\r\n";

        const bool resB = osg_Uart_sendBlocking(&uart2, messageBlock, sizeof(messageBlock) / sizeof(messageBlock[0]), 2000);
        if (resB == false)
        {
            osg_Led_setNumberOfBlinks(&greenLed, 3);
            osg_Led_startBlinking(&greenLed);
            while (osg_Led_isBlinking(&greenLed) == true)
            {
            }
        }
        else
        {
            while (count < 10)
            {
                uint8_t ch = 0;
                const bool resRecv = osg_Uart_receiveBlocking(&uart2, &ch, 1, 100000);
                if (resRecv == false)
                {
                    osg_Led_setNumberOfBlinks(&greenLed, 3);
                    osg_Led_startBlinking(&greenLed);
                    while (osg_Led_isBlinking(&greenLed) == true)
                    {
                    }
                    break;
                }

                const bool resSend = osg_Uart_sendBlocking(&uart2, &ch, 1, 200);
                if (resSend == false)
                {
                    osg_Led_setNumberOfBlinks(&greenLed, 3);
                    osg_Led_startBlinking(&greenLed);
                    while (osg_Led_isBlinking(&greenLed) == true)
                    {
                    }
                    break;
                }

                count++;
            }
        }

        count = 0;
        osg_Uart_dtor(&uart2);
        osg_Led_stopBlinking(&greenLed);

        // ///////////////////////
        // NON-BLOCKING SECTION
        // ///////////////////////
        uartConfig.id = OSG_UART2;
	    uartConfig.baud = OSG_UART_BAUDRATE_9600;
        uartConfig.parity = OSG_UART_PARITY_NONE;
        uartConfig.bits = OSG_UART_BITS_8;
        uartConfig.stopBits = OSG_UART_STOPBITS_1;
        uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
        uartConfig.txPin = OSG_GPIO_PA24;
        uartConfig.rxPin = OSG_GPIO_PA25;
        uartConfig.ctsPin = OSG_GPIO_PA0;
        uartConfig.rtsPin = OSG_GPIO_PA0;
        uartConfig.alternateFunction = OSG_USE_UART2_AF;
        osg_Uart_ctor(&uart2, &uartConfig);
        uint8_t messageNonB[] = "\r\nType characters, they are received and re-sent (printed) with NON-BLOCKING functions:\r\n";
        osg_Uart_setNbCallbacks(&uart2, nbTxCallback, nbRxCallback);
        setUart(&uart2);

        const bool resN = osg_Uart_sendNonBlocking(&uart2, messageNonB, sizeof(messageNonB) / sizeof(messageNonB[0]));
        if (resN == false)
        {
            osg_Led_setNumberOfBlinks(&greenLed, 3);
            osg_Led_startBlinking(&greenLed);
            while (osg_Led_isBlinking(&greenLed) == true)
            {
            }
        }
        else
        {
            while (_nbCount <= 10)
            {
            }
        }

        count = 0;
        _nbCount = 0;
        _nbRxChar = 0;
        osg_Uart_dtor(&uart2);
        osg_Led_stopBlinking(&greenLed);

        // ///////////////////////
        // BUFFERED SECTION
        // ///////////////////////
        uint8_t sendBuffer[90];
        uint8_t receiveBuffer[90];
        uartConfig.id = OSG_UART2;
	    uartConfig.baud = OSG_UART_BAUDRATE_9600;
        uartConfig.parity = OSG_UART_PARITY_NONE;
        uartConfig.bits = OSG_UART_BITS_8;
        uartConfig.stopBits = OSG_UART_STOPBITS_1;
        uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
        uartConfig.txPin = OSG_GPIO_PA24;
        uartConfig.rxPin = OSG_GPIO_PA25;
        uartConfig.ctsPin = OSG_GPIO_PA0;
        uartConfig.rtsPin = OSG_GPIO_PA0;
        uartConfig.alternateFunction = OSG_USE_UART2_AF;
        osg_Uart_ctor(&uart2, &uartConfig);
        uint8_t messageBuf[] = "\r\nType characters, they are received and re-sent (printed) with BUFFERED functions:\r\n";
        osg_Uart_setBufferedCallbacks(&uart2, bufTxCallback, bufRxCallback);
        setUart(&uart2);

        _bufSent = false;
        const bool resBuf = osg_Uart_sendBuffered(&uart2, messageBuf, sizeof(messageBuf) / sizeof(messageBuf[0]));
        if (resBuf == false)
        {
            osg_Led_setNumberOfBlinks(&greenLed, 3);
            osg_Led_startBlinking(&greenLed);
            while (osg_Led_isBlinking(&greenLed) == true)
            {
            }
        }
        else
        {
            while (_bufSent == false)
            {
            }

            osg_Uart_startReceiveBuffered(&uart2);

            while (osg_Uart_isReceiveBufferedEnabled(&uart2) == true)
            {
            }
        }

        _bufCount = 0;
        _bufSent = false;
        count = 0;
        osg_Uart_dtor(&uart2);
        osg_Led_stopBlinking(&greenLed);
    }

    for (;;);
}

void blinkingGreenLed()
{
    osg_Led greenLed;
    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));
    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 1000);

    osg_Led_startBlinking(&greenLed);

    for(;;);
}

void blinkingRedLed()
{
    osg_Led redLed;
    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));
    ledConfig.ledId = OSG_LED_RED;
    osg_Led_ctor(&redLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&redLed, 250);

    osg_Led_startBlinking(&redLed);

    for(;;);
}
