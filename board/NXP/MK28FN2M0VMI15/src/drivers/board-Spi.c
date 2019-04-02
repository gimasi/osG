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
        osg_error("ERROR: SPI port not available on this device.");
    }

    if (_osg_board_Spi_validateSpiPins(self->id, config->mode, self->sclkPin.id, self->mosiPin.id, self->misoPin.id) == false)
    {
        osg_error("ERROR: Wrong PINs configuration for SPI port on this board.");
    }

    if (_osg_board_Spi_validateSsPins(self->id, config->mode, self->ssPinList, self->ssPinListSize) == false)
    {
        osg_error("ERROR: Invalid Slave Select PINs selection. Please refer to the board datasheet.");
    }
    _osg_board_Spi_initSsPins(self->id, config->ssMode, self->ssPinList, self->ssPinListSize, 0);

    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    spi->handler = _osg_board_Spi_getSpiInstance(self->id);
    spi->flags = OSG_SPI_FLAG_DEFAULT;
    spi->mode = config->mode;

    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        dspi_master_config_t masterConfig;
        masterConfig.whichCtar = kDSPI_Ctar0;
        masterConfig.ctarConfig.baudRate = config->clockSpeed;
        masterConfig.ctarConfig.bitsPerFrame = _osg_board_Spi_decodeDataSize(config->dataSize);
        masterConfig.ctarConfig.cpol = _osg_board_Spi_decodePolarity(config->polarity);
        masterConfig.ctarConfig.cpha = _osg_board_Spi_decodePhase(config->phase);
        masterConfig.ctarConfig.direction = _osg_board_Spi_decodeEndianess(config->endianess);
        masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / config->clockSpeed;
        masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / config->clockSpeed;
        masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / config->clockSpeed;

        // @todo: return the correct PCS
        masterConfig.whichPcs = kDSPI_Pcs1;
        masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

        masterConfig.enableContinuousSCK = false;
        masterConfig.enableRxFifoOverWrite = false; // NOTE: can be useful to set true?
        masterConfig.enableModifiedTimingFormat = false;
        masterConfig.samplePoint = kDSPI_SckToSin0Clock;

        DSPI_MasterInit((osg_board_SpiInstance *)spi->handler, &masterConfig, _osg_board_Spi_getSpiClk(self->id));
    }
    else
    {
        dspi_slave_config_t slaveConfig;
        slaveConfig.whichCtar = kDSPI_Ctar0;
        slaveConfig.ctarConfig.bitsPerFrame = _osg_board_Spi_decodeDataSize(config->dataSize);
        slaveConfig.ctarConfig.cpol = _osg_board_Spi_decodePolarity(config->polarity);
        slaveConfig.ctarConfig.cpha = _osg_board_Spi_decodePhase(config->phase);

        slaveConfig.enableContinuousSCK = false;
        slaveConfig.enableRxFifoOverWrite = false; // NOTE: can be useful to set true?
        slaveConfig.enableModifiedTimingFormat = false;
        slaveConfig.samplePoint = kDSPI_SckToSin0Clock;

        DSPI_SlaveInit((osg_board_SpiInstance *)spi->handler, &slaveConfig);
    }

#if SPI_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_Spi_getMutex(self->id);
    if (mutex->status != OSG_MUTEX_STATUS_CONFIGURED)
    {
        _osg_board_Spi_configureMutex(self->id);
        osg_Mutex_ctor(mutex, OSG_MUTEX_RECURSIVE_ENABLED | OSG_MUTEX_RELEASE_WHEN_THREAD_EXIT, "SPI mutex.");
        mutex->status = OSG_MUTEX_STATUS_CONFIGURED;
    }
#endif
}

void osg_board_Spi_dtor(osg_Spi * self)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    DSPI_Deinit((osg_board_SpiInstance *)spi->handler);

    _osg_board_Spi_deinitSSPins(self->id, self->ssPinList, self->ssPinListSize);

    spi->mode = OSG_SPI_SLAVE_MODE;
    spi->txCompleteNb = NULL;
    spi->rxCompleteNb = NULL;
    spi->txCompleteBuffered = NULL;
    spi->rxCompleteBuffered = NULL;
    osg_CircularFifo_dtor(&spi->txFifo);
    osg_CircularFifo_dtor(&spi->rxFifo);
    spi->handler = NULL;
    spi->flags = OSG_SPI_FLAG_DEFAULT;
    spi->rxByte = 0;
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

void osg_board_Spi_setBufferedCallbacks(osg_Spi * self, const osg_SpiCallback tx, const  osg_SpiCallback rx)
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
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandler(self->id);
    int state = -1;

    dspi_transfer_t xfer = {0};
    xfer.txData = buffer;
    xfer.rxData = NULL;
    xfer.dataSize = bufferSize;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    }
    else
    {
        xfer.configFlags = kDSPI_SlaveCtar0;
    }

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_Spi_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);
    status_t res;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterBlockingCallback,
                                        &state);
        res = DSPI_MasterTransferNonBlocking(instance,
                                             &handle->masterHandle,
                                             &xfer);
    }
    else
    {
        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveBlockingCallback,
                                       &state);
        res = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }

    bool ret = false;
    if (res != kStatus_Success)
    {
        ret = false;
    }
    else
    {
        osg_Timer_start(&timer, timeout);
        while (true)
        {
            if (state == kStatus_Success)
            {
                ret = true;
                break;
            }

            if (state == kStatus_DSPI_Error)
            {
                ret = false;
                break;
            }
        }

        if (ret == false)
        {
            if (spi->mode == OSG_SPI_MASTER_MODE)
                DSPI_MasterTransferAbort(instance, &handle->masterHandle);
            else
                DSPI_SlaveTransferAbort(instance, &handle->slaveHandle);
        }
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if SPI_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release SPI mutex.");
#endif

    return false;
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
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandler(self->id);
    int state = -1;

    dspi_transfer_t xfer = {0};
    xfer.txData = NULL;
    xfer.rxData = buffer;
    xfer.dataSize = bufferSize;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    }
    else
    {
        xfer.configFlags = kDSPI_SlaveCtar0;
    }

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_Spi_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);
    status_t res;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterBlockingCallback,
                                        &state);
        res = DSPI_MasterTransferNonBlocking(instance,
                                             &handle->masterHandle,
                                             &xfer);
    }
    else
    {
        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveBlockingCallback,
                                       &state);
        res = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }

    bool ret = false;
    if (res != kStatus_Success)
    {
        ret = false;
    }
    else
    {
        osg_Timer_start(&timer, timeout);
        while (true)
        {
            if (state == kStatus_Success)
            {
                ret = true;
                break;
            }

            if (state == kStatus_DSPI_Error)
            {
                ret = false;
                break;
            }
        }

        if (ret == false)
        {
            if (spi->mode == OSG_SPI_MASTER_MODE)
                DSPI_MasterTransferAbort(instance, &handle->masterHandle);
            else
                DSPI_SlaveTransferAbort(instance, &handle->slaveHandle);
        }
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if SPI_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release SPI mutex.");
#endif

    return false;
}

bool osg_board_Spi_sendNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandler(self->id);

    dspi_transfer_t xfer = {0};
    xfer.txData = buffer;
    xfer.rxData = NULL;
    xfer.dataSize = bufferSize;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    }
    else
    {
        xfer.configFlags = kDSPI_SlaveCtar0;
    }

    const osg_SpiFlags restore = spi->flags;
    spi->flags |= OSG_SPI_FLAG_IS_NB_SEND | OSG_SPI_FLAG_IS_SEND;

    status_t res;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterSendNBCallback,
                                        NULL);
        res = DSPI_MasterTransferNonBlocking(instance,
                                             &handle->masterHandle,
                                             &xfer);
    }
    else
    {
        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveSendNBCallback,
                                       NULL);
        res = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }

    const bool ret = res == kStatus_Success ? true : false;
    if (!ret)
    {
        spi->flags = restore;
    }

    return ret;
}

bool osg_board_Spi_receiveNonBlocking(osg_Spi * self, const uint16_t ssPinIndex, void * buffer, const Size bufferSize)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandler(self->id);

    dspi_transfer_t xfer = {0};
    xfer.txData = NULL;
    xfer.rxData = buffer;
    xfer.dataSize = bufferSize;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    }
    else
    {
        xfer.configFlags = kDSPI_SlaveCtar0;
    }

    if (spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED) return false;
    const osg_SpiFlags restore = spi->flags;
    spi->flags |= OSG_SPI_FLAG_IS_NB_RECEIVE | OSG_SPI_FLAG_IS_RECEIVE;

    status_t res;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterRecvNBCallback,
                                        NULL);
        res = DSPI_MasterTransferNonBlocking(instance,
                                             &handle->masterHandle,
                                             &xfer);
    }
    else
    {
        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveRecvNBCallback,
                                       NULL);
        res = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }

    const bool ret = res == kStatus_Success ? true : false;
    if (!ret)
    {
        spi->flags = restore;
    }

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

    spi->lastSsPin = self->ssPinList[ssPinIndex];
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
        spi->lastSsPin = self->ssPinList[ssPinIndex];
    _osg_board_Spi_doBufferedReceive(spi);
}

void osg_board_Spi_stopReceiveBuffered(osg_Spi * self)
{
    osg_SpiImpl * spi = _osg_board_Spi_getSpiById(self->id);
    osg_assert(osg_CircularFifo_isNull(&spi->rxFifo) == false, "ASSERT FAILED: SPI Rx Buffer is NULL");
    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandler(self->id);

    if (spi->flags & OSG_SPI_FLAG_RX_BUFF_ENABLED)
    {
        if (spi->mode == OSG_SPI_MASTER_MODE)
        {
            DSPI_MasterTransferAbort((osg_board_SpiInstance *)spi->handler, &handle->masterHandle);
        }
        else
        {
            DSPI_SlaveTransferAbort((osg_board_SpiInstance *)spi->handler, &handle->slaveHandle);
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
    osg_board_SpiInstance * instance = (osg_board_SpiInstance *)spi->handler;
    osg_board_SpiHandler * handle = _osg_board_Spi_getSpiHandler(self->id);
    int state = -1;

    dspi_transfer_t xfer = {0};
    xfer.txData = bufferTx;
    xfer.rxData = bufferRx;
    xfer.dataSize = transferSize;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        // @todo: select the correct slave
        xfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    }
    else
    {
        xfer.configFlags = kDSPI_SlaveCtar0;
    }

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_Spi_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);
    status_t res;
    if (spi->mode == OSG_SPI_MASTER_MODE)
    {
        DSPI_MasterTransferCreateHandle(instance,
                                        &handle->masterHandle,
                                        _osg_board_Spi_masterBlockingCallback,
                                        &state);
        res = DSPI_MasterTransferNonBlocking(instance,
                                             &handle->masterHandle,
                                             &xfer);
    }
    else
    {
        DSPI_SlaveTransferCreateHandle(instance,
                                       &handle->slaveHandle,
                                       _osg_board_Spi_slaveBlockingCallback,
                                       &state);
        res = DSPI_SlaveTransferNonBlocking(instance,
                                            &handle->slaveHandle,
                                            &xfer);
    }

    bool ret = false;
    if (res != kStatus_Success)
    {
        ret = false;
    }
    else
    {
        osg_Timer_start(&timer, timeout);
        while (true)
        {
            if (state == kStatus_Success)
            {
                ret = true;
                break;
            }

            if (state == kStatus_DSPI_Error)
            {
                ret = false;
                break;
            }
        }

        if (ret == false)
        {
            if (spi->mode == OSG_SPI_MASTER_MODE)
                DSPI_MasterTransferAbort(instance, &handle->masterHandle);
            else
                DSPI_SlaveTransferAbort(instance, &handle->slaveHandle);
        }
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if SPI_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release SPI mutex.");
#endif

    return ret;
}
