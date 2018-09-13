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
#include "board/drivers/board-Spi.h"
#include <osg.h>
#include <stm32l4xx_hal.h>

typedef SPI_HandleTypeDef osg_board_SpiInstance;
typedef SPI_TypeDef osg_board_SpiId;
void _osg_board_gpio_enablePort(osg_GpioPort port);
GPIO_TypeDef * _osg_board_gpio_getPortHandler(const osg_GpioPort port);
uint16_t _osg_board_gpio_getBoardGpioPin(osg_GpioPin osgPin);

struct osg_board_Spi;
typedef struct osg_board_Spi osg_board_Spi;
struct osg_board_Spi
{
    osg_board_SpiInstance handler;
    osg_SpiCallback callback;
    osg_CircularFifo * txBuffer;
    osg_CircularFifo * rxBuffer;
    Bool isBufferTransfer;
};

struct osg_board_SpiInfo;
typedef struct osg_board_SpiInfo osg_board_SpiInfo;
struct osg_board_SpiInfo
{
    uint16_t destination;
    Size size;
};

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

static uint8_t _osg_board_spi_getSpis()
{
    return (uint8_t)TOT_SPI;
}

static osg_board_Spi _osg_board_spi_peripherals[TOT_SPI];

Bool _osg_board_spi_checkSpi(osg_SpiId spi)
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
            return TRUE;
        default:
            return FALSE;
    }
}

static osg_board_SpiId * _osg_board_spi_getSpiInstance(osg_SpiId spi)
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

static uint8_t _osg_board_spi_getPeripheralIndexFromId(osg_SpiId spi)
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

static uint8_t _osg_board_spi_getPeripheralIndexFromInstance(osg_board_SpiInstance * spi)
{
    uint8_t index = 0;

#ifdef SPI0
    if (spi == &_osg_board_spi_peripherals[index].handler)
        return index;
    index++;
#endif
#ifdef SPI1
    if (spi == &_osg_board_spi_peripherals[index].handler)
        return index;
    index++;
#endif
#ifdef SPI2
    if (spi == &_osg_board_spi_peripherals[index].handler)
        return index;
    index++;
#endif
#ifdef SPI3
    if (spi == &_osg_board_spi_peripherals[index].handler)
        return index;
    index++;
#endif
#ifdef SPI4
    if (spi == &_osg_board_spi_peripherals[index].handler)
        return index;
    index++;
#endif

    return TOT_SPI;
}

static Bool _osg_board_spi_validatePins(const osg_SpiId spiId, const osg_GpioId sclk, const osg_GpioId mosi, const osg_GpioId miso)
{
    switch (spiId)
    {
#ifdef SPI1
        case OSG_SPI1:
        {
            if (sclk == OSG_GPIO_PA5
                || sclk == OSG_GPIO_PB3)
            {
                if (mosi == OSG_GPIO_PA7
                    || mosi == OSG_GPIO_PB5)
                {
                    if (miso == OSG_GPIO_PA6
                        || miso == OSG_GPIO_PB4)
                    {
                        return TRUE;
                    }
                }
            }
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            if (sclk == OSG_GPIO_PB10
                || sclk == OSG_GPIO_PB13)
            {
                if (mosi == OSG_GPIO_PC3
                    || mosi == OSG_GPIO_PB15)
                {
                    if (miso == OSG_GPIO_PC2
                        || miso == OSG_GPIO_PB14)
                    {
                        return TRUE;
                    }
                }
            }
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            if (sclk == OSG_GPIO_PC10
                || sclk == OSG_GPIO_PB3)
            {
                if (mosi == OSG_GPIO_PC12
                    || mosi == OSG_GPIO_PB5)
                {
                    if (miso == OSG_GPIO_PC11
                        || miso == OSG_GPIO_PB4)
                    {
                        return TRUE;
                    }
                }
            }
        }
#endif
        default:
            return FALSE;
    }
}

static void _osg_board_spi_enableSpiClk(osg_SpiId spi)
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

static uint32_t _osg_board_spi_getAlternateFunction(const osg_SpiId spiId)
{
    switch (spiId)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            return GPIO_AF5_SPI0;
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            return GPIO_AF5_SPI1;
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            return GPIO_AF5_SPI2;
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            return GPIO_AF6_SPI3;
        }
#endif
#ifdef SPI4
        case OSG_SPI4:
        {
            return GPIO_AF6_SPI4;
        }
#endif
        default:
            osg_error("ERROR: Impossible to set correct SPI Alternate Function to Pin.");
    }

    return 0;
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

static void _osg_board_spi_activateSlave(osg_GpioId slaveSelectPin)
{
    HAL_GPIO_WritePin(_osg_board_gpio_getPortHandler(osg_gpio_getPort(slaveSelectPin)),
                          _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(slaveSelectPin)),
                          GPIO_PIN_RESET);
}

static void _osg_board_spi_deactivateSlave(osg_GpioId slaveSelectPin)
{
    HAL_GPIO_WritePin(_osg_board_gpio_getPortHandler(osg_gpio_getPort(slaveSelectPin)),
                          _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(slaveSelectPin)),
                          GPIO_PIN_SET);
}

static void _osg_board_spi_initSSPins(const osg_SpiId spiId, const osg_GpioId * ssPins, const Size ssPinsSize, const osg_SpiSlaveSelectMode ssMode)
{
    for (Size i = 0; i < ssPinsSize; ++i)
    {
        osg_GpioId ss = ssPins[i];
        osg_GpioPort port = osg_gpio_getPort(ss);
        _osg_board_gpio_enablePort(port);
        GPIO_InitTypeDef  GPIO_InitStruct;
        GPIO_InitStruct.Pin       = _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(ss));
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = _osg_board_spi_getAlternateFunction(spiId);
        GPIO_InitStruct.Pin = _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(ss));

        if (ssMode == OSG_SPI_SLAVE_SELECT_INPUT)
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(_osg_board_gpio_getPortHandler(port), &GPIO_InitStruct);
        _osg_board_spi_deactivateSlave(ss);
    }
}

static uint32_t _osg_board_spi_decodeBaudRate(uint32_t speed)
{
    // @todo: convert speed in prescaler scale.
    return 0;
}

static uint32_t _osg_board_spi_decodeMode(osg_SpiMode mode)
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

static uint32_t _osg_board_spi_decodePhase(osg_SpiPhase phase)
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

static uint32_t _osg_board_spi_decodePolarity(osg_SpiPolarity polarity)
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

static uint32_t _osg_board_spi_decodeEndianess(osg_SpiEndianess endianess)
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

static uint32_t _osg_board_spi_decodeDataSize(osg_SpiDataSize size)
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

static uint32_t _osg_board_spi_decodeSSMode(osg_SpiSlaveSelectMode mode)
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

static void _osg_board_spi_enableSpiInterrupt(osg_SpiId id)
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

static void _osg_board_spi_resetSpi(osg_SpiId id)
{
    switch (id)
    {
#ifdef SPI0
        case OSG_SPI0:
        {
            __HAL_RCC_SPI0_FORCE_RESET();
            __HAL_RCC_SPI0_RELEASE_RESET();
            __HAL_RCC_SPI0_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SPI0_IRQn);
            break;
        }
#endif
#ifdef SPI1
        case OSG_SPI1:
        {
            __HAL_RCC_SPI1_FORCE_RESET();
            __HAL_RCC_SPI1_RELEASE_RESET();
            __HAL_RCC_SPI1_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SPI1_IRQn);
            break;
        }
#endif
#ifdef SPI2
        case OSG_SPI2:
        {
            __HAL_RCC_SPI2_FORCE_RESET();
            __HAL_RCC_SPI2_RELEASE_RESET();
            __HAL_RCC_SPI2_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SPI2_IRQn);
            break;
        }
#endif
#ifdef SPI3
        case OSG_SPI3:
        {
            __HAL_RCC_SPI3_FORCE_RESET();
            __HAL_RCC_SPI3_RELEASE_RESET();
            __HAL_RCC_SPI3_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SPI3_IRQn);
            break;
        }
#endif
#ifdef SPI4
        case OSG_SPI4:
        {
            __HAL_RCC_SPI4_FORCE_RESET();
            __HAL_RCC_SPI4_RELEASE_RESET();
            __HAL_RCC_SPI4_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SPI4_IRQn);
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid SPI Port in SPI reset.");
    }
}

static void _osg_board_spi_deinitSSPins(osg_SpiId id, osg_GpioId * ssList, Size ssListSize)
{
    for (Size i = 0; i < ssListSize; ++i)
    {
        HAL_GPIO_DeInit(_osg_board_gpio_getPortHandler(osg_gpio_getPort(ssList[i])),
                        _osg_board_gpio_getBoardGpioPin(osg_gpio_getPin(ssList[i])));
    }
}

void osg_board_spi_ctor(osg_Spi * self, uint32_t clockSpeed, osg_SpiPolarity polarity, osg_SpiPhase phase, osg_SpiEndianess endianess, osg_SpiDataSize dataSize, osg_SpiSlaveSelectMode ssMode)
{
    if (_osg_board_spi_checkSpi(self->id) == FALSE)
    {
        osg_error("ERROR: SPI port NOT available on this device!");
        return;
    }

    if (_osg_board_spi_validatePins(self->id, self->sclkPin.id, self->mosiPin.id, self->misoPin.id) == FALSE)
        osg_error("ERROR: Impossible to use these PINs for this SPI port on this board!");

    _osg_board_spi_enableSpiClk(self->id);
    // Init PINs
    _osg_board_spi_initSSPins(self->id, self->ssPinList, self->ssPinListSize, ssMode);
    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(self->id);
    self->handler = &_osg_board_spi_peripherals[index].handler;

    _osg_board_spi_peripherals[index].handler.Instance = _osg_board_spi_getSpiInstance(self->id);
    _osg_board_spi_peripherals[index].handler.Init.Mode = _osg_board_spi_decodeMode(self->mode);
    _osg_board_spi_peripherals[index].handler.Init.BaudRatePrescaler = _osg_board_spi_decodeBaudRate(clockSpeed);
    _osg_board_spi_peripherals[index].handler.Init.Direction = SPI_DIRECTION_2LINES;
    _osg_board_spi_peripherals[index].handler.Init.CLKPhase = _osg_board_spi_decodePhase(phase);
    _osg_board_spi_peripherals[index].handler.Init.CLKPolarity = _osg_board_spi_decodePolarity(polarity);
    _osg_board_spi_peripherals[index].handler.Init.DataSize = _osg_board_spi_decodeDataSize(dataSize);
    _osg_board_spi_peripherals[index].handler.Init.FirstBit = _osg_board_spi_decodeEndianess(endianess);
    _osg_board_spi_peripherals[index].handler.Init.TIMode = SPI_TIMODE_DISABLE;
    _osg_board_spi_peripherals[index].handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    _osg_board_spi_peripherals[index].handler.Init.CRCPolynomial = 7;
    _osg_board_spi_peripherals[index].handler.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
    _osg_board_spi_peripherals[index].handler.Init.NSS = _osg_board_spi_decodeSSMode(ssMode);
    _osg_board_spi_peripherals[index].handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;

    __HAL_SPI_ENABLE((osg_board_SpiInstance *)self->handler);

    if (HAL_SPI_Init((osg_board_SpiInstance *)self->handler) != HAL_OK)
    {
        osg_error("ERROR: SPI init failed.");
    }

    _osg_board_spi_enableSpiInterrupt(self->id);

    _osg_board_spi_peripherals[index].rxBuffer = (osg_CircularFifo_isNull(&self->rxBuffer) == TRUE) ? NULL : &self->rxBuffer;
    _osg_board_spi_peripherals[index].txBuffer = (osg_CircularFifo_isNull(&self->txBuffer) == TRUE) ? NULL : &self->txBuffer;
    self->isConfigured = TRUE;
}

void osg_board_spi_dtor(osg_Spi * self)
{
    HAL_SPI_DeInit((osg_board_SpiInstance *)self->handler);

    _osg_board_spi_resetSpi(self->id);
    _osg_board_spi_deinitSSPins(self->id, self->ssPinList, self->ssPinListSize);

    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(self->id);
    _osg_board_spi_peripherals[index].callback = NULL;
    _osg_board_spi_peripherals[index].rxBuffer = NULL;
    _osg_board_spi_peripherals[index].txBuffer = NULL;

    self->id = OSG_SPI7;
    self->mode = OSG_SPI_SLAVE_MODE;
    self->handler = NULL;
}

Bool osg_board_spi_sendBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout)
{
    // Set SS pin low to activate the slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef ret = HAL_SPI_Transmit(self->handler,
                                                   (uint8_t *) buffer,
                                                   (uint16_t) bufferSize,
                                                   timeout);

    // Set the SS pin high to end the communication with selected slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

    if (ret == HAL_OK)
        return TRUE;

    return FALSE;
}

Bool osg_board_spi_receiveBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, uint32_t timeout)
{
    // Set SS pin low to activate the slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef ret = HAL_SPI_Receive(self->handler,
                                                  (uint8_t *) buffer,
                                                  (uint16_t) bufferSize,
                                                  timeout);

    // Set the SS pin high to end the communication with selected slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

    if (ret == HAL_OK)
        return TRUE;

    return FALSE;
}

Bool osg_board_spi_sendNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    // Set SS pin low to activate the slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(self->id);
    _osg_board_spi_peripherals[index].callback = callback;
    const HAL_StatusTypeDef ret = HAL_SPI_Transmit_IT(self->handler,
                                                      (uint8_t *) buffer,
                                                      (uint16_t) bufferSize);

    // Set the SS pin high to end the communication with selected slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

    if (ret == HAL_OK)
    {
        _osg_board_spi_peripherals[index].isBufferTransfer = TRUE;
        return TRUE;
    }

    return FALSE;
}

Bool osg_board_spi_receiveNonBlocking(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
   // Set SS pin low to activate the slave
   if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(self->id);
    _osg_board_spi_peripherals[index].callback = callback;
    const HAL_StatusTypeDef ret = HAL_SPI_Receive_IT(self->handler,
                                                     (uint8_t *) buffer,
                                                     (uint16_t) bufferSize);

    // Set the SS pin high to end the communication with selected slave
    if (self->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

    if (ret == HAL_OK)
        return TRUE;

    return FALSE;
}

Size osg_board_spi_sendBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize, osg_SpiCallback callback)
{
    if (osg_CircularFifo_isNull(&self->txBuffer) == TRUE)
        return 0;

    const Size freeSpace =  osg_CircularFifo_getFreeSize(&self->txBuffer);
    if (freeSpace < sizeof(osg_board_SpiInfo) + bufferSize)
        return 0;

    osg_board_SpiInfo info;
    info.destination = ssPinIndex;
    info.size = bufferSize;

    const Size infoWrote = osg_CircularFifo_pushBuffer(&self->txBuffer, &info, sizeof(info));
    if (infoWrote != sizeof(info))
        return 0;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&self->txBuffer, buffer, bufferSize);
    if (bytesWrote != bufferSize)
        return 0;

    void * buf = NULL;
    Size s = 0;
    osg_CircularFifo_getFilledBuffer(&self->txBuffer, &buf, &s);
    if (osg_board_spi_sendNonBlocking(self, ssPinIndex, buf, s, callback) == TRUE)
    {
        uint8_t index = _osg_board_spi_getPeripheralIndexFromId(self->id);
        _osg_board_spi_peripherals[index].isBufferTransfer = TRUE;
    }

    return bytesWrote;
}

static void _osg_board_spi_emptyBuffer(osg_CircularFifo * buffer)
{}

Size osg_board_spi_receivedBuffered(osg_Spi * self, uint16_t ssPinIndex, void * buffer, Size bufferSize)
{
    if (osg_CircularFifo_isNull(&self->rxBuffer) == TRUE)
        return 0;

    Size bytesRead = osg_CircularFifo_popBuffer(&self->rxBuffer, buffer, bufferSize);

    return bytesRead;
}

Size osg_board_spi_getSizeOfReceivedData(osg_Spi * self)
{
    if (osg_CircularFifo_isNull(&self->rxBuffer) == FALSE)
        return osg_CircularFifo_getSize(&self->rxBuffer);

    return 0;
}

// STM callback for non blocking trasfer
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    uint8_t index = _osg_board_spi_getPeripheralIndexFromInstance(hspi);

    // store received bytes in Rx FIFO buffer if it is initialized
    if (_osg_board_spi_peripherals[index].rxBuffer != NULL
        && hspi->RxXferCount > 0)
    {
        osg_CircularFifo_pushBuffer(_osg_board_spi_peripherals[index].rxBuffer,
                                    hspi->pRxBuffPtr,
                                    hspi->RxXferCount);
    }

    // pop-out data from Tx FIFO buffer
    if (_osg_board_spi_peripherals[index].txBuffer != NULL
        && hspi->TxXferCount > 0)
    {
        // we have to pop out data from tx FIFO only if the transfer has sent data form this buffer
        if (_osg_board_spi_peripherals[index].isBufferTransfer == TRUE)
        {
            osg_CircularFifo_popBytes(_osg_board_spi_peripherals[index].txBuffer, hspi->TxXferCount);

            // we have other data to send
            if (osg_CircularFifo_isEmpty(_osg_board_spi_peripherals[index].txBuffer) == FALSE)
            {
            }
        }
    }

    // reset simple/buffer non-blocking modes flag
    _osg_board_spi_peripherals[index].isBufferTransfer = FALSE;

    // call the user callback if present
    if (_osg_board_spi_peripherals[index].callback != NULL)
    {
        _osg_board_spi_peripherals[index].callback();
    }
}

// STM callback for non blocking transfer interrupted for error
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    osg_error("ERROR: SPI communication error.");
}

#ifdef SPI0
void SPI0_IRQHandler(void)
{
    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(OSG_SPI0);
    HAL_SPI_IRQHandler(&_osg_board_spi_peripherals[index].handler);
}
#endif
#ifdef SPI1
void SPI1_IRQHandler(void)
{
    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(OSG_SPI1);
    HAL_SPI_IRQHandler(&_osg_board_spi_peripherals[index].handler);
}
#endif
#ifdef SPI2
void SPI2_IRQHandler(void)
{
    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(OSG_SPI2);
    HAL_SPI_IRQHandler(&_osg_board_spi_peripherals[index].handler);
}
#endif
#ifdef SPI3
void SPI3_IRQHandler(void)
{
    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(OSG_SPI3);
    HAL_SPI_IRQHandler(&_osg_board_spi_peripherals[index].handler);
}
#endif
#ifdef SPI4
void SPI4_IRQHandler(void)
{
    uint8_t index = _osg_board_spi_getPeripheralIndexFromId(OSG_SPI4);
    HAL_SPI_IRQHandler(&_osg_board_spi_peripherals[index].handler);
}
#endif
