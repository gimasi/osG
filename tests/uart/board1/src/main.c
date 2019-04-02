#include <osg-all-in-one.h>
#include <string.h>

// ///////////////
// STM32L4
//  - USART3:
//     o PC10 -> TX
//     o PC11 -> RX
// ///////////////

// Support variables and functions
bool sent;
bool received;
void blinkingGreenLed(void);
void turnOnGreenLed(void);
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

	return 0;
}

void uartBlockingSend()
{
    osg_Uart uart3;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART3;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PC10;
    uartConfig.rxPin = OSG_GPIO_PC11;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART3_AF;
    osg_Uart_ctor(&uart3, &uartConfig);

    uint8_t message[] = "Hello osG!\r\n";

    const bool res = osg_Uart_sendBlocking(&uart3, message, sizeof(message) / sizeof(message[0]), 2000);
    if (res == false)
    {
        blinkingGreenLed();
    }

    turnOnGreenLed();

    for (;;);
}

void uartBlockingReceive()
{
    osg_Uart uart3;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART3;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PC10;
    uartConfig.rxPin = OSG_GPIO_PC11;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART3_AF;
    osg_Uart_ctor(&uart3, &uartConfig);

    uint8_t message[12];

    const bool res = osg_Uart_receiveBlocking(&uart3, message, sizeof(message) / sizeof(message[0]), 10000);
    if (res == false)
    {
        blinkingGreenLed();
    }

    turnOnGreenLed();

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
    osg_Uart uart3;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART3;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PC10;
    uartConfig.rxPin = OSG_GPIO_PC11;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART3_AF;
    osg_Uart_ctor(&uart3, &uartConfig);

    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setNbCallbacks(&uart3, nonBlockingSendCallback, NULL);

    sent = false;
    const bool res = osg_Uart_sendNonBlocking(&uart3, message, sizeof(message) / sizeof(message[0]));
    if (res == false)
    {
        blinkingGreenLed();
    }

    while (sent == false)
    {
    }

    turnOnGreenLed();

    for (;;);
}

void uartNonBlockingReceive()
{
    osg_Uart uart3;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART3;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PC10;
    uartConfig.rxPin = OSG_GPIO_PC11;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART3_AF;
    osg_Uart_ctor(&uart3, &uartConfig);

    uint8_t message[12];
    osg_Uart_setNbCallbacks(&uart3, NULL, nonBlockingReceiveCallback);

    received = false;
    const bool res = osg_Uart_receiveNonBlocking(&uart3, message, sizeof(message) / sizeof(message[0]));
    if (res == false)
    {
        blinkingGreenLed();
    }

    while (received == false)
    {
    }

    turnOnGreenLed();

    for(;;);
}

void bufferedSendCallback(void)
{
    sent = true;
}

void uartBufferedSend()
{
    uint8_t sendBuffer[20];
    osg_Uart uart3;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART3;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PC10;
    uartConfig.rxPin = OSG_GPIO_PC11;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART3_AF;
    osg_Uart_ctor(&uart3, &uartConfig);

    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setBufferedCallbacks(&uart3, bufferedSendCallback, NULL);

    sent = false;
    const bool res = osg_Uart_sendBuffered(&uart3, message, sizeof(message) / sizeof(message[0]));
    if (res == false)
    {
        blinkingGreenLed();
    }

    while (sent == false)
    {
    }

    turnOnGreenLed();

    for (;;);
}

void uartBufferedReceive()
{
    uint8_t receiveBuffer[20];
    osg_Uart uart3;
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));
    uartConfig.id = OSG_UART3;
	uartConfig.baud = OSG_UART_BAUDRATE_9600;
    uartConfig.parity = OSG_UART_PARITY_NONE;
    uartConfig.bits = OSG_UART_BITS_8;
    uartConfig.stopBits = OSG_UART_STOPBITS_1;
    uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
    uartConfig.txPin = OSG_GPIO_PC10;
    uartConfig.rxPin = OSG_GPIO_PC11;
    uartConfig.ctsPin = OSG_GPIO_PA0;
    uartConfig.rtsPin = OSG_GPIO_PA0;
    uartConfig.alternateFunction = OSG_USE_UART3_AF;
    osg_Uart_ctor(&uart3, &uartConfig);

    uint8_t message[12];
    int messageSize = sizeof(message) / sizeof(message[0]);
    osg_Uart_setBufferedCallbacks(&uart3, NULL, NULL);

    osg_Uart_startReceiveBuffered(&uart3);

    int charsReceived = osg_Uart_getReceivedCharsRxBuffer(&uart3);
    while (charsReceived != messageSize)
    {
        charsReceived = osg_Uart_getReceivedCharsRxBuffer(&uart3);
    }

    Size res = osg_Uart_receiveBuffered(&uart3, message, messageSize);
    if (res != charsReceived)
    {
        blinkingGreenLed();
    }

    osg_Uart_stopReceiveBuffered(&uart3);

    turnOnGreenLed();

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
    osg_Uart * uart3 = getUart();
    const bool resSend = osg_Uart_sendNonBlocking(uart3, &_nbRxChar, 1);
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
        osg_Uart * uart3 = getUart();
        const bool resRecv = osg_Uart_receiveNonBlocking(uart3, &_nbRxChar, 1);
        if (resRecv == false)
        {
            blinkingGreenLed();
        }
    }
}

void bufRxCallback(void)
{
    osg_Uart * uart3 = getUart();
    if (osg_Uart_getReceivedCharsRxBuffer(uart3) > 0)
    {
        uint8_t buffer[5];
        Size recv = osg_Uart_receiveBuffered(uart3, buffer, sizeof(buffer) / sizeof(buffer[0]));
        if (recv > sizeof(buffer) / sizeof(buffer[0]))
            recv = sizeof(buffer) / sizeof(buffer[0]);

        const bool resSend = osg_Uart_sendBuffered(uart3, buffer, recv);
        if (resSend == true)
            _bufCount += recv;
    }
}

void bufTxCallback(void)
{
    _bufSent = true;

    if (_bufCount >= 10)
    {
        osg_Uart * uart3 = getUart();
        osg_Uart_stopReceiveBuffered(uart3);
    }
}

void uartAllTest()
{
    osg_Uart uart3;
    int count = 0;
    osg_Led greenLed;
    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));
    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConfig);
    osg_Led_setBlinkingPeriod(&greenLed, 250);
    osg_UartConfig uartConfig;
	memset(&uartConfig, 0, sizeof(uartConfig));

    for (;;)
    {
        // ///////////////////////
        // BLOCKING SECTION
        // ///////////////////////
        uartConfig.id = OSG_UART3;
	    uartConfig.baud = OSG_UART_BAUDRATE_9600;
        uartConfig.parity = OSG_UART_PARITY_NONE;
        uartConfig.bits = OSG_UART_BITS_8;
        uartConfig.stopBits = OSG_UART_STOPBITS_1;
        uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
        uartConfig.txPin = OSG_GPIO_PC10;
        uartConfig.rxPin = OSG_GPIO_PC11;
        uartConfig.ctsPin = OSG_GPIO_PA0;
        uartConfig.rtsPin = OSG_GPIO_PA0;
        uartConfig.alternateFunction = OSG_USE_UART3_AF;
        osg_Uart_ctor(&uart3, &uartConfig);
        uint8_t messageBlock[] = "\r\nType characters, they are received and re-sent (printed) with BLOCKING functions:\r\n";

        const bool resB = osg_Uart_sendBlocking(&uart3, messageBlock, sizeof(messageBlock) / sizeof(messageBlock[0]), 2000);
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
                const bool resRecv = osg_Uart_receiveBlocking(&uart3, &ch, 1, 100000);
                if (resRecv == false)
                {
                    osg_Led_setNumberOfBlinks(&greenLed, 3);
                    osg_Led_startBlinking(&greenLed);
                    while (osg_Led_isBlinking(&greenLed) == true)
                    {
                    }
                    break;
                }

                const bool resSend = osg_Uart_sendBlocking(&uart3, &ch, 1, 200);
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
        osg_Uart_dtor(&uart3);
        osg_Led_stopBlinking(&greenLed);

        // ///////////////////////
        // NON-BLOCKING SECTION
        // ///////////////////////
        uartConfig.id = OSG_UART3;
	    uartConfig.baud = OSG_UART_BAUDRATE_9600;
        uartConfig.parity = OSG_UART_PARITY_NONE;
        uartConfig.bits = OSG_UART_BITS_8;
        uartConfig.stopBits = OSG_UART_STOPBITS_1;
        uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
        uartConfig.txPin = OSG_GPIO_PC10;
        uartConfig.rxPin = OSG_GPIO_PC11;
        uartConfig.ctsPin = OSG_GPIO_PA0;
        uartConfig.rtsPin = OSG_GPIO_PA0;
        uartConfig.alternateFunction = OSG_USE_UART3_AF;
        osg_Uart_ctor(&uart3,&uartConfig);
        uint8_t messageNonB[] = "\r\nType characters, they are received and re-sent (printed) with NON-BLOCKING functions:\r\n";
        osg_Uart_setNbCallbacks(&uart3, nbTxCallback, nbRxCallback);
        setUart(&uart3);

        const bool resN = osg_Uart_sendNonBlocking(&uart3, messageNonB, sizeof(messageNonB) / sizeof(messageNonB[0]));
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
        osg_Uart_dtor(&uart3);
        osg_Led_stopBlinking(&greenLed);

        // ///////////////////////
        // BUFFERED SECTION
        // ///////////////////////
        uint8_t sendBuffer[90];
        uint8_t receiveBuffer[90];
        uartConfig.id = OSG_UART3;
	    uartConfig.baud = OSG_UART_BAUDRATE_9600;
        uartConfig.parity = OSG_UART_PARITY_NONE;
        uartConfig.bits = OSG_UART_BITS_8;
        uartConfig.stopBits = OSG_UART_STOPBITS_1;
        uartConfig.flow = OSG_UART_FLOWCONTROL_NONE;
        uartConfig.txPin = OSG_GPIO_PC10;
        uartConfig.rxPin = OSG_GPIO_PC11;
        uartConfig.ctsPin = OSG_GPIO_PA0;
        uartConfig.rtsPin = OSG_GPIO_PA0;
        uartConfig.alternateFunction = OSG_USE_UART3_AF;
        osg_Uart_ctor(&uart3, &uartConfig);
        uint8_t messageBuf[] = "\r\nType characters, they are received and re-sent (printed) with BUFFERED functions:\r\n";
        osg_Uart_setBufferedCallbacks(&uart3, bufTxCallback, bufRxCallback);
        setUart(&uart3);

        _bufSent = false;
        const bool resBuf = osg_Uart_sendBuffered(&uart3, messageBuf, sizeof(messageBuf) / sizeof(messageBuf[0]));
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

            osg_Uart_startReceiveBuffered(&uart3);

            while (osg_Uart_isReceiveBufferedEnabled(&uart3) == true)
            {
            }
        }

        _bufCount = 0;
        _bufSent = false;
        count = 0;
        osg_Uart_dtor(&uart3);
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
    osg_Led_setBlinkingPeriod(&greenLed, 250);

    osg_Led_startBlinking(&greenLed);

    for(;;);
}

void turnOnGreenLed()
{

    osg_Led greenLed;

    osg_LedConfig ledConfig;
    memset(&ledConfig, 0, sizeof(ledConfig));
    ledConfig.ledId = OSG_LED_GREEN;
    osg_Led_ctor(&greenLed, &ledConfig);

    osg_Led_turnOn(&greenLed);

    for(;;);
}
