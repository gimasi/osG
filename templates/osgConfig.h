#ifndef OSG_CONFIG_H
#define OSG_CONFIG_H

/// @file
/// This file exemplifies how to configure the oSG library

/// @name MCU configuration
/// This section lists the available MCUs.
/// @{
#define STM32_L071              1
#define STM32_L476              2
#define STM32_L4R5              3
#define NXP_MK28F15             4
/// @}

/// Set the correct MCU
#if defined(STM32L071xx)
    #define MCU_NAME            STM32_L071
    #include "stm32l0xx_hal_gpio_ex.h"
#elif defined(STM32L476xx)
    #define MCU_NAME            STM32_L476
    #include "stm32l4xx_hal_gpio_ex.h"
#elif defined(STM32L4R5xx)
    #define MCU_NAME            STM32_L4R5
    #include "stm32l4xx_hal_gpio_ex.h"
#elif defined(CPU_MK28FN2M0VMI15)
    #define MCU_NAME            NXP_MK28F15
    #include "fsl_port.h"
#else
    #error "Unsupported MCU"
#endif


/// @name Driver features
/// Each macro can hold:
/// - 1 to enable the feature;
/// - 0 to disable it.
/// Define the Alternative Function for pheripheral PINs.
/// @{
#define OSG_USE_I2C             1
#define OSG_USE_LED             1
#define OSG_USE_SPI             1
#define OSG_USE_TIMER           1
#define OSG_USE_UART            1
/// @}

/// @name I2C Alternative Function configuration.
/// Set the Alternative Function for pheripheral PINs.
/// @{
#define OSG_USE_I2C0_AF         0
#define OSG_USE_I2C1_AF         0
#define OSG_USE_I2C2_AF         0
#define OSG_USE_I2C3_AF         0
#define OSG_USE_I2C4_AF         0
#define OSG_USE_I2C5_AF         0
/// @}

/// @name SPI Alternative Function configuration.
/// Set the Alternative Function for pheripheral PINs.
/// @{
#define OSG_USE_SPI0_AF         0
#define OSG_USE_SPI1_AF         0
#define OSG_USE_SPI2_AF         0
#define OSG_USE_SPI3_AF         0
#define OSG_USE_SPI4_AF         0
#define OSG_USE_SPI5_AF         0
/// @}

/// @name UART Alternative Function configuration.
/// Set the Alternative Function for pheripheral PINs.
/// @{
#define OSG_USE_UART0_AF        0
#define OSG_USE_UART1_AF        0
#define OSG_USE_UART2_AF        0
#define OSG_USE_UART3_AF        0
#define OSG_USE_UART4_AF        0
#define OSG_USE_UART5_AF        0
/// @}

/// @name OS Names
/// List of available OS "names".
/// Set the OSG_OS_NAME to the name of the preferred one
/// @{
#define OSG_OS_NONE             0
#define OSG_OS_CMSIS_RTOS2      1
#define OSG_OS_NAME             OSG_OS_NONE
/// @}

/// @name OS features
/// Each macro can hold:
/// - 1 to enable the feature
/// - 0 to disable it
/// Note: OSG_OS_USE_TIMERS is usually set to the
/// same value of OSG_USE_TIMERS
/// @{
#define OSG_OS_USE_MESSAGES     1
#define OSG_OS_USE_MUTEX        1
#define OSG_OS_USE_THREAD       1
#define OSG_OS_USE_TIMER        OSG_USE_TIMER
/// @}


#endif
