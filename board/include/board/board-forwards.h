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
#ifndef OSG_BOARD_FORWARDS_H
#define OSG_BOARD_FORWARDS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// /////////////////////////////////////////////////////////////////////////////
// Data types
// /////////////////////////////////////////////////////////////////////////////

enum Bool;
typedef enum Bool Bool;

enum osg_IrqPriority;
typedef enum osg_IrqPriority osg_IrqPriority;

enum osg_PinMax;
typedef enum osg_PinMax osg_PinMax;

typedef size_t Size;

typedef uint8_t Byte;

struct osg_CircularFifo;
typedef struct osg_CircularFifo osg_CircularFifo;


// /////////////////////////////////////////////////////////////////////////////
// BaseHw
// /////////////////////////////////////////////////////////////////////////////

typedef void (*osg_BaseHwApplication)(void);

// /////////////////////////////////////////////////////////////////////////////
// Leds
// /////////////////////////////////////////////////////////////////////////////

enum osg_LedId;
typedef enum osg_LedId osg_LedId;

// /////////////////////////////////////////////////////////////////////////////
// Gpio
// /////////////////////////////////////////////////////////////////////////////

enum osg_GpioId;
typedef enum osg_GpioId osg_GpioId;

struct osg_Gpio;
typedef struct osg_Gpio osg_Gpio;

enum osg_GpioPin;
typedef enum osg_GpioPin osg_GpioPin;

enum osg_GpioMode;
typedef enum osg_GpioMode osg_GpioMode;

enum osg_GpioPull;
typedef enum osg_GpioPull osg_GpioPull;

enum osg_GpioSpeed;
typedef enum osg_GpioSpeed osg_GpioSpeed;

enum osg_GpioAlternateFunction;
typedef enum osg_GpioAlternateFunction osg_GpioAlternateFunction;

typedef void (*osg_GpioInterruptCallback)(const osg_GpioPin);

// /////////////////////////////////////////////////////////////////////////////
// Timer
// /////////////////////////////////////////////////////////////////////////////

struct osg_Timer;
typedef struct osg_Timer osg_Timer;

enum osg_TimerType;
typedef enum osg_TimerType osg_TimerType;

typedef void (*osg_TimerCallback)(void *);

// /////////////////////////////////////////////////////////////////////////////
// Thread
// /////////////////////////////////////////////////////////////////////////////

enum osg_ThreadState;
typedef enum osg_ThreadState osg_ThreadState;

struct osg_Thread;
typedef struct osg_Thread osg_Thread;

typedef void (*osg_ThreadFunction)(void *);

// /////////////////////////////////////////////////////////////////////////////
// I2C
// /////////////////////////////////////////////////////////////////////////////

enum osg_I2cMode;
typedef enum osg_I2cMode osg_I2cMode;

enum osg_I2cId;
typedef enum osg_I2cId osg_I2cId;

enum osg_I2cAddressingMode;
typedef enum osg_I2cAddressingMode osg_I2cAddressingMode;

enum osg_I2cFlags;
typedef enum osg_I2cFlags osg_I2cFlags;

typedef void (* osg_I2cCallback)(void);

struct osg_I2cImpl;
typedef struct osg_I2cImpl osg_I2cImpl;

struct osg_I2c;
typedef struct osg_I2c osg_I2c;

// /////////////////////////////////////////////////////////////////////////////
// SPI
// /////////////////////////////////////////////////////////////////////////////

typedef void (* osg_SpiCallback)(void);

enum osg_SpiMode;
typedef enum osg_SpiMode osg_SpiMode;

enum osg_SpiId;
typedef enum osg_SpiId osg_SpiId;

enum osg_SpiPolarity;
typedef enum osg_SpiPolarity osg_SpiPolarity;

enum osg_SpiPhase;
typedef enum osg_SpiPhase osg_SpiPhase;

enum osg_SpiEndianess;
typedef enum osg_SpiEndianess osg_SpiEndianess;

enum osg_SpiDataSize;
typedef enum osg_SpiDataSize osg_SpiDataSize;

enum osg_SpiSlaveSelectMode;
typedef enum osg_SpiSlaveSelectMode osg_SpiSlaveSelectMode;

struct osg_Spi;
typedef struct osg_Spi osg_Spi;

// /////////////////////////////////////////////////////////////////////////////
// UART
// /////////////////////////////////////////////////////////////////////////////

enum osg_UartId;
typedef enum osg_UartId osg_UartId;

enum osg_UartFlags;
typedef enum osg_UartFlags osg_UartFlags;

struct osg_Uart;
typedef struct osg_Uart osg_Uart;

struct osg_UartImpl;
typedef struct osg_UartImpl osg_UartImpl;

typedef void (* osg_UartCallback)(void);

enum osg_UartBaudRate;
typedef enum osg_UartBaudRate osg_UartBaudRate;

enum osg_UartParity;
typedef enum osg_UartParity osg_UartParity;

enum osg_UartBits;
typedef enum osg_UartBits osg_UartBits;

enum osg_UartStopBits;
typedef enum osg_UartStopBits osg_UartStopBits;

enum osg_UartFlowControl;
typedef enum osg_UartFlowControl osg_UartFlowControl;

enum osg_UartType;
typedef enum osg_UartType osg_UartType;

#ifdef __cplusplus
}
#endif

#endif
