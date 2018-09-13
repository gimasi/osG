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
#ifndef OSG_DRIVERS_UARTDATATYPES_H
#define OSG_DRIVERS_UARTDATATYPES_H

#include "../utils/CircularFifo.h"
#include "GpioDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

enum osg_UartId;
typedef enum osg_UartId osg_UartId;
/// @brief Enum of possible UART devices
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartId
{
    OSG_UART0,
    OSG_UART1,
    OSG_UART2,
    OSG_UART3,
    OSG_UART4,
    OSG_UART5,
    OSG_UART6
};

enum osg_UartFlags;
typedef enum osg_UartFlags osg_UartFlags;
/// @brief Enum used to represent UART internal bit flags
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartFlags
{
    /// @brief Initial value
    OSG_UART_FLAG_DEFAULT         = 0x00,
    /// @brief UART is performing a non-blocking communication
    OSG_UART_FLAG_IS_NB_SEND      = 0x02,
    /// @brief UART is performing a non-blocking communication
    OSG_UART_FLAG_IS_NB_RECEIVE   = 0x04,
    /// @brief UART is sending data
    OSG_UART_FLAG_IS_SEND         = 0x08,
    /// @brief UART is receiving data
    OSG_UART_FLAG_IS_RECEIVE      = 0x10,
    /// @brief UART buffered rx process is enabled
    OSG_UART_FLAG_RX_BUFF_ENABLED = 0x20,
    /// @brief UART wait for aborting rx process
    OSG_UART_FLAG_RX_ABORTED      = 0x40,
    /// @brief UART configuration created
    OSG_UART_FLAG_HANDLE_CREATED  = 0x80
};

struct osg_Uart;
typedef struct osg_Uart osg_Uart;
/// @brief Uart class
/// @ingroup UART
/// @memberof osg_Uart
struct osg_Uart
{
    /// @brief This UART ID
    /// @private
    osg_UartId id;
    /// @brief Tx pin ID
    /// @private
    osg_Gpio txPin;
    /// @brief Rx pin ID
    /// @private
    osg_Gpio rxPin;
    /// @brief RTS pin ID
    /// @private
    osg_Gpio rtsPin;
    /// @brief  CTS pin ID
    /// @private
    osg_Gpio ctsPin;
};

/// @brief Convenience typedef for UART callback
/// @ingroup UART
/// @memberof osg_Uart
typedef void (* osg_UartCallback)(void);

enum osg_UartBaudRate;
typedef enum osg_UartBaudRate osg_UartBaudRate;
/// @brief UART possible baudrates
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartBaudRate
{
    OSG_UART_BAUDRATE_300,
    OSG_UART_BAUDRATE_600,
    OSG_UART_BAUDRATE_1200,
    OSG_UART_BAUDRATE_2400,
    OSG_UART_BAUDRATE_4800,
    OSG_UART_BAUDRATE_9600,
    OSG_UART_BAUDRATE_19200,
    OSG_UART_BAUDRATE_38400,
    OSG_UART_BAUDRATE_57600,
    OSG_UART_BAUDRATE_115200,
    OSG_UART_BAUDRATE_230400,
    OSG_UART_BAUDRATE_460800,
    OSG_UART_BAUDRATE_576000,
    OSG_UART_BAUDRATE_921600,
    OSG_UART_BAUDRATE_1000000,
    OSG_UART_BAUDRATE_2000000,
};

enum osg_UartParity;
typedef enum osg_UartParity osg_UartParity;
/// @brief UART parity
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartParity
{
    OSG_UART_PARITY_NONE,
    OSG_UART_PARITY_EVEN,
    OSG_UART_PARITY_ODD
};

enum osg_UartBits;
typedef enum osg_UartBits osg_UartBits;
/// @brief UART data bits, without considering possible parity.
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartBits
{
    OSG_UART_BITS_5,
    OSG_UART_BITS_6,
    OSG_UART_BITS_7,
    OSG_UART_BITS_8,
    OSG_UART_BITS_9
};

enum osg_UartStopBits;
typedef enum osg_UartStopBits osg_UartStopBits;
/// @brief UART stop bits
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartStopBits
{
    OSG_UART_STOPBITS_0_5,
    OSG_UART_STOPBITS_1,
    OSG_UART_STOPBITS_1_5,
    OSG_UART_STOPBITS_2
};

enum osg_UartFlowControl;
typedef enum osg_UartFlowControl osg_UartFlowControl;
/// @brief UART flow control
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartFlowControl
{
    OSG_UART_FLOWCONTROL_NONE,
    OSG_UART_FLOWCONTROL_RTS,
    OSG_UART_FLOWCONTROL_CTS,
    OSG_UART_FLOWCONTROL_RTS_CTS,
};

enum osg_UartType;
typedef enum osg_UartType osg_UartType;
/// @brief UART types
/// @ingroup UART
/// @memberof osg_Uart
enum osg_UartType
{
    OSG_UART,
    OSG_USART,
    OSG_LPUART
};

struct osg_UartImpl;
typedef struct osg_UartImpl osg_UartImpl;
/// @brief UART internal struct.
/// @ingroup UART
/// @memberof osg_Uart
struct osg_UartImpl
{
    /// @brief Low-level handler
    /// @private
    void * handler;
    /// @brief Send callback
    /// @private
    osg_UartCallback txCompleteNb;
    /// @brief receive callback
    /// @private
    osg_UartCallback rxCompleteNb;
    /// @brief Send callback
    /// @private
    osg_UartCallback txCompleteBuffered;
    /// @brief receive callback
    /// @private
    osg_UartCallback rxCompleteBuffered;
    /// @brief Internal flags
    /// @private
    osg_UartFlags flags;
    /// @brief Tx internal fifo
    /// @private
    osg_CircularFifo txFifo;
    /// @brief Rx internal fifo
    /// @private
    osg_CircularFifo rxFifo;
    /// @brief Rx receive byte
    /// @private
    Byte rxByte;
    /// @brief UART type
    /// @private
    osg_UartType type;
};

#ifdef __cplusplus
}
#endif

#endif
