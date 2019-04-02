#if MCU_NAME == STM32_L476
    //#define BOARD_PATH ../../board/STM32/L476/src/
    #include "../../board/STM32/L476/src/board-BaseHw.c"
    #include "../../board/STM32/L476/src/drivers/board-Gpio.c"
    #include "../../board/STM32/L476/src/drivers/board-GpioSupport.c"
    #if OSG_USE_I2C == 1
        #include "../../board/STM32/L476/src/drivers/board-I2c.c"
        #include "../../board/STM32/L476/src/drivers/board-I2cSupport.c"
    #endif
    #if OSG_USE_LED == 1
        #include "../../board/STM32/L476/src/drivers/board-Led.c"
    #endif
    #if OSG_USE_SPI == 1
        #include "../../board/STM32/L476/src/drivers/board-Spi.c"
        #include "../../board/STM32/L476/src/drivers/board-SpiSupport.c"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../../board/STM32/L476/src/drivers/board-Timer.c"
        #include "../../board/STM32/L476/src/drivers/board-TimerSupport.c"
    #endif
    #if OSG_USE_UART == 1
        #include "../../board/STM32/L476/src/drivers/board-Uart.c"
        #include "../../board/STM32/L476/src/drivers/board-UartSupport.c"
    #endif
#elif MCU_NAME == STM32_L4R5
    //#define BOARD_PATH ../../board/STM32/L4R5/src/
    #include "../../board/STM32/L4R5/src/board-BaseHw.c"
    #include "../../board/STM32/L4R5/src/drivers/board-Gpio.c"
    #include "../../board/STM32/L4R5/src/drivers/board-GpioSupport.c"
    #if OSG_USE_I2C == 1
        #include "../../board/STM32/L4R5/src/drivers/board-I2c.c"
        #include "../../board/STM32/L4R5/src/drivers/board-I2cSupport.c"
    #endif
    #if OSG_USE_LED == 1
        #include "../../board/STM32/L4R5/src/drivers/board-Led.c"
    #endif
    #if OSG_USE_SPI == 1
        #include "../../board/STM32/L4R5/src/drivers/board-Spi.c"
        #include "../../board/STM32/L4R5/src/drivers/board-SpiSupport.c"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../../board/STM32/L4R5/src/drivers/board-Timer.c"
        #include "../../board/STM32/L4R5/src/drivers/board-TimerSupport.c"
    #endif
    #if OSG_USE_UART == 1
        #include "../../board/STM32/L4R5/src/drivers/board-Uart.c"
        #include "../../board/STM32/L4R5/src/drivers/board-UartSupport.c"
    #endif
#elif MCU_NAME == STM32_L071
    //#define BOARD_PATH ../../board/STM32/L071/src/
    #include "../../board/STM32/L071/src/board-BaseHw.c"
    #include "../../board/STM32/L071/src/drivers/board-Gpio.c"
    #include "../../board/STM32/L071/src/drivers/board-GpioSupport.c"
    #if OSG_USE_I2C == 1
        #include "../../board/STM32/L071/src/drivers/board-I2c.c"
        #include "../../board/STM32/L071/src/drivers/board-I2cSupport.c"
    #endif
    #if OSG_USE_LED == 1
        #include "../../board/STM32/L071/src/drivers/board-Led.c"
    #endif
    #if OSG_USE_SPI == 1
        #include "../../board/STM32/L071/src/drivers/board-Spi.c"
        #include "../../board/STM32/L071/src/drivers/board-SpiSupport.c"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../../board/STM32/L071/src/drivers/board-Timer.c"
        #include "../../board/STM32/L071/src/drivers/board-TimerSupport.c"
    #endif
    #if OSG_USE_UART == 1
        #include "../../board/STM32/L071/src/drivers/board-Uart.c"
        #include "../../board/STM32/L071/src/drivers/board-UartSupport.c"
    #endif
#elif MCU_NAME == NXP_MK28F15
    //#define BOARD_PATH ../../board/NXP/MK28FN2M0VMI15/src/
    #include "../../board/NXP/MK28FN2M0VMI15/src/board-BaseHw.c"
    #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-Gpio.c"
    #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-GpioSupport.c"
    #if OSG_USE_I2C == 1
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-I2c.c"
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-I2cSupport.c"
    #endif
    #if OSG_USE_LED == 1
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-Led.c"
    #endif
    #if OSG_USE_SPI == 1
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-Spi.c"
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-SpiSupport.c"
    #endif
    #if OSG_USE_TIMER == 1
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-Timer.c"
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-TimerSupport.c"
    #endif
    #if OSG_USE_UART == 1
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-Uart.c"
        #include "../../board/NXP/MK28FN2M0VMI15/src/drivers/board-UartSupport.c"
    #endif
#else
    #error "INVALID MCU. Check the 'osgConfig.h' file"
#endif

// https://stackoverflow.com/questions/9096201/concatenate-string-in-c-include-filename
//#define BOARD_STRINGIFY(X) #X
//#define BOARD_FILE2(A, B) BOARD_STRINGIFY(A ## B)
//#define BOARD_FILE_PATHX(A, B) BOARD_FILE2(A, B)
//#define BOARD_FILE_PATH(A) BOARD_FILE_PATHX(BOARD_PATH, A)
//#define BOARD_FILE(A) BOARD_FILE_PATH(A)

//#include BOARD_FILE(board-BaseHw.c)
//#include BOARD_FILE(drivers/board-Gpio.c)

//#if OSG_USE_I2C == 1
//    #include BOARD_FILE(drivers/board-I2c.c)
//#endif

//#include BOARD_FILE(drivers/board-Led.c)

//#if OSG_USE_SPI == 1
//    #include BOARD_FILE(drivers/board-Spi.c)
//#endif

//#if OSG_USE_TIMER == 1
//    #include BOARD_FILE(drivers/board-Timer.c)
//#endif

//#if OSG_USE_UART == 1
//    #include BOARD_FILE(drivers/board-Uart.c)
//#endif
