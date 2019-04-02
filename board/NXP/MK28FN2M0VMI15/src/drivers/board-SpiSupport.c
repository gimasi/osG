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
#include "board-SpiSupport.h"

#define TOT_SPI FSL_FEATURE_SOC_DSPI_COUNT

static osg_SpiImpl _osg_board_Spi_devices[TOT_SPI];
static osg_board_SpiHandler _osg_board_Spi_handlers[TOT_SPI];
#if SPI_MUTEX_ENABLED
    static osg_Mutex osg_rtos_Spi_mutexes[TOT_SPI];
    static osg_rtos_Mutex osg_rtos_Spi_mutexesId[TOT_SPI];
#endif


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

uint8_t _osg_board_Spi_getSpis()
{
    return (uint8_t)TOT_SPI;
}

uint8_t _osg_board_Spi_getDeviceIndex(const osg_SpiId id)
{
    uint8_t index = 0;

#ifdef SPI0
    if (id == OSG_SPI0)
        return index;
    index++;
#endif
#ifdef SPI1
    if (id == OSG_SPI1)
        return index;
    index++;
#endif
#ifdef SPI2
    if (id == OSG_SPI2)
        return index;
    index++;
#endif
#ifdef SPI3
    if (id == OSG_SPI3)
        return index;
    index++;
#endif
#ifdef SPI4
    if (id == OSG_SPI4)
        return index;
    index++;
#endif

    return TOT_SPI;
}

osg_SpiImpl * _osg_board_Spi_getSpiByInstance(void * handler)
{
    for (int i = 0; i < TOT_SPI; ++i)
    {
        if (_osg_board_Spi_devices[i].handler == handler) return &_osg_board_Spi_devices[i];
    }

    return NULL;
}

osg_SpiImpl * _osg_board_Spi_getSpiById(const osg_SpiId id)
{
    uint8_t index = _osg_board_Spi_getDeviceIndex(id);
    return &_osg_board_Spi_devices[index];
}

osg_board_SpiInstance * _osg_board_Spi_getSpiInstance(const osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0
        case OSG_SPI0:
            return SPI0;
#endif
#ifdef SPI1
        case OSG_SPI1:
            return SPI1;
#endif
#ifdef SPI2
        case OSG_SPI2:
            return SPI2;
#endif
#ifdef SPI3
        case OSG_SPI3:
            return SPI3;
#endif
#ifdef SPI4
        case OSG_SPI4:
            return SPI4;
#endif
        default:
            osg_error("ERROR: Invalid SPI port");
    }

    return NULL;
}

osg_board_SpiHandler * _osg_board_Spi_getSpiHandler(const osg_SpiId id)
{
    uint8_t index = _osg_board_Spi_getDeviceIndex(id);
    return &_osg_board_Spi_handlers[index];
}

osg_board_SpiHandler * _osg_board_Spi_getSpiHandlerByInstance(void * handler)
{
    for (int i = 0; i < TOT_SPI; ++i)
    {
        if (_osg_board_Spi_devices[i].handler == handler) return &_osg_board_Spi_handlers[i];
    }

    return NULL;
}

#if SPI_MUTEX_ENABLED
void _osg_board_Spi_configureMutex(const osg_SpiId id)
{
    uint8_t index = _osg_board_Spi_getDeviceIndex(id);
    osg_rtos_Spi_mutexes[index].handler = &osg_rtos_Spi_mutexesId[index];
}


osg_Mutex * _osg_board_Spi_getMutex(const osg_SpiId id)
{
    uint8_t index = _osg_board_Spi_getDeviceIndex(id);
    return &osg_rtos_Spi_mutexes[index];
}
#endif


// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////

bool _osg_board_Spi_checkSpi(const osg_SpiId spi)
{
    switch (spi)
    {
#ifdef SPI0
        case OSG_SPI0:
#endif
#ifdef SPI1
        case OSG_SPI1:
#endif
#ifdef SPI2
        case OSG_SPI2:
#endif
#ifdef SPI3
        case OSG_SPI3:
#endif
#ifdef SPI4
        case OSG_SPI4:
#endif
            return true;
        default:
            return false;
    }
}

bool _osg_board_Spi_validateSpiPins(const osg_SpiId spiId, const osg_SpiMode mode, const osg_GpioId sclk, const osg_GpioId mosi, const osg_GpioId miso)
{
    osg_GpioId sout, sin;
    if (mode == OSG_SPI_MASTER_MODE)
    {
        sout = mosi;
        sin = miso;
    }
    else if (mode == OSG_SPI_SLAVE_MODE)
    {
        sout = miso;
        sin = mosi;
    }
    else
    {
        osg_error("ERROR: Ivalid SPI mode");
    }

    switch (spiId)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            if (sclk == OSG_GPIO_PE17
                || sclk == OSG_GPIO_PA15
                || sclk == OSG_GPIO_PC5
                || sclk == OSG_GPIO_PD1)
            {
                if (sout == OSG_GPIO_PE18
                    || sout == OSG_GPIO_PA16
                    || sout == OSG_GPIO_PC6
                    || sout == OSG_GPIO_PD2)
                {
                    if (sin == OSG_GPIO_PE19
                        || sin == OSG_GPIO_PA17
                        || sin == OSG_GPIO_PC7
                        || sin == OSG_GPIO_PD3)
                    {
                        return true;
                    }
                }
            }
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            if (sclk == OSG_GPIO_PE1
                || sclk == OSG_GPIO_PE2
                || sclk == OSG_GPIO_PB11
                || sclk == OSG_GPIO_PD5)
            {
                if (sout == OSG_GPIO_PE2
                    || sout == OSG_GPIO_PE3
                    || sout == OSG_GPIO_PB16
                    || sout == OSG_GPIO_PD6)
                {
                    if (sin == OSG_GPIO_PE1
                        || sin == OSG_GPIO_PE4
                        || sin == OSG_GPIO_PB17
                        || sin == OSG_GPIO_PD7)
                    {
                        return true;
                    }
                }
            }
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            if (sclk == OSG_GPIO_PB21
                || sclk == OSG_GPIO_PD12)
            {
                if (sout == OSG_GPIO_PB22
                    || sout == OSG_GPIO_PD13)
                {
                    if (sin == OSG_GPIO_PB23
                        || sin == OSG_GPIO_PD14)
                    {
                        return true;
                    }
                }
            }
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            if (sclk == OSG_GPIO_PE21)
            {
                if (sout == OSG_GPIO_PE22)
                {
                    if (sin == OSG_GPIO_PE23)
                    {
                        return true;
                    }
                }
            }
        }
#endif
        default:
            return false;
    }
}

bool _osg_board_Spi_validateSsPins(const osg_SpiId spiId, const osg_SpiMode mode, osg_GpioId * ssPinList, const Size ssPinListSize)
{
    // at least one slave select pin
    if (ssPinListSize < 1)
        return false;
    
    // if SPI slave than only one slave select pin
    if (mode == OSG_SPI_SLAVE_MODE
        && ssPinListSize > 1)
        return false;
    
    for (Size i = 0; i < ssPinListSize; ++i)
    {
        switch (spiId)
        {
#ifdef SPI0
            case OSG_SPI0:
            {
                // max 6 SS PINs for SPI0
                if (i < 6)
                {
                        // PCS0
                    if (ssPinList[i] == OSG_GPIO_PE16
                        || ssPinList[i] == OSG_GPIO_PA14
                        || ssPinList[i] == OSG_GPIO_PC4
                        || ssPinList[i] == OSG_GPIO_PD0
                        // PCS1
                        || ssPinList[i] == OSG_GPIO_PC3
                        || ssPinList[i] == OSG_GPIO_PD4
                        // PCS2
                        || ssPinList[i] == OSG_GPIO_PC2
                        || ssPinList[i] == OSG_GPIO_PD5
                        // PCS3
                        || ssPinList[i] == OSG_GPIO_PC1
                        || ssPinList[i] == OSG_GPIO_PD6
                        // PCS4
                        || ssPinList[i] == OSG_GPIO_PC0
                        // PCS5
                        || ssPinList[i] == OSG_GPIO_PB23)
                    {
                        continue;
                    }
                }
            }
#endif
#ifdef SPI1
            case OSG_SPI1:
            {
                // max 3 SS PINs for SPI1
                if (i < 3)
                {
                        // PCS0
                    if (ssPinList[i] == OSG_GPIO_PE5
                        || ssPinList[i] == OSG_GPIO_PB10
                        || ssPinList[i] == OSG_GPIO_PD4
                        // PCS1
                        || ssPinList[i] == OSG_GPIO_PE0
                        || ssPinList[i] == OSG_GPIO_PB9
                        // PCS2
                        || ssPinList[i] == OSG_GPIO_PE3)
                    {
                        continue;
                    }
                }
            }
#endif
#ifdef SPI2
            case OSG_SPI2:
            {
                // max 2 SS PINs for SPI2
                if (i < 2)
                {
                        // PCS0
                    if (ssPinList[i] == OSG_GPIO_PB20
                        || ssPinList[i] == OSG_GPIO_PD11
                        // PCS1
                        || ssPinList[i] == OSG_GPIO_PD15)
                    {
                        continue;
                    }
                }
            }
#endif
#ifdef SPI3
            case OSG_SPI3:
            {
                // max 1 SS PIN for SPI3
                if (i < 1)
                {
                        // PCS0
                    if (ssPinList[i] == OSG_GPIO_PE20)
                    {
                        continue;
                    }
                }
            }
#endif
            default:
                return false;
        }
    }

    return true;
}


// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////

dspi_clock_phase_t _osg_board_Spi_decodePhase(const osg_SpiPhase phase)
{
    switch (phase)
    {
        case OSG_SPI_PHASE_1ST:
            return kDSPI_ClockPhaseFirstEdge;
        case OSG_SPI_PHASE_2ND:
            return kDSPI_ClockPhaseSecondEdge;

        default:
            osg_error("ERROR: Ivalid SPI phase.");
    }

    return kDSPI_ClockPhaseFirstEdge;
}

dspi_clock_polarity_t _osg_board_Spi_decodePolarity(const osg_SpiPolarity polarity)
{
    switch (polarity)
    {
        case OSG_SPI_POLARITY_LOW:
            return kDSPI_ClockPolarityActiveLow;
        case OSG_SPI_POLARITY_HIGH:
            return kDSPI_ClockPolarityActiveHigh;

        default:
            osg_error("ERROR: Invalid SPI polarity.");
    }

    return kDSPI_ClockPolarityActiveLow;
}

dspi_shift_direction_t _osg_board_Spi_decodeEndianess(const osg_SpiEndianess endianess)
{
    switch (endianess)
    {
        case OSG_SPI_MSB_FIRST:
            return kDSPI_MsbFirst;
        case OSG_SPI_LSB_FIRST:
            return kDSPI_LsbFirst;

        default:
            osg_error("ERROR: Invalid SPI endianess.");
    }

    return kDSPI_MsbFirst;
}

uint32_t _osg_board_Spi_decodeDataSize(const osg_SpiDataSize size)
{
    switch (size)
    {
        case OSG_SPI_DATA_4_BITS:
            return 4;
        case OSG_SPI_DATA_8_BITS:
            return 8;
        case OSG_SPI_DATA_16_BITS:
            return 16;
        
        default:
            osg_error("ERROR: Invalid SPI data size.");
    }

    return 0;
}

//port_mux_t _osg_board_Spi_decodePinMuxAlt(const osg_GpioId pin, const osg_GpioAlternateFunction alt)
//{
//    switch (pin)
//    {
//        case OSG_GPIO_PA14:
//        case OSG_GPIO_PA15:
//        case OSG_GPIO_PA16:
//        case OSG_GPIO_PA17:
//        case OSG_GPIO_PB9:
//        case OSG_GPIO_PB10:
//        case OSG_GPIO_PB11:
//        case OSG_GPIO_PB16:
//        case OSG_GPIO_PB17:
//        case OSG_GPIO_PB20:
//        case OSG_GPIO_PB21:
//        case OSG_GPIO_PB22:
//        case OSG_GPIO_PC0:
//        case OSG_GPIO_PC1:
//        case OSG_GPIO_PC2:
//        case OSG_GPIO_PC3:
//        case OSG_GPIO_PC4:
//        case OSG_GPIO_PC5:
//        case OSG_GPIO_PC6:
//        case OSG_GPIO_PC7:
//        case OSG_GPIO_PD0:
//        case OSG_GPIO_PD1:
//        case OSG_GPIO_PD2:
//        case OSG_GPIO_PD3:
//        case OSG_GPIO_PD11:
//        case OSG_GPIO_PD12:
//        case OSG_GPIO_PD13:
//        case OSG_GPIO_PD14:
//        case OSG_GPIO_PD15:
//        case OSG_GPIO_PE0:
//        case OSG_GPIO_PE4:
//        case OSG_GPIO_PE5:
//        case OSG_GPIO_PE16:
//        case OSG_GPIO_PE17:
//        case OSG_GPIO_PE18:
//        case OSG_GPIO_PE19:
//            return kPORT_MuxAlt2;
//        case OSG_GPIO_PE20:
//        case OSG_GPIO_PE21:
//        case OSG_GPIO_PE22:
//        case OSG_GPIO_PE23:
//            return kPORT_MuxAlt4;
//        case OSG_GPIO_PD7:
//            return kPORT_MuxAlt7;
//        case OSG_GPIO_PB23:
//            if (alt == OSG_GPIO_ALTERNATE_SPI0_SS)
//                return kPORT_MuxAlt3;
//            else
//                return kPORT_MuxAlt2;
//        case OSG_GPIO_PD4:
//            if (alt == OSG_GPIO_ALTERNATE_SPI0_SS)
//                return kPORT_MuxAlt2;
//            else
//                return kPORT_MuxAlt7;
//        case OSG_GPIO_PD5:
//            if (alt == OSG_GPIO_ALTERNATE_SPI1_CLK)
//                return kPORT_MuxAlt7;
//            else
//                return kPORT_MuxAlt2;
//        case OSG_GPIO_PD6:
//            if (alt == OSG_GPIO_ALTERNATE_SPI0_SS)
//                return kPORT_MuxAlt2;
//            else
//                return kPORT_MuxAlt7;
//        case OSG_GPIO_PE1:
//            if (alt == OSG_GPIO_ALTERNATE_SPI1_CLK)
//                return kPORT_MuxAlt2;
//            else
//                return kPORT_MuxAlt7;
//        case OSG_GPIO_PE2:
//            if (alt == OSG_GPIO_ALTERNATE_SPI1_CLK)
//                return kPORT_MuxAlt7;
//            else
//                return kPORT_MuxAlt2;
//        case OSG_GPIO_PE3:
//            if (alt == OSG_GPIO_ALTERNATE_SPI1_SS)
//                return kPORT_MuxAlt2;
//            else
//                return kPORT_MuxAlt7;
//        
//        default:
//            osg_error("ERROR: PIN can't be used for SPI.");
//    }
//
//    return kPORT_PinDisabledOrAnalog;
//}

uint32_t _osg_board_Spi_getSpiClk(const osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0_CLK_SRC
        case OSG_SPI0:
            return CLOCK_GetFreq(SPI0_CLK_SRC);
#endif
#ifdef SPI1_CLK_SRC
        case OSG_SPI1:
            return CLOCK_GetFreq(SPI1_CLK_SRC);
#endif
#ifdef SPI2_CLK_SRC
        case OSG_SPI2:
            return CLOCK_GetFreq(SPI2_CLK_SRC);
#endif
#ifdef SPI3_CLK_SRC
        case OSG_SPI3:
            return CLOCK_GetFreq(SPI3_CLK_SRC);
#endif
#ifdef SPI4_CLK_SRC
        case OSG_SPI4:
            return CLOCK_GetFreq(SPI4_CLK_SRC);
#endif
        default:
            osg_error("ERROR: Invalid SPI port");
    }

    return (clock_name_t)0;
}


// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////

//osg_GpioAlternateFunction _osg_board_Spi_encodeAlternateFunction(const osg_SpiId spiId, const bool isMosiPin, const bool isMisoPin)
//{
//    switch (spiId)
//    {
//#ifdef SPI0
//        case OSG_SPI0:
//        {
//            if (isMosiPin == true)
//                return OSG_GPIO_ALTERNATE_SPI0_MOSI;
//            else if (isMisoPin == true)
//                return OSG_GPIO_ALTERNATE_SPI0_MISO;
//            else
//                return OSG_GPIO_ALTERNATE_SPI0_CLK;
//        }
//#endif
//#ifdef SPI1
//        case OSG_SPI1:
//        {
//            if (isMosiPin == true)
//                return OSG_GPIO_ALTERNATE_SPI1_MOSI;
//            else if (isMisoPin == true)
//                return OSG_GPIO_ALTERNATE_SPI1_MISO;
//            else
//                return OSG_GPIO_ALTERNATE_SPI1_CLK;
//        }
//#endif
//#ifdef SPI2
//        case OSG_SPI2:
//        {
//            if (isMosiPin == true)
//                return OSG_GPIO_ALTERNATE_SPI2_MOSI;
//            else if (isMisoPin == true)
//                return OSG_GPIO_ALTERNATE_SPI2_MISO;
//            else
//                return OSG_GPIO_ALTERNATE_SPI2_CLK;
//        }
//#endif
//#ifdef SPI3
//        case OSG_SPI3:
//        {
//            if (isMosiPin == true)
//                return OSG_GPIO_ALTERNATE_SPI3_MOSI;
//            else if (isMisoPin == true)
//                return OSG_GPIO_ALTERNATE_SPI3_MISO;
//            else
//                return OSG_GPIO_ALTERNATE_SPI3_CLK;
//        }
//#endif
//#ifdef SPI4
//        case OSG_SPI4:
//        {
//            if (isMosiPin == true)
//                return OSG_GPIO_ALTERNATE_SPI4_MOSI;
//            else if (isMisoPin == true)
//                return OSG_GPIO_ALTERNATE_SPI4_MISO;
//            else
//                return OSG_GPIO_ALTERNATE_SPI4_CLK;
//        }
//#endif
//        default:
//            osg_error("ERROR: Impossible to set correct SPI Alternate Function to Pin.");
//    }
//
//    // suppress warning
//    return OSG_GPIO_ALTERNATE_DEFAULT;
//}


// ///////////////////////////////////////////////////////
// ENABLE AND DISABLE
// ///////////////////////////////////////////////////////

void _osg_board_Spi_initSsPins(const osg_SpiId spiId, const osg_SpiSlaveSelectMode ssMode, const osg_GpioId * ssPins, const Size ssPinsSize, const uint16_t alterante)
{
    osg_assert(ssMode == OSG_SPI_SLAVE_SELECT_AUTO, "ASSERT FAILED: Invalid Slave Select mode for NXP board. Only AUTO is available.");
        
    for (Size i = 0; i < ssPinsSize; ++i)
    {
        osg_GpioId ss = ssPins[i];
        osg_GpioPort port = osg_Gpio_getPort(ss);
        _osg_board_Gpio_enablePort(port);
        osg_board_Port * boardPort = _osg_board_Gpio_getPortHandler(port);
        uint32_t pin = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(ss));
        
//        osg_GpioAlternateFunction alt;
//        switch (spiId)
//        {
//#ifdef SPI0
//            case OSG_SPI0:
//                alt = OSG_GPIO_ALTERNATE_SPI0_SS;
//                break;
//#endif
//#ifdef SPI1
//            case OSG_SPI1:
//                alt = OSG_GPIO_ALTERNATE_SPI1_SS;
//                break;
//#endif
//#ifdef SPI2
//            case OSG_SPI2:
//                alt = OSG_GPIO_ALTERNATE_SPI2_SS;
//                break;
//#endif
//#ifdef SPI3
//            case OSG_SPI3:
//                alt = OSG_GPIO_ALTERNATE_SPI3_SS;
//                break;
//#endif
//            default:
//                osg_error("ERROR: Invalid SPI port.");
//        }

        port_mux_t mux = (port_mux_t)alterante;
        PORT_SetPinMux(boardPort, pin, mux);
    }
}

void _osg_board_Spi_deinitSSPins(const osg_SpiId id, osg_GpioId * ssList, const Size ssListSize)
{
    for (Size i = 0; i < ssListSize; ++i)
    {
        osg_GpioId ss = ssList[i];
        osg_GpioPort port = osg_Gpio_getPort(ss);
        osg_board_Port * boardPort = _osg_board_Gpio_getPortHandler(port);
        uint32_t pin = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(ss));
        PORT_SetPinMux(boardPort, pin, kPORT_PinDisabledOrAnalog);
    }
}


// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////

void _osg_board_Spi_doBufferedSend(osg_SpiImpl * spi)
{
    if (osg_CircularFifo_isEmpty(&spi->txFifo)) return;
    const osg_SpiFlags restore = spi->flags;
    spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
    spi->flags |= OSG_SPI_FLAG_IS_SEND;
    void * buffer = NULL;
    Size size = 0;
    osg_CircularFifo_getFilledBuffer(&spi->txFifo, &buffer, &size);

    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandlerByInstance(spi->handler);
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    status_t ok;

    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        dspi_transfer_t xfer = {0};
        xfer.txData = buffer;
        xfer.rxData = NULL;
        xfer.dataSize = size;
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterSendBuffCallback,
                                        NULL);
        ok = DSPI_MasterTransferNonBlocking(instance,
                                            &handle->masterHandle,
                                            &xfer);
    }
    else
    {
        dspi_transfer_t xfer = {0};
        xfer.txData = buffer;
        xfer.rxData = NULL;
        xfer.dataSize = size;
        xfer.configFlags = kDSPI_SlaveCtar0;

        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveSendBuffCallback,
                                       NULL);
        ok = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }


    if (ok != kStatus_Success) spi->flags = restore;
}

void _osg_board_Spi_doBufferedReceive(osg_SpiImpl * spi)
{
    if (osg_CircularFifo_isNull(&spi->rxFifo)
            || !(spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED))
        return;
    
    const osg_SpiFlags restore = spi->flags;
    spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
    spi->flags |= OSG_SPI_FLAG_IS_RECEIVE;

    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandlerByInstance(spi->handler);
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    status_t ok;

    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        dspi_transfer_t xfer = {0};
        xfer.txData = NULL;
        xfer.rxData = &spi->rxByte;
        xfer.dataSize = 1;
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterRecvBuffCallback,
                                        NULL);
        ok = DSPI_MasterTransferNonBlocking(instance,
                                            &handle->masterHandle,
                                            &xfer);
    }
    else
    {
        dspi_transfer_t xfer = {0};
        xfer.txData = NULL;
        xfer.rxData = &spi->rxByte;
        xfer.dataSize = 1;
        xfer.configFlags = kDSPI_SlaveCtar0;

        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveRecvBuffCallback,
                                       NULL);
        ok = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }

    if (ok != kStatus_Success) spi->flags = restore;
}

void _osg_board_Spi_masterBlockingCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    int * pState = (int *)userData;
    *pState = status;
}

void _osg_board_Spi_masterSendNBCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        spi->flags &= ~OSG_SPI_FLAG_IS_SEND;
        if (spi->txCompleteNb != NULL) spi->txCompleteNb();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
    }
}

void _osg_board_Spi_masterRecvNBCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        spi->flags &= ~OSG_SPI_FLAG_IS_RECEIVE;
        if (spi->rxCompleteNb != NULL) spi->rxCompleteNb();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
    }
}

void _osg_board_Spi_masterSendBuffCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        osg_CircularFifo_popBytes(&spi->txFifo, handle->totalByteCount - handle->remainingSendByteCount);
        if (spi->txCompleteBuffered != NULL) spi->txCompleteBuffered();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
        _osg_board_Spi_doBufferedSend(spi);
    }
}

void _osg_board_Spi_masterRecvBuffCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        osg_CircularFifo_pushBuffer(&spi->rxFifo, &spi->rxByte, handle->totalByteCount - handle->remainingReceiveByteCount);
        if (spi->rxCompleteBuffered != NULL) spi->rxCompleteBuffered();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
        _osg_board_Spi_doBufferedReceive(spi);
    }
}

void _osg_board_Spi_slaveBlockingCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    int * pState = (int *)userData;
    *pState = status;
}

void _osg_board_Spi_slaveSendNBCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        spi->flags &= ~OSG_SPI_FLAG_IS_SEND;
        if (spi->txCompleteNb != NULL) spi->txCompleteNb();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
    }
}

void _osg_board_Spi_slaveRecvNBCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        spi->flags &= ~OSG_SPI_FLAG_IS_RECEIVE;
        if (spi->rxCompleteNb != NULL) spi->rxCompleteNb();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
    }
}

void _osg_board_Spi_slaveSendBuffCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        osg_CircularFifo_popBytes(&spi->txFifo, handle->totalByteCount - handle->remainingSendByteCount);
        if (spi->txCompleteBuffered != NULL) spi->txCompleteBuffered();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
        _osg_board_Spi_doBufferedSend(spi);
    }
}

void _osg_board_Spi_slaveRecvBuffCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        osg_SpiImpl * spi = _osg_board_Spi_getSpiByInstance(base);
        osg_CircularFifo_pushBuffer(&spi->rxFifo, &spi->rxByte, handle->totalByteCount - handle->remainingReceiveByteCount);
        if (spi->rxCompleteBuffered != NULL) spi->rxCompleteBuffered();
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
        _osg_board_Spi_doBufferedReceive(spi);
    }
}

void _osg_board_Spi_timeoutCallback(void * argument)
{
    int * pState = (int *)argument;
    *pState = kStatus_DSPI_Error;
}
