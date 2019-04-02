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
#include <string.h>
#include "../../../../include/board/drivers/board-Spi.h"
#include "board-SpiSupport.h"
#include "../../../../../osg/include/osg.h"

void osg_board_Spi_ctor(osg_Spi * self, const osg_SpiConfig * const config)
{
    if (_osg_board_Spi_checkSpi(self->id) == false)
    {
        osg_error("ERROR: SPI port NOT available on this device!");
        return;
    }

    if (_osg_board_Spi_validatePins(self->id, self->sclkPin.id, self->mosiPin.id, self->misoPin.id) == false)
        osg_error("ERROR: Impossible to use these PINs for this SPI port on this board!");

    _osg_board_Spi_resetSpi(self->id);
    _osg_board_Spi_enableSpiClk(self->id);
    // Init PINs
    _osg_board_Spi_initSSPins(self->id, self->ssPinList, self->ssPinListSize, config->ssMode, 0);
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    spi->handler = _osg_board_Spi_getSpiHandler(self->id);
    spi->mode = config->mode;

    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;
    handler->Instance = _osg_board_Spi_getSpiInstance(self->id);
    handler->Init.Mode = _osg_board_Spi_decodeMode(spi->mode);
    handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256/*_osg_board_Spi_decodeBaudRate(config->clockSpeed)*/;
    handler->Init.Direction = SPI_DIRECTION_2LINES;
    handler->Init.CLKPhase = _osg_board_Spi_decodePhase(config->phase);
    handler->Init.CLKPolarity = _osg_board_Spi_decodePolarity(config->polarity);
    handler->Init.DataSize = _osg_board_Spi_decodeDataSize(config->dataSize);
    handler->Init.FirstBit = _osg_board_Spi_decodeEndianess(config->endianess);
    handler->Init.TIMode = SPI_TIMODE_DISABLE;
    handler->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    handler->Init.CRCPolynomial = 7;
    handler->Init.CRCLength = SPI_CRC_LENGTH_8BIT;
    handler->Init.NSS = _osg_board_Spi_decodeSSMode(config->ssMode);
    handler->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;

    _osg_board_Spi_enableSpi(handler);

    if (HAL_SPI_Init(handler) != HAL_OK)
    {
        osg_error("ERROR: SPI init failed.");
    }

    _osg_board_Spi_enableSpiInterrupt(self->id);
    spi->flags = OSG_SPI_FLAG_DEFAULT;

#if SPI_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_Spi_getMutex(self->id);
    if (mutex->status != OSG_MUTEX_STATUS_CONFIGURED)
    {
        _osg_board_Spi_configureMutex(self->id);
        osg_MutexConfig conf;
        conf.attributes = OSG_MUTEX_RECURSIVE_ENABLED | OSG_MUTEX_RELEASE_WHEN_THREAD_EXIT;
        conf.name = "SPI mutex.";
        osg_Mutex_ctor(mutex, &conf);
        mutex->status = OSG_MUTEX_STATUS_CONFIGURED;
    }
#endif
}

void osg_board_Spi_dtor(osg_Spi * self)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;
    HAL_SPI_DeInit(handler);

    _osg_board_Spi_resetSpi(self->id);
    _osg_board_Spi_disableSpiClk(self->id);
    _osg_board_Spi_disableSpiInterrupt(self->id);
    _osg_board_Spi_deinitSSPins(self->id, self->ssPinList, self->ssPinListSize);

    spi->txCompleteBuffered = NULL;
    spi->rxCompleteBuffered = NULL;
    spi->txCompleteNb = NULL;
    spi->rxCompleteNb = NULL;
    osg_CircularFifo_dtor(&spi->txFifo);
    osg_CircularFifo_dtor(&spi->rxFifo);
    spi->handler = NULL;
    spi->flags = OSG_SPI_FLAG_DEFAULT;
    spi->mode = OSG_SPI_SLAVE_MODE;
}

void osg_board_Spi_setBuffers(osg_Spi * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);

    osg_CircularFifoConfig circularConfig;
    memset(&circularConfig, 0, sizeof(circularConfig));

    circularConfig.buffer = circularRxBuffer;
    circularConfig.size = rxBufferSize;
    circularConfig.behavior = OSG_FIFO_OVERWRITE_NO_ERROR;
    osg_CircularFifo_ctor(&spi->rxFifo, &circularConfig);

    circularConfig.buffer = circularTxBuffer;
    circularConfig.size = txBufferSize;
    circularConfig.behavior = OSG_FIFO_ERROR;
    osg_CircularFifo_ctor(&spi->txFifo, &circularConfig);
}

void osg_board_Spi_setBufferedCallbacks(osg_Spi * self, const osg_SpiCallback tx, const osg_SpiCallback rx)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    spi->txCompleteBuffered = tx;
    spi->rxCompleteBuffered = rx;
}

void osg_board_Spi_setNbCallbacks(osg_Spi * self, const osg_SpiCallback tx, const osg_SpiCallback rx)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    spi->txCompleteNb = tx;
    spi->rxCompleteNb = rx;
}

bool osg_board_Spi_sendBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize, const uint32_t timeout)
{
#if SPI_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_Spi_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: SPI mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;

    // Set SS pin low to activate the slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef ret = HAL_SPI_Transmit(handler,
                                                   (uint8_t *) buffer,
                                                   (uint16_t) bufferSize,
                                                   timeout);

    // Set the SS pin high to end the communication with selected slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

#if SPI_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release SPI mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

bool osg_board_Spi_receiveBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize, const uint32_t timeout)
{
#if SPI_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_Spi_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: SPI mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    if (spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED) return false;
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;

    // Set SS pin low to activate the slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef ret = HAL_SPI_Receive(handler,
                                                  (uint8_t *) buffer,
                                                  (uint16_t) bufferSize,
                                                  timeout);

    // Set the SS pin high to end the communication with selected slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

#if SPI_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release SPI mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

bool osg_board_Spi_sendNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;

    const osg_SpiFlags restore = spi->flags;
    spi->flags |= OSG_SPI_FLAG_IS_NB_SEND | OSG_SPI_FLAG_IS_SEND;

    // Set SS pin low to activate the slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef status = HAL_SPI_Transmit_IT(handler,
                                                      (uint8_t *) buffer,
                                                      (uint16_t) bufferSize);

    // Set the SS pin high to end the communication with selected slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

    const bool ret = osg_bool(status == HAL_OK);
    if (ret == false) spi->flags = restore;

    return ret;
}

bool osg_board_Spi_receiveNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;

    if (spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED) return false;
    const osg_SpiFlags restore = spi->flags;
    spi->flags |= OSG_SPI_FLAG_IS_NB_RECEIVE | OSG_SPI_FLAG_IS_RECEIVE;

    // Set SS pin low to activate the slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef status = HAL_SPI_Receive_IT(handler,
                                                     (uint8_t *) buffer,
                                                     (uint16_t) bufferSize);

    // Set the SS pin high to end the communication with selected slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

    const bool ret = osg_bool(ret == HAL_OK);
    if (ret == false) spi->flags = restore;

    return ret;
}

bool osg_board_Spi_sendBuffered(osg_Spi * self, const uint16_t ssPinIndex, const void * buffer, const Size bufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);

    if (osg_CircularFifo_isNull(&spi->txFifo) == true)
        return false;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&spi->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return false;

    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        _osg_board_Spi_activateSlave(spi->lastSsPin);
        spi->lastSsPin = self->ssPinList[ssPinIndex];
    }

    _osg_board_Spi_doBufferedSend(spi);

    return true;
}

Size osg_board_Spi_receiveBuffered(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    if (osg_CircularFifo_isNull(&spi->rxFifo) == true)
        return false;

    return osg_CircularFifo_popBuffer(&spi->rxFifo, buffer, bufferSize);
}

void osg_board_Spi_startReceiveBuffered(osg_Spi * self, const uint16_t ssPinIndex)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_assert(osg_CircularFifo_isNull(&spi->rxFifo) == false, "ASSERT FAILED: SPI Rx Buffer is NULL");

    spi->flags |= OSG_SPI_FLAG_RX_BUFF_ENABLED | OSG_SPI_FLAG_IS_RECEIVE;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        spi->lastSsPin = self->ssPinList[ssPinIndex];
        _osg_board_Spi_activateSlave(spi->lastSsPin);
    }
    _osg_board_Spi_doBufferedReceive(spi);
}

void osg_board_Spi_stopReceiveBuffered(osg_Spi * self)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_assert(osg_CircularFifo_isNull(&spi->rxFifo) == false, "ASSERT FAILED: SPI Rx Buffer is NULL");
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;

    if (spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED)
    {
        if (spi->mode == OSG_SPI_MASTER_MODE)
        {
             _osg_board_Spi_deactivateSlave(spi->lastSsPin);
        }

        const HAL_StatusTypeDef ret = HAL_SPI_Abort_IT(handler);
        if (ret == HAL_OK)
        {
            while (!(spi->flags & OSG_SPI_FLAG_RX_ABORTED))
            {
                // waiting for abort
            }

            spi->flags &= ~OSG_SPI_FLAG_IS_RECEIVE;
            spi->flags &= ~OSG_SPI_FLAG_RX_BUFF_ENABLED;
            spi->flags &= ~OSG_SPI_FLAG_RX_ABORTED;
        }
    }
    osg_CircularFifo_clear(&spi->rxFifo);
}

bool osg_board_Spi_isReceiveBufferedEnabled(osg_Spi * self)
{
    return osg_bool(_osg_board_Spi_getSpiById(self->id)->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED);
}

uint8_t osg_board_Spi_countSpis()
{
    return _osg_board_Spi_getSpis();
}

bool osg_board_Spi_sendReceiveBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * bufferTx, void * bufferRx, const Size transferSize, const uint32_t timeout)
{
#if SPI_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_Spi_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: SPI mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiHandler * handler = (osg_board_SpiHandler *)spi->handler;

    // Set SS pin low to activate the slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_activateSlave(self->ssPinList[ssPinIndex]);
    }

    const HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(handler,
                                                          (uint8_t *) bufferTx,
                                                          (uint8_t *) bufferRx,
                                                          (uint16_t) transferSize,
                                                          timeout);

    // Set the SS pin high to end the communication with selected slave
    if (spi->mode == OSG_SPI_MASTER_MODE
        && ssPinIndex < self->ssPinListSize)
    {
        _osg_board_Spi_deactivateSlave(self->ssPinList[ssPinIndex]);
    }

#if SPI_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release SPI mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

// STM callback for non blocking trasfer
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiByHandler(hspi);

    if (spi->flags & OSG_SPI_FLAG_IS_SEND)
    {
        if (spi->flags & OSG_SPI_FLAG_IS_NB_SEND)
        {
            if (spi->txCompleteNb != NULL) spi->txCompleteNb();
        }
        else
        {
            osg_board_SpiHandler * handler = (osg_board_SpiHandler *) spi->handler;
            osg_CircularFifo_popBytes(&spi->txFifo, handler->TxXferSize - handler->TxXferCount);
            if (spi->txCompleteBuffered != NULL) spi->txCompleteBuffered();
        }
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_SEND;
        _osg_board_Spi_doBufferedSend(spi);
    }

    if (spi->flags & OSG_SPI_FLAG_IS_RECEIVE)
    {
        if (spi->flags & OSG_SPI_FLAG_IS_NB_RECEIVE)
        {
            if (spi->rxCompleteNb != NULL) spi->rxCompleteNb();
        }
        else
        {
            osg_CircularFifo_pushBuffer(&spi->rxFifo, &spi->rxByte, 1);
            if (spi->rxCompleteBuffered != NULL) spi->rxCompleteBuffered();
        }
        spi->flags &= ~OSG_SPI_FLAG_IS_NB_RECEIVE;
        _osg_board_Spi_doBufferedReceive(spi);
    }
}
