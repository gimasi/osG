#include <osg.h>

// ///////////////
// STM32L4
//  - USART3:
//     o PC10 -> TX
//     o PC11 -> RX
// ///////////////

// Support variables and functions
Bool sent;
Bool received;
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
    const Bool startOk = osg_baseHw_start(uartAllTest);
    osg_assert(startOk == TRUE, "ASSERT FAILED: Starting osG application.");
    
	return 0;
}

void uartBlockingSend()
{
    osg_Uart uart3;
    osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[] = "Hello osG!\r\n";
    
    const Bool res = osg_Uart_sendBlocking(&uart3, message, sizeof(message) / sizeof(message[0]), 2000);
    if (res == FALSE)
    {
        blinkingGreenLed();
    }
    
    turnOnGreenLed();
    
    for (;;);
}

void uartBlockingReceive()
{
    osg_Uart uart3;
    osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[12];
    
    const Bool res = osg_Uart_receiveBlocking(&uart3, message, sizeof(message) / sizeof(message[0]), 10000);
    if (res == FALSE)
    {
        blinkingGreenLed();
    }
    
    turnOnGreenLed();
    
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
    osg_Uart uart3;
    osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setNbCallbacks(&uart3, nonBlockingSendCallback, NULL);
    
    sent = FALSE;
    const Bool res = osg_Uart_sendNonBlocking(&uart3, message, sizeof(message) / sizeof(message[0]));
    if (res == FALSE)
    {
        blinkingGreenLed();
    }
    
    while (sent == FALSE)
    {
    }
    
    turnOnGreenLed();
    
    for (;;);
}

void uartNonBlockingReceive()
{
    osg_Uart uart3;
    osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[12];
    osg_Uart_setNbCallbacks(&uart3, NULL, nonBlockingReceiveCallback);
    
    received = FALSE;
    const Bool res = osg_Uart_receiveNonBlocking(&uart3, message, sizeof(message) / sizeof(message[0]));
    if (res == FALSE)
    {
        blinkingGreenLed();
    }
    
    while (received == FALSE)
    {
    }
    
    turnOnGreenLed();
    
    for(;;);
}

void bufferedSendCallback(void)
{
    sent = TRUE;
}

void uartBufferedSend()
{
    uint8_t sendBuffer[20];
    osg_Uart uart3;
    osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, sendBuffer, sizeof(sendBuffer) / sizeof(sendBuffer[0]), NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
    uint8_t message[] = "Hello osG!\r\n";
    osg_Uart_setBufferedCallbacks(&uart3, bufferedSendCallback, NULL);
    
    sent = FALSE;
    const Bool res = osg_Uart_sendBuffered(&uart3, message, sizeof(message) / sizeof(message[0]));
    if (res == FALSE)
    {
        blinkingGreenLed();
    }
    
    while (sent == FALSE)
    {
    }
    
    turnOnGreenLed();
    
    for (;;);
}

void uartBufferedReceive()
{
    uint8_t receiveBuffer[20];
    osg_Uart uart3;
    osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, receiveBuffer, sizeof(receiveBuffer) / sizeof(receiveBuffer[0]), OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
    
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
    osg_Uart * uart3 = getUart();
    const Bool resSend = osg_Uart_sendNonBlocking(uart3, &_nbRxChar, 1);
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
        osg_Uart * uart3 = getUart();
        const Bool resRecv = osg_Uart_receiveNonBlocking(uart3, &_nbRxChar, 1);
        if (resRecv == FALSE)
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
        
        const Bool resSend = osg_Uart_sendBuffered(uart3, buffer, recv);
        if (resSend == TRUE)
            _bufCount += recv;
    }
}

void bufTxCallback(void)
{
    _bufSent = TRUE;
    
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
    osg_Led_ctor(&greenLed, OSG_LED_GREEN);
    
    for (;;)
    {
        // ///////////////////////
        // BLOCKING SECTION
        // ///////////////////////
        osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
        uint8_t messageBlock[] = "\r\nType characters, they are received and re-sent (printed) with BLOCKING functions:\r\n";

        const Bool resB = osg_Uart_sendBlocking(&uart3, messageBlock, sizeof(messageBlock) / sizeof(messageBlock[0]), 2000);
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
                const Bool resRecv = osg_Uart_receiveBlocking(&uart3, &ch, 1, 100000);
                if (resRecv == FALSE)
                {
                    osg_Led_startBlinking(&greenLed, 250);
                    osg_baseHw_wait(1000);
                    break;
                }
                
                const Bool resSend = osg_Uart_sendBlocking(&uart3, &ch, 1, 200);
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
        osg_Uart_dtor(&uart3);
        osg_Led_stopBlinking(&greenLed);

        // ///////////////////////
        // NON-BLOCKING SECTION
        // ///////////////////////
        osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, NULL, 0, NULL, 0, OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
        uint8_t messageNonB[] = "\r\nType characters, they are received and re-sent (printed) with NON-BLOCKING functions:\r\n";
        osg_Uart_setNbCallbacks(&uart3, nbTxCallback, nbRxCallback);
        setUart(&uart3);
        
        const Bool resN = osg_Uart_sendNonBlocking(&uart3, messageNonB, sizeof(messageNonB) / sizeof(messageNonB[0]));
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
        osg_Uart_dtor(&uart3);
        osg_Led_stopBlinking(&greenLed);
        
        // ///////////////////////
        // BUFFERED SECTION
        // ///////////////////////
        uint8_t sendBuffer[90];
        uint8_t receiveBuffer[90];
        osg_Uart_ctor(&uart3, OSG_UART3, OSG_UART_BAUDRATE_9600, OSG_UART_PARITY_NONE, OSG_UART_BITS_8, OSG_UART_STOPBITS_1, OSG_UART_FLOWCONTROL_NONE, sendBuffer, sizeof(sendBuffer) / sizeof(sendBuffer[0]), receiveBuffer, sizeof(receiveBuffer) / sizeof(receiveBuffer[0]), OSG_GPIO_PC10, OSG_GPIO_PC11, OSG_GPIO_PA0, OSG_GPIO_PA0);
        uint8_t messageBuf[] = "\r\nType characters, they are received and re-sent (printed) with BUFFERED functions:\r\n";
        osg_Uart_setBufferedCallbacks(&uart3, bufTxCallback, bufRxCallback);
        setUart(&uart3);
        
        _bufSent = FALSE;
        const Bool resBuf = osg_Uart_sendBuffered(&uart3, messageBuf, sizeof(messageBuf) / sizeof(messageBuf[0]));
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
            
            osg_Uart_startReceiveBuffered(&uart3);
            
            while (osg_Uart_isReceiveBufferedEnabled(&uart3) == TRUE)
            {
            }
        }
        
        _bufCount = 0;
        _bufSent = FALSE;
        count = 0;
        osg_Uart_dtor(&uart3);
        osg_Led_stopBlinking(&greenLed);
    }
    
    for (;;);
}

void blinkingGreenLed()
{
    osg_Led greenLed;
    osg_Led_ctor(&greenLed, OSG_LED_GREEN);
    
    osg_Led_startBlinking(&greenLed, 250);
    
    for(;;);
}

void turnOnGreenLed()
{
    osg_Led greenLed;
    osg_Led_ctor(&greenLed, OSG_LED_GREEN);
    
    osg_Led_turnOn(&greenLed);
    
    for(;;);
}
