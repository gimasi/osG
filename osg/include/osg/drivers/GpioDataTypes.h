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
    OSG_GPIO_PA0 = 0,
    OSG_GPIO_PA1 = 1,
    OSG_GPIO_PA2 = 2,
    OSG_GPIO_PA3 = 3,
    OSG_GPIO_PA4 = 4,
    OSG_GPIO_PA5 = 5,
    OSG_GPIO_PA6 = 6,
    OSG_GPIO_PA7 = 7,
    OSG_GPIO_PA8 = 8,
    OSG_GPIO_PA9 = 9,
    OSG_GPIO_PA10 = 10,
    OSG_GPIO_PA11 = 11,
    OSG_GPIO_PA12 = 12,
    OSG_GPIO_PA13 = 13,
    OSG_GPIO_PA14 = 14,
    OSG_GPIO_PA15 = 15,
    OSG_GPIO_PA16 = 16,
    OSG_GPIO_PA17 = 17,
    OSG_GPIO_PA18 = 18,
    OSG_GPIO_PA19 = 19,
    OSG_GPIO_PA20 = 20,
    OSG_GPIO_PA21 = 21,
    OSG_GPIO_PA22 = 22,
    OSG_GPIO_PA23 = 23,
    OSG_GPIO_PA24 = 24,
    OSG_GPIO_PA25 = 25,
    OSG_GPIO_PA26 = 26,
    OSG_GPIO_PA27 = 27,
    OSG_GPIO_PA28 = 28,
    OSG_GPIO_PA29 = 29,
    OSG_GPIO_PA30 = 30,
    OSG_GPIO_PA31 = 31,

    OSG_GPIO_PB0 = 32,
    OSG_GPIO_PB1 = 33,
    OSG_GPIO_PB2 = 34,
    OSG_GPIO_PB3 = 35,
    OSG_GPIO_PB4 = 36,
    OSG_GPIO_PB5 = 37,
    OSG_GPIO_PB6 = 38,
    OSG_GPIO_PB7 = 39,
    OSG_GPIO_PB8 = 40,
    OSG_GPIO_PB9 = 41,
    OSG_GPIO_PB10 = 42,
    OSG_GPIO_PB11 = 43,
    OSG_GPIO_PB12 = 44,
    OSG_GPIO_PB13 = 45,
    OSG_GPIO_PB14 = 46,
    OSG_GPIO_PB15 = 47,
    OSG_GPIO_PB16 = 48,
    OSG_GPIO_PB17 = 49,
    OSG_GPIO_PB18 = 50,
    OSG_GPIO_PB19 = 51,
    OSG_GPIO_PB20 = 52,
    OSG_GPIO_PB21 = 53,
    OSG_GPIO_PB22 = 54,
    OSG_GPIO_PB23 = 55,
    OSG_GPIO_PB24 = 56,
    OSG_GPIO_PB25 = 57,
    OSG_GPIO_PB26 = 58,
    OSG_GPIO_PB27 = 59,
    OSG_GPIO_PB28 = 60,
    OSG_GPIO_PB29 = 61,
    OSG_GPIO_PB30 = 62,
    OSG_GPIO_PB31 = 63,

    OSG_GPIO_PC0 = 64,
    OSG_GPIO_PC1 = 65,
    OSG_GPIO_PC2 = 66,
    OSG_GPIO_PC3 = 67,
    OSG_GPIO_PC4 = 68,
    OSG_GPIO_PC5 = 69,
    OSG_GPIO_PC6 = 70,
    OSG_GPIO_PC7 = 71,
    OSG_GPIO_PC8 = 72,
    OSG_GPIO_PC9 = 73,
    OSG_GPIO_PC10 = 74,
    OSG_GPIO_PC11 = 75,
    OSG_GPIO_PC12 = 76,
    OSG_GPIO_PC13 = 77,
    OSG_GPIO_PC14 = 78,
    OSG_GPIO_PC15 = 79,
    OSG_GPIO_PC16 = 80,
    OSG_GPIO_PC17 = 81,
    OSG_GPIO_PC18 = 82,
    OSG_GPIO_PC19 = 83,
    OSG_GPIO_PC20 = 84,
    OSG_GPIO_PC21 = 85,
    OSG_GPIO_PC22 = 86,
    OSG_GPIO_PC23 = 87,
    OSG_GPIO_PC24 = 88,
    OSG_GPIO_PC25 = 89,
    OSG_GPIO_PC26 = 90,
    OSG_GPIO_PC27 = 91,
    OSG_GPIO_PC28 = 92,
    OSG_GPIO_PC29 = 93,
    OSG_GPIO_PC30 = 94,
    OSG_GPIO_PC31 = 95,

    OSG_GPIO_PD0 = 96,
    OSG_GPIO_PD1 = 97,
    OSG_GPIO_PD2 = 98,
    OSG_GPIO_PD3 = 99,
    OSG_GPIO_PD4 = 100,
    OSG_GPIO_PD5 = 101,
    OSG_GPIO_PD6 = 102,
    OSG_GPIO_PD7 = 103,
    OSG_GPIO_PD8 = 104,
    OSG_GPIO_PD9 = 105,
    OSG_GPIO_PD10 = 106,
    OSG_GPIO_PD11 = 107,
    OSG_GPIO_PD12 = 108,
    OSG_GPIO_PD13 = 109,
    OSG_GPIO_PD14 = 110,
    OSG_GPIO_PD15 = 111,
    OSG_GPIO_PD16 = 112,
    OSG_GPIO_PD17 = 113,
    OSG_GPIO_PD18 = 114,
    OSG_GPIO_PD19 = 115,
    OSG_GPIO_PD20 = 116,
    OSG_GPIO_PD21 = 117,
    OSG_GPIO_PD22 = 118,
    OSG_GPIO_PD23 = 119,
    OSG_GPIO_PD24 = 120,
    OSG_GPIO_PD25 = 121,
    OSG_GPIO_PD26 = 122,
    OSG_GPIO_PD27 = 123,
    OSG_GPIO_PD28 = 124,
    OSG_GPIO_PD29 = 125,
    OSG_GPIO_PD30 = 126,
    OSG_GPIO_PD31 = 127,

    OSG_GPIO_PE0 = 128,
    OSG_GPIO_PE1 = 129,
    OSG_GPIO_PE2 = 130,
    OSG_GPIO_PE3 = 131,
    OSG_GPIO_PE4 = 132,
    OSG_GPIO_PE5 = 133,
    OSG_GPIO_PE6 = 134,
    OSG_GPIO_PE7 = 135,
    OSG_GPIO_PE8 = 136,
    OSG_GPIO_PE9 = 137,
    OSG_GPIO_PE10 = 138,
    OSG_GPIO_PE11 = 139,
    OSG_GPIO_PE12 = 140,
    OSG_GPIO_PE13 = 141,
    OSG_GPIO_PE14 = 142,
    OSG_GPIO_PE15 = 143,
    OSG_GPIO_PE16 = 144,
    OSG_GPIO_PE17 = 145,
    OSG_GPIO_PE18 = 146,
    OSG_GPIO_PE19 = 147,
    OSG_GPIO_PE20 = 148,
    OSG_GPIO_PE21 = 149,
    OSG_GPIO_PE22 = 150,
    OSG_GPIO_PE23 = 151,
    OSG_GPIO_PE24 = 152,
    OSG_GPIO_PE25 = 153,
    OSG_GPIO_PE26 = 154,
    OSG_GPIO_PE27 = 155,
    OSG_GPIO_PE28 = 156,
    OSG_GPIO_PE29 = 157,
    OSG_GPIO_PE30 = 158,
    OSG_GPIO_PE31 = 159,

    OSG_GPIO_PF0 = 160,
    OSG_GPIO_PF1 = 161,
    OSG_GPIO_PF2 = 162,
    OSG_GPIO_PF3 = 163,
    OSG_GPIO_PF4 = 164,
    OSG_GPIO_PF5 = 165,
    OSG_GPIO_PF6 = 166,
    OSG_GPIO_PF7 = 167,
    OSG_GPIO_PF8 = 168,
    OSG_GPIO_PF9 = 169,
    OSG_GPIO_PF10 = 170,
    OSG_GPIO_PF11 = 171,
    OSG_GPIO_PF12 = 172,
    OSG_GPIO_PF13 = 173,
    OSG_GPIO_PF14 = 174,
    OSG_GPIO_PF15 = 175,
    OSG_GPIO_PF16 = 176,
    OSG_GPIO_PF17 = 177,
    OSG_GPIO_PF18 = 178,
    OSG_GPIO_PF19 = 179,
    OSG_GPIO_PF20 = 180,
    OSG_GPIO_PF21 = 181,
    OSG_GPIO_PF22 = 182,
    OSG_GPIO_PF23 = 183,
    OSG_GPIO_PF24 = 184,
    OSG_GPIO_PF25 = 185,
    OSG_GPIO_PF26 = 186,
    OSG_GPIO_PF27 = 187,
    OSG_GPIO_PF28 = 188,
    OSG_GPIO_PF29 = 189,
    OSG_GPIO_PF30 = 190,
    OSG_GPIO_PF31 = 191,

    OSG_GPIO_PG0 = 192,
    OSG_GPIO_PG1 = 193,
    OSG_GPIO_PG2 = 194,
    OSG_GPIO_PG3 = 195,
    OSG_GPIO_PG4 = 196,
    OSG_GPIO_PG5 = 197,
    OSG_GPIO_PG6 = 198,
    OSG_GPIO_PG7 = 199,
    OSG_GPIO_PG8 = 200,
    OSG_GPIO_PG9 = 201,
    OSG_GPIO_PG10 = 202,
    OSG_GPIO_PG11 = 203,
    OSG_GPIO_PG12 = 204,
    OSG_GPIO_PG13 = 205,
    OSG_GPIO_PG14 = 206,
    OSG_GPIO_PG15 = 207,
    OSG_GPIO_PG16 = 208,
    OSG_GPIO_PG17 = 209,
    OSG_GPIO_PG18 = 210,
    OSG_GPIO_PG19 = 211,
    OSG_GPIO_PG20 = 212,
    OSG_GPIO_PG21 = 213,
    OSG_GPIO_PG22 = 214,
    OSG_GPIO_PG23 = 215,
    OSG_GPIO_PG24 = 216,
    OSG_GPIO_PG25 = 217,
    OSG_GPIO_PG26 = 218,
    OSG_GPIO_PG27 = 219,
    OSG_GPIO_PG28 = 220,
    OSG_GPIO_PG29 = 221,
    OSG_GPIO_PG30 = 222,
    OSG_GPIO_PG31 = 223,

    OSG_GPIO_PH0 = 224,
    OSG_GPIO_PH1 = 225,
    OSG_GPIO_PH2 = 226,
    OSG_GPIO_PH3 = 227,
    OSG_GPIO_PH4 = 228,
    OSG_GPIO_PH5 = 229,
    OSG_GPIO_PH6 = 230,
    OSG_GPIO_PH7 = 231,
    OSG_GPIO_PH8 = 232,
    OSG_GPIO_PH9 = 233,
    OSG_GPIO_PH10 = 234,
    OSG_GPIO_PH11 = 235,
    OSG_GPIO_PH12 = 236,
    OSG_GPIO_PH13 = 237,
    OSG_GPIO_PH14 = 238,
    OSG_GPIO_PH15 = 239,
    OSG_GPIO_PH16 = 240,
    OSG_GPIO_PH17 = 241,
    OSG_GPIO_PH18 = 242,
    OSG_GPIO_PH19 = 243,
    OSG_GPIO_PH20 = 244,
    OSG_GPIO_PH21 = 245,
    OSG_GPIO_PH22 = 246,
    OSG_GPIO_PH23 = 247,
    OSG_GPIO_PH24 = 248,
    OSG_GPIO_PH25 = 249,
    OSG_GPIO_PH26 = 250,
    OSG_GPIO_PH27 = 251,
    OSG_GPIO_PH28 = 252,
    OSG_GPIO_PH29 = 253,
    OSG_GPIO_PH30 = 254,
    OSG_GPIO_PH31 = 255,

    OSG_GPIO_PI0 = 256,
    OSG_GPIO_PI1 = 257,
    OSG_GPIO_PI2 = 258,
    OSG_GPIO_PI3 = 259,
    OSG_GPIO_PI4 = 260,
    OSG_GPIO_PI5 = 261,
    OSG_GPIO_PI6 = 262,
    OSG_GPIO_PI7 = 263,
    OSG_GPIO_PI8 = 264,
    OSG_GPIO_PI9 = 265,
    OSG_GPIO_PI10 = 266,
    OSG_GPIO_PI11 = 267,
    OSG_GPIO_PI12 = 268,
    OSG_GPIO_PI13 = 269,
    OSG_GPIO_PI14 = 270,
    OSG_GPIO_PI15 = 271,
    OSG_GPIO_PI16 = 272,
    OSG_GPIO_PI17 = 273,
    OSG_GPIO_PI18 = 274,
    OSG_GPIO_PI19 = 275,
    OSG_GPIO_PI20 = 276,
    OSG_GPIO_PI21 = 277,
    OSG_GPIO_PI22 = 278,
    OSG_GPIO_PI23 = 279,
    OSG_GPIO_PI24 = 280,
    OSG_GPIO_PI25 = 281,
    OSG_GPIO_PI26 = 282,
    OSG_GPIO_PI27 = 283,
    OSG_GPIO_PI28 = 284,
    OSG_GPIO_PI29 = 285,
    OSG_GPIO_PI30 = 286,
    OSG_GPIO_PI31 = 287
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

enum osg_GpioAlternateFunction;
typedef enum osg_GpioAlternateFunction osg_GpioAlternateFunction;
/// @brief Possible pins alternate functions
/// @ingroup Gpio
/// @memberof osg_Gpio
enum osg_GpioAlternateFunction
{
    OSG_GPIO_ALTERNATE_DEFAULT,
    OSG_GPIO_ALTERNATE_I2C0,
    OSG_GPIO_ALTERNATE_I2C1,
    OSG_GPIO_ALTERNATE_I2C2,
    OSG_GPIO_ALTERNATE_I2C3,
    OSG_GPIO_ALTERNATE_I2C4,
    OSG_GPIO_ALTERNATE_SPI0,
    OSG_GPIO_ALTERNATE_SPI1,
    OSG_GPIO_ALTERNATE_SPI2,
    OSG_GPIO_ALTERNATE_SPI3,
    OSG_GPIO_ALTERNATE_SPI4,
    OSG_GPIO_ALTERNATE_UART0,
    OSG_GPIO_ALTERNATE_UART1,
    OSG_GPIO_ALTERNATE_UART2,
    OSG_GPIO_ALTERNATE_UART3,
    OSG_GPIO_ALTERNATE_UART4,
    OSG_GPIO_ALTERNATE_UART5
    // @todo: copy stm32l4xx_hal_gpio_ex.h
};

#ifdef __cplusplus
}
#endif

#endif
