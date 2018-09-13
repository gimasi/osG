#include <osg.h>

// ///////////////
// NXP MK28F15
//  - UART2
//     o PA24 -> TX
//     o PA25 -> RX
// ////////////////

// Support variables and functions
Bool sent;
Bool received;
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
    const Bool startOk = osg_baseHw_start(uartAllTest);
    osg_assert(startOk == TRUE, "ASSERT FAILED: Starting osG application.");
    
    for (;;);
    
	return 0;
}

void uartBlockingSend()
{
    osg_Uart uart2;
    osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[] = "Hello osG!\r\n";
    
    const Bool res = osg_Uart_sendBlocking(&uart2, message, sizeof(message) / sizeof(message[0]), 2000);
    if (res == FALSE)
    {
        blinkingRedLed();
    }
    
    blinkingGreenLed();
    
    for (;;);
}

void uartBlockingReceive()
{
    osg_Uart uart2;
    osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[12];
    
    const Bool res = osg_Uart_receiveBlocking(&uart2, message, sizeof(message) / sizeof(message[0]), 10000);
    if (res == FALSE)
    {
        blinkingRedLed();
    }
    
    blinkingGreenLed();
    
    for(;;);
}

void nonBlockingSendCallback(void)
{
    sent = TRUE;
}

void nonBlockingReceiveCallback(void)
{
    received = TRUE;
}

void uartNonBlockingSend()
{
    osg_Uart uart2;
    osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setNbCallbacks(&uart2, nonBlockingSendCallback, NULL);
    
    sent = FALSE;
    const Bool res = osg_Uart_sendNonBlocking(&uart2, message, sizeof(message) / sizeof(message[0]));
    if (res == FALSE)
    {
        blinkingRedLed();
    }
    
    while (sent == FALSE)
    {
    }
    
    blinkingGreenLed();
    
    for (;;);
}

void uartNonBlockingReceive()
{
    osg_Uart uart2;
    osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[12];
    osg_Uart_setNbCallbacks(&uart2, NULL, nonBlockingReceiveCallback);
    
    received = FALSE;
    const Bool res = osg_Uart_receiveNonBlocking(&uart2, message, sizeof(message) / sizeof(message[0]));
    if (res == FALSE)
    {
        blinkingRedLed();
    }
    
    while (received == FALSE)
    {
    }
    
    blinkingGreenLed();
    
    for(;;);
}

void bufferedSendCallback(void)
{
    sent = TRUE;
}

void bufferedReceiveCallback(void)
{
    received = TRUE;
}

void uartBufferedSend()
{
    uint8_t sendBuffer[20];
    osg_Uart uart2;
    osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, sendBuffer, sizeof(sendBuffer) / sizeof(sendBuffer[0]), NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setBufferedCallbacks(&uart2, bufferedSendCallback, NULL);
    
    sent = FALSE;
    const Bool res = osg_Uart_sendBuffered(&uart2, message, sizeof(message) / sizeof(message[0]));
    if (res == FALSE)
    {
        blinkingRedLed();
    }
    
    while (sent == FALSE)
    {
    }
    
    blinkingGreenLed();
    
    for (;;);
}

void uartBufferedReceive()
{
    uint8_t receiveBuffer[20];
    osg_Uart uart2;
    osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, receiveBuffer, sizeof(receiveBuffer) / sizeof(receiveBuffer[0]), OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[12];
    int messageSize = sizeof(message) / sizeof(message[0]);
    osg_Uart_setBufferedCallbacks(&uart2, NULL, bufferedReceiveCallback);
    
    received = FALSE;
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
static Bool _bufSent = FALSE;
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
    const Bool resSend = osg_Uart_sendNonBlocking(uart2, &_nbRxChar, 1);
    if (resSend == FALSE)
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
        const Bool resRecv = osg_Uart_receiveNonBlocking(uart2, &_nbRxChar, 1);
        if (resRecv == FALSE)
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
        const Bool resSend = osg_Uart_sendBuffered(uart2, buffer, recv);
        if (resSend == TRUE)
            _bufCount += recv;
    }
}

void bufTxCallback(void)
{
    _bufSent = TRUE;
    
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
    osg_Led_ctor(&greenLed, OSG_LED_GREEN);
    
    for (;;)
    {
        // ///////////////////////
        // BLOCKING SECTION
        // ///////////////////////
        osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
        uint8_t messageBlock[] = "\r\nType characters, they are received and re-sent (printed) with BLOCKING functions:\r\n";

        const Bool resB = osg_Uart_sendBlocking(&uart2, messageBlock, sizeof(messageBlock) / sizeof(messageBlock[0]), 2000);
        if (resB == FALSE)
        {
            osg_Led_startBlinking(&greenLed, 250);
            osg_baseHw_wait(1000);
        }
        else 
        {
            while (count < 10)
            {    
                uint8_t ch = 0;
                const Bool resRecv = osg_Uart_receiveBlocking(&uart2, &ch, 1, 100000);
                if (resRecv == FALSE)
                {
                    osg_Led_startBlinking(&greenLed, 250);
                    osg_baseHw_wait(1000);
                    break;
                }
                
                const Bool resSend = osg_Uart_sendBlocking(&uart2, &ch, 1, 200);
                if (resSend == FALSE)
                {
                    osg_Led_startBlinking(&greenLed, 250);
                    osg_baseHw_wait(1000);
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
        osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
        uint8_t messageNonB[] = "\r\nType characters, they are received and re-sent (printed) with NON-BLOCKING functions:\r\n";
        osg_Uart_setNbCallbacks(&uart2, nbTxCallback, nbRxCallback);
        setUart(&uart2);
        
        const Bool resN = osg_Uart_sendNonBlocking(&uart2, messageNonB, sizeof(messageNonB) / sizeof(messageNonB[0]));
        if (resN == FALSE)
        {
            osg_Led_startBlinking(&greenLed, 250);
            osg_baseHw_wait(1000);
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
        osg_Uart_ctor(&uart2, OSG_UART2, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, sendBuffer, sizeof(sendBuffer) / sizeof(sendBuffer[0]), receiveBuffer, sizeof(receiveBuffer) / sizeof(receiveBuffer[0]), OSG_GPIO_PA24, OSG_GPIO_PA25, OSG_GPIO_PA0, OSG_GPIO_PA0);
        uint8_t messageBuf[] = "\r\nType characters, they are received and re-sent (printed) with BUFFERED functions:\r\n";
        osg_Uart_setBufferedCallbacks(&uart2, bufTxCallback, bufRxCallback);
        setUart(&uart2);
        
        _bufSent = FALSE;
        const Bool resBuf = osg_Uart_sendBuffered(&uart2, messageBuf, sizeof(messageBuf) / sizeof(messageBuf[0]));
        if (resBuf == FALSE)
        {
            osg_Led_startBlinking(&greenLed, 250);
            osg_baseHw_wait(1000);
        }
        else
        {
            while (_bufSent == FALSE)
            {
            }
            
            osg_Uart_startReceiveBuffered(&uart2);
            
            while (osg_Uart_isReceiveBufferedEnabled(&uart2) == TRUE)
            {
            }
        }
        
        _bufCount = 0;
        _bufSent = FALSE;
        count = 0;
        osg_Uart_dtor(&uart2);
        osg_Led_stopBlinking(&greenLed);
    }
    
    for (;;);
}

void blinkingGreenLed()
{
    osg_Led greenLed;
    osg_Led_ctor(&greenLed, OSG_LED_GREEN);
    
    osg_Led_startBlinking(&greenLed, 1000);
    
    for(;;);
}

void blinkingRedLed()
{
    osg_Led redLed;
    osg_Led_ctor(&redLed, OSG_LED_RED);
    
    osg_Led_startBlinking(&redLed, 250);
    
    for(;;);
}
