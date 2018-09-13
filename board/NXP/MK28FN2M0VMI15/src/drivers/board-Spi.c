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
#include "../../../../include/board/drivers/board-Spi.h"
#include <osg.h>
#include <fsl_dspi.h>
#include <fsl_port.h>

port_mux_t _osg_board_spi_getPinPortMuxForSpi(const osg_GpioId pin)
{
    return kPORT_MuxAlt2;
}

osg_GpioAlternateFunction _osg_board_spi_getGpioAlternateFunction(const osg_SpiId spiId)
{
    switch (spiId)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            return OSG_GPIO_ALTERNATE_SPI0;
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            return OSG_GPIO_ALTERNATE_SPI1;
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            return OSG_GPIO_ALTERNATE_SPI2;
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            return OSG_GPIO_ALTERNATE_SPI3;
        }
#endif
#ifdef SPI4
        case OSG_SPI4:
        {
            return OSG_GPIO_ALTERNATE_SPI4;
        }
#endif
        default:
            osg_error("ERROR: Impossible to set correct SPI Alternate Function to Pin.");
    }

    // suppress warning
    return OSG_GPIO_ALTERNATE_DEFAULT;
}

void osg_board_spi_ctor(osg_Spi * self, uint32_t clockSpeed, osg_SpiPolarity polarity, osg_SpiPhase phase, osg_SpiEndianess endianess, osg_SpiDataSize dataSize, osg_SpiSlaveSelectMode ssMode)
{
}

void osg_board_spi_dtor(osg_Spi * self)
{
}

Bool osg_board_spi_sendBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout)
{
    return FALSE;
}

Bool osg_board_spi_receiveBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout)
{
    return FALSE;
}

Bool osg_board_spi_sendNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    return FALSE;
}

Bool osg_board_spi_receiveNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    return FALSE;
}

Size osg_board_spi_sendBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    return 0;
}

Size osg_board_spi_receivedBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize)
{
    return 0;
}

Size osg_board_spi_getSizeOfReceivedData(osg_Spi * self)
{
    return 0;
}
