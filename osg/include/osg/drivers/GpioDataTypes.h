// This file is part of osG.
//
// osG is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// osG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
//
// osG is also available under a commercial license.
// Please contact GIMASI at info@gimasi.ch for further information.
//
#ifndef OSG_DRIVERS_GPIODATATYPES_H
#define OSG_DRIVERS_GPIODATATYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum osg_GpioPort;
typedef enum osg_GpioPort osg_GpioPort;
/// @brief List of available GPIO ports
/// Related constant OSG_GPIO_PORT_NUMBER is the total number of ports.
/// @ingroup Gpio
/// @memberof osg_Gpio
enum osg_GpioPort
{
    OSG_GPIO_PORT_NONE,
    OSG_GPIO_PORT_A,
    OSG_GPIO_PORT_B,
    OSG_GPIO_PORT_C,
    OSG_GPIO_PORT_D,
    OSG_GPIO_PORT_E,
    OSG_GPIO_PORT_F,
    OSG_GPIO_PORT_G,
    OSG_GPIO_PORT_H,
    OSG_GPIO_PORT_I
};
enum
{
    /// @brief Number of possible pins
    OSG_GPIO_PORT_NUMBER = OSG_GPIO_PORT_I + 1
};

enum osg_GpioPin;
typedef enum osg_GpioPin osg_GpioPin;
/// @brief List of available GPIO pins
/// Related constant OSG_GPIO_PIN_NUMBER is the total number of ports.
/// @ingroup Gpio
/// @memberof osg_Gpio
enum osg_GpioPin
{
    OSG_GPIO_PIN_NONE,
    OSG_GPIO_PIN_0,
    OSG_GPIO_PIN_1,
    OSG_GPIO_PIN_2,
    OSG_GPIO_PIN_3,
    OSG_GPIO_PIN_4,
    OSG_GPIO_PIN_5,
    OSG_GPIO_PIN_6,
    OSG_GPIO_PIN_7,
    OSG_GPIO_PIN_8,
    OSG_GPIO_PIN_9,
    OSG_GPIO_PIN_10,
    OSG_GPIO_PIN_11,
    OSG_GPIO_PIN_12,
    OSG_GPIO_PIN_13,
    OSG_GPIO_PIN_14,
    OSG_GPIO_PIN_15,
    OSG_GPIO_PIN_16,
    OSG_GPIO_PIN_17,
    OSG_GPIO_PIN_18,
    OSG_GPIO_PIN_19,
    OSG_GPIO_PIN_20,
    OSG_GPIO_PIN_21,
    OSG_GPIO_PIN_22,
    OSG_GPIO_PIN_23,
    OSG_GPIO_PIN_24,
    OSG_GPIO_PIN_25,
    OSG_GPIO_PIN_26,
    OSG_GPIO_PIN_27,
    OSG_GPIO_PIN_28,
    OSG_GPIO_PIN_29,
    OSG_GPIO_PIN_30,
    OSG_GPIO_PIN_31

};
enum
{
    /// @brief Number of possible pins
    OSG_GPIO_PIN_NUMBER = OSG_GPIO_PIN_31 + 1
};

enum osg_GpioId;
typedef enum osg_GpioId osg_GpioId;
/// @brief List of all available pins by ID.
/// Related constant OSG_GPIO_ID_NUMBER is the total number of ports.
/// @ingroup Gpio
/// @memberof osg_Gpio
/// @todo Add macros to remove non-available pins
enum osg_GpioId
{
    OSG_GPIO_NONE = 0,

    OSG_GPIO_PA0  =  1,
    OSG_GPIO_PA1  =  2,
    OSG_GPIO_PA2  =  3,
    OSG_GPIO_PA3  =  4,
    OSG_GPIO_PA4  =  5,
    OSG_GPIO_PA5  =  6,
    OSG_GPIO_PA6  =  7,
    OSG_GPIO_PA7  =  8,
    OSG_GPIO_PA8  =  9,
    OSG_GPIO_PA9  = 10,
    OSG_GPIO_PA10 = 11,
    OSG_GPIO_PA11 = 12,
    OSG_GPIO_PA12 = 13,
    OSG_GPIO_PA13 = 14,
    OSG_GPIO_PA14 = 15,
    OSG_GPIO_PA15 = 16,
    OSG_GPIO_PA16 = 17,
    OSG_GPIO_PA17 = 18,
    OSG_GPIO_PA18 = 19,
    OSG_GPIO_PA19 = 20,
    OSG_GPIO_PA20 = 21,
    OSG_GPIO_PA21 = 22,
    OSG_GPIO_PA22 = 23,
    OSG_GPIO_PA23 = 24,
    OSG_GPIO_PA24 = 25,
    OSG_GPIO_PA25 = 26,
    OSG_GPIO_PA26 = 27,
    OSG_GPIO_PA27 = 28,
    OSG_GPIO_PA28 = 29,
    OSG_GPIO_PA29 = 30,
    OSG_GPIO_PA30 = 31,
    OSG_GPIO_PA31 = 32,

    OSG_GPIO_PB0  = 33,
    OSG_GPIO_PB1  = 34,
    OSG_GPIO_PB2  = 35,
    OSG_GPIO_PB3  = 36,
    OSG_GPIO_PB4  = 37,
    OSG_GPIO_PB5  = 38,
    OSG_GPIO_PB6  = 39,
    OSG_GPIO_PB7  = 40,
    OSG_GPIO_PB8  = 41,
    OSG_GPIO_PB9  = 42,
    OSG_GPIO_PB10 = 43,
    OSG_GPIO_PB11 = 44,
    OSG_GPIO_PB12 = 45,
    OSG_GPIO_PB13 = 46,
    OSG_GPIO_PB14 = 47,
    OSG_GPIO_PB15 = 48,
    OSG_GPIO_PB16 = 49,
    OSG_GPIO_PB17 = 50,
    OSG_GPIO_PB18 = 51,
    OSG_GPIO_PB19 = 52,
    OSG_GPIO_PB20 = 53,
    OSG_GPIO_PB21 = 54,
    OSG_GPIO_PB22 = 55,
    OSG_GPIO_PB23 = 56,
    OSG_GPIO_PB24 = 57,
    OSG_GPIO_PB25 = 58,
    OSG_GPIO_PB26 = 59,
    OSG_GPIO_PB27 = 60,
    OSG_GPIO_PB28 = 61,
    OSG_GPIO_PB29 = 62,
    OSG_GPIO_PB30 = 63,
    OSG_GPIO_PB31 = 64,

    OSG_GPIO_PC0  = 65,
    OSG_GPIO_PC1  = 66,
    OSG_GPIO_PC2  = 67,
    OSG_GPIO_PC3  = 68,
    OSG_GPIO_PC4  = 69,
    OSG_GPIO_PC5  = 70,
    OSG_GPIO_PC6  = 71,
    OSG_GPIO_PC7  = 72,
    OSG_GPIO_PC8  = 73,
    OSG_GPIO_PC9  = 74,
    OSG_GPIO_PC10 = 75,
    OSG_GPIO_PC11 = 76,
    OSG_GPIO_PC12 = 77,
    OSG_GPIO_PC13 = 78,
    OSG_GPIO_PC14 = 79,
    OSG_GPIO_PC15 = 80,
    OSG_GPIO_PC16 = 81,
    OSG_GPIO_PC17 = 82,
    OSG_GPIO_PC18 = 83,
    OSG_GPIO_PC19 = 84,
    OSG_GPIO_PC20 = 85,
    OSG_GPIO_PC21 = 86,
    OSG_GPIO_PC22 = 87,
    OSG_GPIO_PC23 = 88,
    OSG_GPIO_PC24 = 89,
    OSG_GPIO_PC25 = 90,
    OSG_GPIO_PC26 = 91,
    OSG_GPIO_PC27 = 92,
    OSG_GPIO_PC28 = 93,
    OSG_GPIO_PC29 = 94,
    OSG_GPIO_PC30 = 95,
    OSG_GPIO_PC31 = 96,

    OSG_GPIO_PD0  =  97,
    OSG_GPIO_PD1  =  98,
    OSG_GPIO_PD2  =  99,
    OSG_GPIO_PD3  = 100,
    OSG_GPIO_PD4  = 101,
    OSG_GPIO_PD5  = 102,
    OSG_GPIO_PD6  = 103,
    OSG_GPIO_PD7  = 104,
    OSG_GPIO_PD8  = 105,
    OSG_GPIO_PD9  = 106,
    OSG_GPIO_PD10 = 107,
    OSG_GPIO_PD11 = 108,
    OSG_GPIO_PD12 = 109,
    OSG_GPIO_PD13 = 110,
    OSG_GPIO_PD14 = 111,
    OSG_GPIO_PD15 = 112,
    OSG_GPIO_PD16 = 113,
    OSG_GPIO_PD17 = 114,
    OSG_GPIO_PD18 = 115,
    OSG_GPIO_PD19 = 116,
    OSG_GPIO_PD20 = 117,
    OSG_GPIO_PD21 = 118,
    OSG_GPIO_PD22 = 119,
    OSG_GPIO_PD23 = 120,
    OSG_GPIO_PD24 = 121,
    OSG_GPIO_PD25 = 122,
    OSG_GPIO_PD26 = 123,
    OSG_GPIO_PD27 = 124,
    OSG_GPIO_PD28 = 125,
    OSG_GPIO_PD29 = 126,
    OSG_GPIO_PD30 = 127,
    OSG_GPIO_PD31 = 128,

    OSG_GPIO_PE0  = 129,
    OSG_GPIO_PE1  = 130,
    OSG_GPIO_PE2  = 131,
    OSG_GPIO_PE3  = 132,
    OSG_GPIO_PE4  = 133,
    OSG_GPIO_PE5  = 134,
    OSG_GPIO_PE6  = 135,
    OSG_GPIO_PE7  = 136,
    OSG_GPIO_PE8  = 137,
    OSG_GPIO_PE9  = 138,
    OSG_GPIO_PE10 = 139,
    OSG_GPIO_PE11 = 140,
    OSG_GPIO_PE12 = 141,
    OSG_GPIO_PE13 = 142,
    OSG_GPIO_PE14 = 143,
    OSG_GPIO_PE15 = 144,
    OSG_GPIO_PE16 = 145,
    OSG_GPIO_PE17 = 146,
    OSG_GPIO_PE18 = 147,
    OSG_GPIO_PE19 = 148,
    OSG_GPIO_PE20 = 149,
    OSG_GPIO_PE21 = 150,
    OSG_GPIO_PE22 = 151,
    OSG_GPIO_PE23 = 152,
    OSG_GPIO_PE24 = 153,
    OSG_GPIO_PE25 = 154,
    OSG_GPIO_PE26 = 155,
    OSG_GPIO_PE27 = 156,
    OSG_GPIO_PE28 = 157,
    OSG_GPIO_PE29 = 158,
    OSG_GPIO_PE30 = 159,
    OSG_GPIO_PE31 = 160,

    OSG_GPIO_PF0  = 161,
    OSG_GPIO_PF1  = 162,
    OSG_GPIO_PF2  = 163,
    OSG_GPIO_PF3  = 164,
    OSG_GPIO_PF4  = 165,
    OSG_GPIO_PF5  = 166,
    OSG_GPIO_PF6  = 167,
    OSG_GPIO_PF7  = 168,
    OSG_GPIO_PF8  = 169,
    OSG_GPIO_PF9  = 170,
    OSG_GPIO_PF10 = 171,
    OSG_GPIO_PF11 = 172,
    OSG_GPIO_PF12 = 173,
    OSG_GPIO_PF13 = 174,
    OSG_GPIO_PF14 = 175,
    OSG_GPIO_PF15 = 176,
    OSG_GPIO_PF16 = 177,
    OSG_GPIO_PF17 = 178,
    OSG_GPIO_PF18 = 179,
    OSG_GPIO_PF19 = 180,
    OSG_GPIO_PF20 = 181,
    OSG_GPIO_PF21 = 182,
    OSG_GPIO_PF22 = 183,
    OSG_GPIO_PF23 = 184,
    OSG_GPIO_PF24 = 185,
    OSG_GPIO_PF25 = 186,
    OSG_GPIO_PF26 = 187,
    OSG_GPIO_PF27 = 188,
    OSG_GPIO_PF28 = 189,
    OSG_GPIO_PF29 = 190,
    OSG_GPIO_PF30 = 191,
    OSG_GPIO_PF31 = 192,

    OSG_GPIO_PG0  = 193,
    OSG_GPIO_PG1  = 194,
    OSG_GPIO_PG2  = 195,
    OSG_GPIO_PG3  = 196,
    OSG_GPIO_PG4  = 197,
    OSG_GPIO_PG5  = 198,
    OSG_GPIO_PG6  = 199,
    OSG_GPIO_PG7  = 200,
    OSG_GPIO_PG8  = 201,
    OSG_GPIO_PG9  = 202,
    OSG_GPIO_PG10 = 203,
    OSG_GPIO_PG11 = 204,
    OSG_GPIO_PG12 = 205,
    OSG_GPIO_PG13 = 206,
    OSG_GPIO_PG14 = 207,
    OSG_GPIO_PG15 = 208,
    OSG_GPIO_PG16 = 209,
    OSG_GPIO_PG17 = 210,
    OSG_GPIO_PG18 = 211,
    OSG_GPIO_PG19 = 212,
    OSG_GPIO_PG20 = 213,
    OSG_GPIO_PG21 = 214,
    OSG_GPIO_PG22 = 215,
    OSG_GPIO_PG23 = 216,
    OSG_GPIO_PG24 = 217,
    OSG_GPIO_PG25 = 218,
    OSG_GPIO_PG26 = 219,
    OSG_GPIO_PG27 = 220,
    OSG_GPIO_PG28 = 221,
    OSG_GPIO_PG29 = 222,
    OSG_GPIO_PG30 = 223,
    OSG_GPIO_PG31 = 224,

    OSG_GPIO_PH0  = 225,
    OSG_GPIO_PH1  = 226,
    OSG_GPIO_PH2  = 227,
    OSG_GPIO_PH3  = 228,
    OSG_GPIO_PH4  = 229,
    OSG_GPIO_PH5  = 230,
    OSG_GPIO_PH6  = 231,
    OSG_GPIO_PH7  = 232,
    OSG_GPIO_PH8  = 233,
    OSG_GPIO_PH9  = 234,
    OSG_GPIO_PH10 = 235,
    OSG_GPIO_PH11 = 236,
    OSG_GPIO_PH12 = 237,
    OSG_GPIO_PH13 = 238,
    OSG_GPIO_PH14 = 239,
    OSG_GPIO_PH15 = 240,
    OSG_GPIO_PH16 = 241,
    OSG_GPIO_PH17 = 242,
    OSG_GPIO_PH18 = 243,
    OSG_GPIO_PH19 = 244,
    OSG_GPIO_PH20 = 245,
    OSG_GPIO_PH21 = 246,
    OSG_GPIO_PH22 = 247,
    OSG_GPIO_PH23 = 248,
    OSG_GPIO_PH24 = 249,
    OSG_GPIO_PH25 = 250,
    OSG_GPIO_PH26 = 251,
    OSG_GPIO_PH27 = 252,
    OSG_GPIO_PH28 = 253,
    OSG_GPIO_PH29 = 254,
    OSG_GPIO_PH30 = 255,
    OSG_GPIO_PH31 = 256,

    OSG_GPIO_PI0  = 257,
    OSG_GPIO_PI1  = 258,
    OSG_GPIO_PI2  = 259,
    OSG_GPIO_PI3  = 260,
    OSG_GPIO_PI4  = 261,
    OSG_GPIO_PI5  = 262,
    OSG_GPIO_PI6  = 263,
    OSG_GPIO_PI7  = 264,
    OSG_GPIO_PI8  = 265,
    OSG_GPIO_PI9  = 266,
    OSG_GPIO_PI10 = 267,
    OSG_GPIO_PI11 = 268,
    OSG_GPIO_PI12 = 269,
    OSG_GPIO_PI13 = 270,
    OSG_GPIO_PI14 = 271,
    OSG_GPIO_PI15 = 272,
    OSG_GPIO_PI16 = 273,
    OSG_GPIO_PI17 = 274,
    OSG_GPIO_PI18 = 275,
    OSG_GPIO_PI19 = 276,
    OSG_GPIO_PI20 = 277,
    OSG_GPIO_PI21 = 278,
    OSG_GPIO_PI22 = 279,
    OSG_GPIO_PI23 = 280,
    OSG_GPIO_PI24 = 281,
    OSG_GPIO_PI25 = 282,
    OSG_GPIO_PI26 = 283,
    OSG_GPIO_PI27 = 284,
    OSG_GPIO_PI28 = 285,
    OSG_GPIO_PI29 = 286,
    OSG_GPIO_PI30 = 287,
    OSG_GPIO_PI31 = 289
};

enum
{
    /// @brief Number of total possible pin IDs
    OSG_GPIO_ID_NUMBER =  OSG_GPIO_PI31 + 1
};



struct osg_Gpio;
typedef struct osg_Gpio osg_Gpio;
/// @brief Pin class.
/// @ingroup Gpio
/// @memberof osg_Gpio
struct osg_Gpio
{
    /// @brief Low level handler
    /// @private
    void * handler;
    /// @brief The GPIO ID
    /// @private
    osg_GpioId id;
    /// @brief The GPIO Alternative Function
    /// @private
    uint16_t altFunc;
};

enum osg_GpioMode;
typedef enum osg_GpioMode osg_GpioMode;
/// @brief Pins available modes
/// @ingroup Gpio
/// @memberof osg_Gpio
enum osg_GpioMode
{
    OSG_GPIO_MODE_INPUT,
    OSG_GPIO_MODE_OUTPUT_PP,
    OSG_GPIO_MODE_OUTPUT_OD,
    OSG_GPIO_MODE_AF_PP,
    OSG_GPIO_MODE_AF_OD,
    OSG_GPIO_MODE_ANALOG,
    OSG_GPIO_MODE_ANALOG_ADC_CONTROL,
    OSG_GPIO_MODE_IT_RISING,
    OSG_GPIO_MODE_IT_FALLING,
    OSG_GPIO_MODE_IT_RISING_FALLING,
    OSG_GPIO_MODE_IT_LOGIC_0,
    OSG_GPIO_MODE_IT_LOGIC_1,
    OSG_GPIO_MODE_EVT_RISING,
    OSG_GPIO_MODE_EVT_FALLING,
    OSG_GPIO_MODE_EVT_RISING_FALLING
};

enum osg_GpioPull;
typedef enum osg_GpioPull osg_GpioPull;
/// @brief Pins pull options
/// @ingroup Gpio
/// @memberof osg_Gpio
enum osg_GpioPull
{
    OSG_GPIO_NOPULL,
    OSG_GPIO_PULLUP,
    OSG_GPIO_PULLDOWN
};

enum osg_GpioSpeed;
typedef enum osg_GpioSpeed osg_GpioSpeed;
/// @brief Pins speeds
/// @ingroup Gpio
/// @memberof osg_Gpio
enum osg_GpioSpeed
{
    OSG_GPIO_SPEED_FREQ_LOW,
    OSG_GPIO_SPEED_FREQ_MEDIUM,
    OSG_GPIO_SPEED_FREQ_HIGH,
    OSG_GPIO_SPEED_FREQ_VERY_HIGH
};

// enum osg_GpioAlternateFunction;
// typedef enum osg_GpioAlternateFunction osg_GpioAlternateFunction;
// /// @brief Possible pins alternate functions
// /// @ingroup Gpio
// /// @memberof osg_Gpio
// enum osg_GpioAlternateFunction
// {
//     OSG_GPIO_ALTERNATE_DEFAULT,
//     OSG_GPIO_ALTERNATE_I2C0,
//     OSG_GPIO_ALTERNATE_I2C1,
//     OSG_GPIO_ALTERNATE_I2C2,
//     OSG_GPIO_ALTERNATE_I2C3,
//     OSG_GPIO_ALTERNATE_I2C4,
//     OSG_GPIO_ALTERNATE_SPI0_CLK,
//     OSG_GPIO_ALTERNATE_SPI0_MOSI,
//     OSG_GPIO_ALTERNATE_SPI0_MISO,
//     OSG_GPIO_ALTERNATE_SPI0_SS,
//     OSG_GPIO_ALTERNATE_SPI1_CLK,
//     OSG_GPIO_ALTERNATE_SPI1_MOSI,
//     OSG_GPIO_ALTERNATE_SPI1_MISO,
//     OSG_GPIO_ALTERNATE_SPI1_SS,
//     OSG_GPIO_ALTERNATE_SPI2_CLK,
//     OSG_GPIO_ALTERNATE_SPI2_MOSI,
//     OSG_GPIO_ALTERNATE_SPI2_MISO,
//     OSG_GPIO_ALTERNATE_SPI2_SS,
//     OSG_GPIO_ALTERNATE_SPI3_CLK,
//     OSG_GPIO_ALTERNATE_SPI3_MOSI,
//     OSG_GPIO_ALTERNATE_SPI3_MISO,
//     OSG_GPIO_ALTERNATE_SPI3_SS,
//     OSG_GPIO_ALTERNATE_SPI4_CLK,
//     OSG_GPIO_ALTERNATE_SPI4_MOSI,
//     OSG_GPIO_ALTERNATE_SPI4_MISO,
//     OSG_GPIO_ALTERNATE_SPI4_SS,
//     OSG_GPIO_ALTERNATE_UART0,
//     OSG_GPIO_ALTERNATE_UART1,
//     OSG_GPIO_ALTERNATE_UART2,
//     OSG_GPIO_ALTERNATE_UART3,
//     OSG_GPIO_ALTERNATE_UART4,
//     OSG_GPIO_ALTERNATE_UART5
//     // @todo: copy stm32l4xx_hal_gpio_ex.h
// };

struct osg_GpioConfig;
typedef struct osg_GpioConfig osg_GpioConfig;
/// @brief Pin config class.
/// @ingroup Gpio
/// @memberof osg_Gpio
struct osg_GpioConfig
{
    /// @brief id The pin id
    osg_GpioId id;
    /// @brief mode The pin mode
    osg_GpioMode mode;
    /// @brief pull The pin pull mode
    osg_GpioPull pull;
    /// @brief speed The pin speed
    osg_GpioSpeed speed;
    /// @brief alternate The pin alternate function
    uint16_t alternate;
};

#ifdef __cplusplus
}
#endif

#endif
