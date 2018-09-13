#include <osg.h>
#include <string.h>

// ////////////////////////////////////
// SLAVE
// ////////////////////////////////////
#define STM_BOARD   "STM"
#define NXP_BOARD   "NXP"
#define STM_ADDR    0x10
#define NXP_ADDR    0x08
#if defined(STM32L476xx)
#define BOARD STM_BOARD
#define ADDR STM_ADDR
#elif defined(CPU_MK28FN2M0VMI15)
#define BOARD NXP_BOARD
#define ADDR NXP_ADDR
#else
#define BOARD "UNKNOWN"
define ADDR 0xFF
#endif

static Bool send = FALSE;
static Bool recv = FALSE;

void handleI2cErrors(void)
{
    osg_error("ERROR: I2C communication error.");
}

void handleI2cTx(void)
{
    send = TRUE;
}

void handleI2cRx(void)
{
    recv = TRUE;
}


static void testSlaveBlockingReceive(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[10];
    memset(buff, 0, sizeof(buff));
    const Bool recvOk = osg_I2c_receiveBlocking(i2c, ADDR, buff, 10, 5000);
    if (recvOk == TRUE)
    {
        if (BOARD == STM_BOARD)
            osg_Led_turnOn(greenLed);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(greenLed, 500);
    }
    else
    {
        if (BOARD == STM_BOARD)
            osg_Led_startBlinking(greenLed, 500);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(redLed, 200);
    }
}

static void nbRxClbk(void)
{
    recv = TRUE;
}

static void testSlaveNonBlockingReceive(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[10];
    memset(buff, 0, sizeof(buff));
    osg_I2c_setNbCallbacks(i2c, NULL, nbRxClbk);
    
    recv = FALSE;
    const Bool recvOk = osg_I2c_receiveNonBlocking(i2c, ADDR, buff, 10);
    if (recvOk == TRUE)
    {
        if (BOARD == STM_BOARD)
            osg_Led_turnOn(greenLed);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(blueLed, 500);
    }
    else
    {
        if (BOARD == STM_BOARD)
            osg_Led_startBlinking(greenLed, 500);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(redLed, 200);
        
        return;
    }
    
    while (recv == FALSE)
    {
    }
    
    if (BOARD == STM_BOARD)
    {
        osg_Led_turnOff(greenLed);
        osg_baseHw_wait(500);
        osg_Led_turnOn(greenLed);
    }
    else if (BOARD == NXP_BOARD)
    {
        osg_Led_stopBlinking(blueLed);
        osg_Led_turnOff(blueLed);
        osg_Led_startBlinking(greenLed, 500);
    }
}

static void bufRxClbk(void)
{
    recv = TRUE;
}

static void testSlaveBufferedReceive(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t message[10];
    memset(message, 0, sizeof(message));
    osg_I2c_setBufferedCallbacks(i2c, NULL, bufRxClbk);
    
    recv = FALSE;
    osg_I2c_startReceiveBuffered(i2c, ADDR);
    if (BOARD == STM_BOARD)
        osg_Led_turnOn(greenLed);
    else if (BOARD == NXP_BOARD)
        osg_Led_startBlinking(blueLed, 500);
    
    while (recv == FALSE)
    {
    }

    const Size recvOk = osg_I2c_receiveBuffered(i2c, message, 1);
    if (recvOk == 1)
    {
        if (BOARD == STM_BOARD)
        {
            osg_Led_turnOff(greenLed);
            osg_baseHw_wait(500);
            osg_Led_turnOn(greenLed);
        }
        else if (BOARD == NXP_BOARD)
        {
            osg_Led_stopBlinking(blueLed);
            osg_Led_turnOff(blueLed);
            osg_Led_startBlinking(greenLed, 500);
        }
    }
    else
    {
        if (BOARD == STM_BOARD)
            osg_Led_startBlinking(greenLed, 500);
        else if (BOARD == NXP_BOARD)
        {
            osg_Led_stopBlinking(blueLed);
            osg_Led_turnOff(blueLed);
            osg_Led_startBlinking(redLed, 200);
        }
    }
    
    osg_I2c_stopReceiveBuffered(i2c);

}

static void testSlaveBlockingSend(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[] = "Hello osG!";
    const Bool sendOk = osg_I2c_sendBlocking(i2c, ADDR, buff, 10, 5000);
    if (sendOk == TRUE)
    {
        if (BOARD == STM_BOARD)
            osg_Led_turnOn(greenLed);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(greenLed, 500);
    }
    else
    {
        if (BOARD == STM_BOARD)
            osg_Led_startBlinking(greenLed, 500);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(redLed, 200);
    }
}

static void nbTxClbk(void)
{
    send = TRUE;
}

static void testSlaveNonBlockingSend(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t buff[] = "Hello osG!";
    osg_I2c_setNbCallbacks(i2c, nbTxClbk, NULL);
    
    send = FALSE;
    const Bool sendOk = osg_I2c_sendNonBlocking(i2c, ADDR, buff, 10);
    if (sendOk == TRUE)
    {
        if (BOARD == STM_BOARD)
            osg_Led_turnOn(greenLed);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(blueLed, 500);
    }
    else
    {
        if (BOARD == STM_BOARD)
            osg_Led_startBlinking(greenLed, 500);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(redLed, 200);
        
        return;
    }
    
    while (send == FALSE)
    {
    }
    
    if (BOARD == STM_BOARD)
    {
        osg_Led_turnOff(greenLed);
        osg_baseHw_wait(500);
        osg_Led_turnOn(greenLed);
    }
    else if (BOARD == NXP_BOARD)
    {
        osg_Led_stopBlinking(blueLed);
        osg_Led_turnOff(blueLed);
        osg_Led_startBlinking(greenLed, 500);
    }
}

static void bufTxClbk(void)
{
    send = TRUE;
}

static void testSlaveBufferedSend(osg_I2c* i2c, osg_Led* greenLed, osg_Led* redLed, osg_Led* blueLed)
{
    uint8_t message[] = "Hello osG!";
    osg_I2c_setBufferedCallbacks(i2c, bufTxClbk, NULL);
    
    send = FALSE;
    const Bool sendOk = osg_I2c_sendBuffered(i2c, ADDR, message, 10);
    if (sendOk == TRUE)
    {
        if (BOARD == STM_BOARD)
            osg_Led_turnOn(greenLed);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(blueLed, 500);
    }
    else
    {
        if (BOARD == STM_BOARD)
            osg_Led_startBlinking(greenLed, 500);
        else if (BOARD == NXP_BOARD)
            osg_Led_startBlinking(redLed, 200);
        
        return;
    }
    
    while (send == FALSE)
    {
    }
    
    if (BOARD == STM_BOARD)
    {
        osg_Led_turnOff(greenLed);
        osg_baseHw_wait(500);
        osg_Led_turnOn(greenLed);
    }
    else if (BOARD == NXP_BOARD)
    {
        osg_Led_stopBlinking(blueLed);
        osg_Led_turnOff(blueLed);
        osg_Led_startBlinking(greenLed, 500);
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

    osg_Led_ctor(&greenLed, OSG_LED_GREEN);
    if (BOARD == NXP_BOARD)
    {
        osg_Led_ctor(&redLed, OSG_LED_RED);
        osg_Led_ctor(&blueLed, OSG_LED_BLUE);
    }
    
    if (BOARD == STM_BOARD)
    {
        // STM32L476 init
        osg_I2c_ctor(&i2c, OSG_I2C1, OSG_I2C_SLAVE_MODE, 0x00000D15, OSG_I2C_7BIT_ADDRESS, FALSE, ADDR,
                        bufferTx, 20,
                        bufferRx, 20,
                        OSG_GPIO_PB9, OSG_GPIO_PB8);

    }
    else if (BOARD == NXP_BOARD)
    {
        // NXP MK28F15 init
        osg_I2c_ctor(&i2c, OSG_I2C0, OSG_I2C_SLAVE_MODE, 100000, OSG_I2C_7BIT_ADDRESS, FALSE, ADDR,
                        bufferTx, 20,
                        bufferRx, 20,
                        OSG_GPIO_PB3, OSG_GPIO_PB2);
    }
    else
    {
        osg_error("ERROR: Unknown board!");
    }
    
    //testSlaveBlockingReceive(&i2c, &greenLed, &redLed, &blueLed);
    
    testSlaveNonBlockingReceive(&i2c, &greenLed, &redLed, &blueLed);
    
    //testSlaveBufferedReceive(&i2c, &greenLed, &redLed, &blueLed);
    
    //testSlaveBlockingSend(&i2c, &greenLed, &redLed, &blueLed);
    
    //testSlaveNonBlockingSend(&i2c, &greenLed, &redLed, &blueLed);
    
    //testSlaveBufferedSend(&i2c, &greenLed, &redLed, &blueLed);
    
    for (;;);
    
    osg_I2c_dtor(&i2c);
    osg_Led_dtor(&greenLed);
    if (BOARD == NXP_BOARD)
    {
        osg_Led_dtor(&redLed);
        osg_Led_dtor(&blueLed);
    }
}

int main(void)
{
    const Bool res = osg_baseHw_start(applicationUserCode);
    osg_assert(res == TRUE, "ASSERT FAILED: HW base initialization failed.");
    
    return 0;
}
