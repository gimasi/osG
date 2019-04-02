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

#ifdef SPI0
#define COUNT_SPI0 1
#else
#define COUNT_SPI0 0
#endif
#ifdef SPI1
#define COUNT_SPI1 1
#else
#define COUNT_SPI1 0
#endif
#ifdef SPI2
#define COUNT_SPI2 1
#else
#define COUNT_SPI2 0
#endif
#ifdef SPI3
#define COUNT_SPI3 1
#else
#define COUNT_SPI3 0
#endif
#ifdef SPI4
#define COUNT_SPI4 1
#else
#define COUNT_SPI4 0
#endif

#define TOT_SPI (COUNT_SPI0 + COUNT_SPI1 + COUNT_SPI2 + COUNT_SPI3 + COUNT_SPI4)

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

uint8_t _osg_board_Spi_getDeviceIndex(const osg_SpiId spi)
{
    uint8_t index = 0;

#ifdef SPI0
    if (spi == OSG_SPI0)
        return index;
    index++;
#endif
#ifdef SPI1
    if (spi == OSG_SPI1)
        return index;
    index++;
#endif
#ifdef SPI2
    if (spi == OSG_SPI2)
        return index;
    index++;
#endif
#ifdef SPI3
    if (spi == OSG_SPI3)
        return index;
    index++;
#endif
#ifdef SPI4
    if (spi == OSG_SPI4)
        return index;
    index++;
#endif

    return TOT_SPI;
}

osg_SpiImpl * _osg_board_Spi_getSpiByHandler(void * handler)
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

osg_board_SpiInstance * _osg_board_Spi_getSpiInstance(const osg_SpiId spi)
{
    switch (spi)
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

bool _osg_board_Spi_validatePins(const osg_SpiId spiId, const osg_GpioId sclk, const osg_GpioId mosi, const osg_GpioId miso)
{
    switch (spiId)
    {
#ifdef SPI1
        case OSG_SPI1:
        {
            if (sclk == OSG_GPIO_PA5
                || sclk == OSG_GPIO_PB3
                || sclk == OSG_GPIO_PE13
                || sclk == OSG_GPIO_PG2)
            {
                if (mosi == OSG_GPIO_PA7
                    || mosi == OSG_GPIO_PB5
                    || mosi == OSG_GPIO_PE15
                    || mosi == OSG_GPIO_PG4)
                {
                    if (miso == OSG_GPIO_PA6
                        || miso == OSG_GPIO_PB4
                        || miso == OSG_GPIO_PE14
                        || miso == OSG_GPIO_PG3)
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
            if (sclk == OSG_GPIO_PB10
                || sclk == OSG_GPIO_PB13
                || sclk == OSG_GPIO_PD1)
            {
                if (mosi == OSG_GPIO_PB15
                    || mosi == OSG_GPIO_PC3
                    || mosi == OSG_GPIO_PD4)
                {
                    if (miso == OSG_GPIO_PB14
                        || miso == OSG_GPIO_PC2
                        || miso == OSG_GPIO_PD3)
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
            if (sclk == OSG_GPIO_PB3
                || sclk == OSG_GPIO_PC10
                || sclk == OSG_GPIO_PG9)
            {
                if (mosi == OSG_GPIO_PB5
                    || mosi == OSG_GPIO_PC12
                    || mosi == OSG_GPIO_PG11)
                {
                    if (miso == OSG_GPIO_PB4
                        || miso == OSG_GPIO_PC11
                        || miso == OSG_GPIO_PG10)
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


// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO STM
// ///////////////////////////////////////////////////////

uint32_t _osg_board_Spi_decodeBaudRate(const uint32_t speed)
{
    // @todo: convert speed in prescaler scale.
    return SPI_BAUDRATEPRESCALER_8;
}

uint32_t _osg_board_Spi_decodeMode(const osg_SpiMode mode)
{
    switch (mode)
    {
        case OSG_SPI_MASTER_MODE:
            return SPI_MODE_MASTER;
        case OSG_SPI_SLAVE_MODE:
            return SPI_MODE_SLAVE;
        default:
            osg_error("ERROR: Impossible to retrieve SPI mode.");
    }

    // disable warning
    return 0;
}

uint32_t _osg_board_Spi_decodePhase(const osg_SpiPhase phase)
{
    switch (phase)
    {
        case OSG_SPI_PHASE_1ST:
            return SPI_PHASE_1EDGE;
        case OSG_SPI_PHASE_2ND:
            return SPI_PHASE_2EDGE;
        default:
            osg_error("ERROR: Impossible to retrieve SPI phase.");
    }

    // disable warning
    return 0;
}

uint32_t _osg_board_Spi_decodePolarity(const osg_SpiPolarity polarity)
{
    switch (polarity)
    {
        case OSG_SPI_POLARITY_LOW:
            return SPI_POLARITY_LOW;
        case OSG_SPI_POLARITY_HIGH:
            return SPI_POLARITY_HIGH;
        default:
            osg_error("ERROR: Impossible to retrieve SPI polarity.");
    }

    // disable warning
    return 0;
}

uint32_t _osg_board_Spi_decodeEndianess(const osg_SpiEndianess endianess)
{
    switch (endianess)
    {
        case OSG_SPI_MSB_FIRST:
            return SPI_FIRSTBIT_MSB;
        case OSG_SPI_LSB_FIRST:
            return SPI_FIRSTBIT_LSB;
        default:
            osg_error("ERROR: Impossible to retrieve SPI endianess.");
    }

    // disable warning
    return 0;
}

uint32_t _osg_board_Spi_decodeDataSize(const osg_SpiDataSize size)
{
    switch (size)
    {
        case OSG_SPI_DATA_8_BITS:
            return SPI_DATASIZE_8BIT;
        case OSG_SPI_DATA_16_BITS:
            return SPI_DATASIZE_16BIT;
        default:
            osg_error("ERROR: Impossible to retrieve SPI data size.");
    }

    // disable warning
    return 0;
}

uint32_t _osg_board_Spi_decodeSSMode(const osg_SpiSlaveSelectMode mode)
{
    switch (mode)
    {
        case OSG_SPI_SLAVE_SELECT_INPUT:
            return SPI_NSS_HARD_INPUT;
        case OSG_SPI_SLAVE_SELECT_OUTPUT:
            return SPI_NSS_HARD_OUTPUT;
        default:
            return SPI_NSS_SOFT;
    }
}

//uint32_t _osg_board_Spi_decodeAlternateFunction(const osg_SpiId spiId)
//{
//    switch (spiId)
//    {
//#ifdef SPI0
//        case OSG_SPI0:
//        {
//            return GPIO_AF5_SPI0;
//        }
//#endif
//#ifdef SPI1
//        case OSG_SPI1:
//        {
//            return GPIO_AF5_SPI1;
//        }
//#endif
//#ifdef SPI2
//        case OSG_SPI2:
//        {
//            return GPIO_AF5_SPI2;
//        }
//#endif
//#ifdef SPI3
//        case OSG_SPI3:
//        {
//            return GPIO_AF6_SPI3;
//        }
//#endif
//#ifdef SPI4
//        case OSG_SPI4:
//        {
//            return GPIO_AF6_SPI4;
//        }
//#endif
//        default:
//            osg_error("ERROR: Impossible to set correct SPI Alternate Function to Pin.");
//    }
//
//    return 0;
//}


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

void _osg_board_Spi_enableSpi(osg_board_SpiHandler * handler)
{
    __HAL_SPI_ENABLE(handler);
}

void _osg_board_Spi_enableSpiClk(const osg_SpiId spi)
{
    switch(spi)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            __HAL_RCC_SPI0_CLK_ENABLE();
            break;
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            __HAL_RCC_SPI1_CLK_ENABLE();
            break;
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            __HAL_RCC_SPI2_CLK_ENABLE();
            break;
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            __HAL_RCC_SPI3_CLK_ENABLE();
            break;
        }
#endif
#ifdef SPI4
        case OSG_SPI4:
        {
            __HAL_RCC_SPI4_CLK_ENABLE();
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid SPI Port in SPI initialization.");
    }
}

void _osg_board_Spi_enableSpiInterrupt(const osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            HAL_NVIC_SetPriority(SPI0_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(SPI0_IRQn);
            break;
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(SPI1_IRQn);
            break;
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            HAL_NVIC_SetPriority(SPI2_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(SPI2_IRQn);
            break;
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            HAL_NVIC_SetPriority(SPI3_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(SPI3_IRQn);
            break;
        }
#endif
#ifdef SPI4
        case OSG_SPI4:
        {
            HAL_NVIC_SetPriority(SPI4_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(SPI4_IRQn);
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid SPI port; impossible to enable the correct interrupt.");
    }
}

void _osg_board_Spi_initSSPins(const osg_SpiId spiId, const osg_GpioId * ssPins, const Size ssPinsSize, const osg_SpiSlaveSelectMode ssMode, const uint16_t alternate)
{
    for (Size i = 0; i < ssPinsSize; ++i)
    {
        osg_GpioId ss = ssPins[i];
        osg_GpioPort port = osg_Gpio_getPort(ss);
        _osg_board_Gpio_enablePort(port);
        GPIO_InitTypeDef  GPIO_InitStruct;
        GPIO_InitStruct.Pin       = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(ss));
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = alternate;
        GPIO_InitStruct.Pin = _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(ss));

        if (ssMode == OSG_SPI_SLAVE_SELECT_INPUT)
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(_osg_board_Gpio_getPortHandler(port), &GPIO_InitStruct);
        _osg_board_Spi_deactivateSlave(ss);
    }
}

void _osg_board_Spi_deinitSSPins(const osg_SpiId id, osg_GpioId * ssList, const Size ssListSize)
{
    for (Size i = 0; i < ssListSize; ++i)
    {
        HAL_GPIO_DeInit(_osg_board_Gpio_getPortHandler(osg_Gpio_getPort(ssList[i])),
                        _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(ssList[i])));
    }
}

void _osg_board_Spi_resetSpi(const osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            __HAL_RCC_SPI0_FORCE_RESET();
            __HAL_RCC_SPI0_RELEASE_RESET();
            break;
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            __HAL_RCC_SPI1_FORCE_RESET();
            __HAL_RCC_SPI1_RELEASE_RESET();
            break;
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            __HAL_RCC_SPI2_FORCE_RESET();
            __HAL_RCC_SPI2_RELEASE_RESET();
            break;
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            __HAL_RCC_SPI3_FORCE_RESET();
            __HAL_RCC_SPI3_RELEASE_RESET();
            break;
        }
#endif
#ifdef SPI4
        case OSG_SPI4:
        {
            __HAL_RCC_SPI4_FORCE_RESET();
            __HAL_RCC_SPI4_RELEASE_RESET();
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid SPI Port in SPI reset.");
    }
}

void _osg_board_Spi_disableSpiClk(const osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0
        case OSG_SPI0: __HAL_RCC_SPI0_CLK_DISABLE(); break;
#endif
#ifdef SPI1
        case OSG_SPI1: __HAL_RCC_SPI1_CLK_DISABLE(); break;
#endif
#ifdef SPI2
        case OSG_SPI2: __HAL_RCC_SPI2_CLK_DISABLE(); break;
#endif
#ifdef SPI3
        case OSG_SPI3: __HAL_RCC_SPI3_CLK_DISABLE(); break;
#endif
#ifdef SPI4
        case OSG_SPI4: __HAL_RCC_SPI4_CLK_DISABLE(); break;
#endif
        default:
            osg_error("ERROR: Invalid SPI Port in SPI disabling clock.");
    }
}

void _osg_board_Spi_disableSpiInterrupt(const osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0
        case OSG_SPI0: HAL_NVIC_DisableIRQ(SPI0_IRQn); break;
#endif
#ifdef SPI1
        case OSG_SPI1: HAL_NVIC_DisableIRQ(SPI1_IRQn); break;
#endif
#ifdef SPI2
        case OSG_SPI2: HAL_NVIC_DisableIRQ(SPI2_IRQn); break;
#endif
#ifdef SPI3
        case OSG_SPI3: HAL_NVIC_DisableIRQ(SPI3_IRQn); break;
#endif
#ifdef SPI4
        case OSG_SPI4: HAL_NVIC_DisableIRQ(SPI4_IRQn); break;
#endif
        default:
            osg_error("ERROR: Invalid SPI Port in SPI disabling interrupt.");
    }
}


// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////

void _osg_board_Spi_activateSlave(const osg_GpioId slaveSelectPin)
{
    HAL_GPIO_WritePin(_osg_board_Gpio_getPortHandler(osg_Gpio_getPort(slaveSelectPin)),
                      _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(slaveSelectPin)),
                      GPIO_PIN_RESET);
}

void _osg_board_Spi_deactivateSlave(const osg_GpioId slaveSelectPin)
{
    HAL_GPIO_WritePin(_osg_board_Gpio_getPortHandler(osg_Gpio_getPort(slaveSelectPin)),
                      _osg_board_Gpio_decodeGpioPin(osg_Gpio_getPin(slaveSelectPin)),
                      GPIO_PIN_SET);
}

void _osg_board_Spi_doBufferedSend(osg_SpiImpl * spi)
{
    if (osg_CircularFifo_isEmpty(&spi->txFifo))
    {
        if (spi->mode == OSG_SPI_MASTER_MODE)
            _osg_board_Spi_deactivateSlave(spi->lastSsPin);
        return;
    }
    
    const osg_SpiFlags restore = spi->flags;
    spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
    spi->flags |= OSG_SPI_FLAG_IS_SEND;
    void * buffer = NULL;
    Size size = 0;
    osg_CircularFifo_getFilledBuffer(&spi->txFifo, &buffer, &size);

    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;
    const HAL_StatusTypeDef ok = HAL_SPI_Transmit_IT(handler, (uint8_t *) buffer, (uint16_t) size);

    if (ok != HAL_OK) spi->flags = restore;
}

void _osg_board_Spi_doBufferedReceive(osg_SpiImpl * spi)
{
    if (osg_CircularFifo_isNull(&spi->rxFifo)
            || !(spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED))
        return;
    const osg_SpiFlags restore = spi->flags;
    spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
    spi->flags |= OSG_SPI_FLAG_IS_RECEIVE;

    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;
    const HAL_StatusTypeDef ok = HAL_SPI_Receive_IT(handler, &spi->rxByte, 1);

    if (ok != HAL_OK) spi->flags = restore;
}


// ///////////////////////////////////////////////////////
// STM INTERNAL CALLBACKS
// ///////////////////////////////////////////////////////

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    osg_error("ERROR: SPI communication error.");
}

#ifdef SPI0
void SPI0_IRQHandler(void)
{
    HAL_SPI_IRQHandler(_osg_board_Spi_getSpiHandler(OSG_SPI0));
}
#endif
#ifdef SPI1
void SPI1_IRQHandler(void)
{
    HAL_SPI_IRQHandler(_osg_board_Spi_getSpiHandler(OSG_SPI1));
}
#endif
#ifdef SPI2
void SPI2_IRQHandler(void)
{
    HAL_SPI_IRQHandler(_osg_board_Spi_getSpiHandler(OSG_SPI2));
}
#endif
#ifdef SPI3
void SPI3_IRQHandler(void)
{
    HAL_SPI_IRQHandler(_osg_board_Spi_getSpiHandler(OSG_SPI3));
}
#endif
#ifdef SPI4
void SPI4_IRQHandler(void)
{
    HAL_SPI_IRQHandler(_osg_board_Spi_getSpiHandler(OSG_SPI4));
}
#endif
