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
#include "../../../../include/board/drivers/board-I2c.h"
#include "board-I2cSupport.h"
#include "../../../../include/board/drivers/board-Gpio.h"
#include "../../../../../osg/include/osg.h"

void osg_board_I2c_ctor(osg_I2c * self, const osg_I2cConfig * const config)
{
    if (_osg_board_I2c_checkI2c(self->id) == false)
    {
        osg_error("ERROR: I2C port NOT available on this device!");
    }

    if (_osg_board_I2c_validatePins(self->id, self->sdaPin.id, self->sclPin.id) == false)
        osg_error("ERROR: Impossible to use these PINs for this I2C port on this board!");

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    i2c->handler = _osg_board_I2c_getI2cInstance(self->id);
    i2c->flags = OSG_I2C_FLAG_DEFAULT;


    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        i2c_master_config_t masterConfig;
        // masterConfig->baudRate_Bps = 100000U;
        // masterConfig->enableStopHold = false;
        // masterConfig->glitchFilterWidth = 0U;
        // masterConfig->enableMaster = true;
        I2C_MasterGetDefaultConfig(&masterConfig);
        masterConfig.baudRate_Bps = config->clockSpeed;

        I2C_MasterInit((osg_board_I2cInstance *)i2c->handler, &masterConfig, _osg_board_I2c_getI2cClk(self->id));
    }
    else
    {
        i2c_slave_config_t slaveConfig;
        // slaveConfig->addressingMode = kI2C_Address7bit;
        // slaveConfig->enableGeneralCall = false;
        // slaveConfig->enableWakeUp = false;
        // slaveConfig->enableBaudRateCtl = false;
        // slaveConfig->enableSlave = true;
        I2C_SlaveGetDefaultConfig(&slaveConfig);

        slaveConfig.addressingMode = _osg_board_I2c_decodeAddressingMode(config->addressingMode);
        if (config->enableGeneralCall == true)
            slaveConfig.enableGeneralCall = true;
        slaveConfig.slaveAddress = config->ownAddress;
        slaveConfig.upperAddress = 0;

        I2C_SlaveInit((osg_board_I2cInstance *)i2c->handler, &slaveConfig, _osg_board_I2c_getI2cClk(self->id));
    }

#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    if (mutex->status != OSG_MUTEX_STATUS_CONFIGURED)
    {
        _osg_board_I2c_configureMutex(self->id);
        osg_Mutex_ctor(mutex, OSG_MUTEX_RECURSIVE_ENABLED | OSG_MUTEX_RELEASE_WHEN_THREAD_EXIT, "I2C mutex.");
        mutex->status = OSG_MUTEX_STATUS_CONFIGURED;
    }
#endif
}

void osg_board_I2c_dtor(osg_I2c * self)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (self->mode == OSG_I2C_MASTER_MODE)
    {
        I2C_MasterDeinit((osg_board_I2cInstance *)i2c->handler);
    }
    else
    {
        I2C_SlaveDeinit((osg_board_I2cInstance *)i2c->handler);
    }

    i2c->txCompleteNb = NULL;
    i2c->rxCompleteNb = NULL;
    i2c->txCompleteBuffered = NULL;
    i2c->rxCompleteBuffered = NULL;
    osg_CircularFifo_dtor(&i2c->txFifo);
    osg_CircularFifo_dtor(&i2c->rxFifo);
    i2c->handler = NULL;
}

void osg_board_I2c_setBuffers(osg_I2c * self, void * circularTxBuffer, const Size txBufferSize, void * circularRxBuffer, const Size rxBufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    osg_CircularFifoConfig circularConfig;
    memset(&circularConfig, 0, sizeof(circularConfig));

    circularConfig.buffer = circularRxBuffer;
    circularConfig.size = rxBufferSize;
    circularConfig.behavior = OSG_FIFO_OVERWRITE_NO_ERROR;
    osg_CircularFifo_ctor(&i2c->rxFifo, &circularConfig);

    circularConfig.buffer = circularTxBuffer;
    circularConfig.size = txBufferSize;
    circularConfig.behavior = OSG_FIFO_ERROR;
    osg_CircularFifo_ctor(&i2c->txFifo, &circularConfig);
}

void osg_board_I2c_setBufferedCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    i2c->txCompleteBuffered = tx;
    i2c->rxCompleteBuffered = rx;
}

void osg_board_I2c_setNbCallbacks(osg_I2c * self, osg_I2cCallback tx, osg_I2cCallback rx)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    i2c->txCompleteNb = tx;
    i2c->rxCompleteNb = rx;
}

bool osg_board_I2c_masterSendBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size, const uint32_t timeout)
{
#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: I2C mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);
    int state = -1;

    i2c_master_transfer_t masterXfer;
    masterXfer.slaveAddress = slaveAddress;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = (void *)buffer;
    masterXfer.dataSize = size;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_I2c_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);
    I2C_MasterTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                   &handle->masterHandle,
                                   _osg_board_I2c_masterBlockingCallback,
                                   &state);
    const status_t res = I2C_MasterTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                       &handle->masterHandle,
                                                       &masterXfer);

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

            if (state == kStatus_I2C_Timeout)
            {
                ret = false;
                break;
            }
        }
        
        if (ret == false)
            I2C_SlaveTransferAbort((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle);
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return false;
}

bool osg_board_I2c_masterSendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);

    i2c_master_transfer_t masterXfer;
    masterXfer.slaveAddress = slaveAddress;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = (void *)buffer;
    masterXfer.dataSize = size;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_SEND | OSG_I2C_FLAG_IS_SEND;

    I2C_MasterTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                   &handle->masterHandle,
                                   _osg_board_I2c_masterSendCallback,
                                   NULL);
    const status_t res = I2C_MasterTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                       &handle->masterHandle,
                                                       &masterXfer);

    const bool ret = osg_bool(res == kStatus_Success);
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

bool osg_board_I2c_masterSendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    if (osg_CircularFifo_isNull(&i2c->txFifo) == true)
        return false;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&i2c->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return false;

    i2c->slaveAddress = slaveAddress;
    _osg_board_I2c_doBufferedSend(i2c, self->mode);

    return true;
}


bool osg_board_I2c_masterReceiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout)
{
#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: I2C mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);
    int state = -1;

    i2c_master_transfer_t masterXfer;
    masterXfer.slaveAddress = slaveAddress;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = buffer;
    masterXfer.dataSize = size;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_I2c_timeoutCallback;
    config.argument = &state;
    osg_Timer_ctor(&timer, &config);
    I2C_MasterTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                   &handle->masterHandle,
                                   _osg_board_I2c_masterBlockingCallback,
                                   &state);
    const status_t res = I2C_MasterTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                       &handle->masterHandle,
                                                       &masterXfer);

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

            if (state == kStatus_I2C_Timeout)
            {
                ret = false;
                break;
            }
        }
        
        if (ret == false)
            I2C_SlaveTransferAbort((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle);
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return false;
}

bool osg_board_I2c_masterReceiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);

    i2c_master_transfer_t masterXfer;
    masterXfer.slaveAddress = slaveAddress;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = (void *)buffer;
    masterXfer.dataSize = size;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return false;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_RECEIVE | OSG_I2C_FLAG_IS_RECEIVE;

    I2C_MasterTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                   &handle->masterHandle,
                                   _osg_board_I2c_masterRecvCallback,
                                   NULL);
    const status_t status = I2C_MasterTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                          &handle->masterHandle,
                                                          &masterXfer);

    const bool ret = osg_bool(status == kStatus_Success);
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

Size osg_board_I2c_masterReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (osg_CircularFifo_isNull(&i2c->rxFifo) == true)
        return 0;

    return osg_CircularFifo_popBuffer(&i2c->rxFifo, buffer, bufferSize);
}

bool osg_board_I2c_slaveSendBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout)
{
#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: I2C mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);
    osg_board_I2cSlaveData transfer;
    transfer.state = -1;
    transfer.dataSize = size;
    transfer.data = buffer;

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_I2c_timeoutCallback;
    config.argument = &transfer.state;
    osg_Timer_ctor(&timer, &config);
    I2C_SlaveTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                  &handle->slaveHandle,
                                  _osg_board_I2c_slaveBlockingCallback,
                                  &transfer);
    const status_t res = I2C_SlaveTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                      &handle->slaveHandle,
                                                      kI2C_SlaveAllEvents);

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
            if (transfer.state == kStatus_Success)
            {
                ret = true;
                break;
            }

            if (transfer.state == kStatus_I2C_Timeout)
            {
                ret = false;
                break;
            }
        }
        
        if (ret == false)
            I2C_SlaveTransferAbort((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle);
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return ret;
}

bool osg_board_I2c_slaveSendNonBlocking(osg_I2c * self, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);

    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_SEND | OSG_I2C_FLAG_IS_SEND;

    osg_board_I2cSlaveData * nbXfer = _osg_board_I2c_getNonBlockingTransferSlaveData();
    nbXfer->state = -1;
    nbXfer->dataSize = size;
    nbXfer->data = buffer;

    I2C_SlaveTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                  &handle->slaveHandle,
                                  _osg_board_I2c_slaveSendRecvCallback,
                                  &nbXfer);
    const status_t res = I2C_SlaveTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                      &handle->slaveHandle,
                                                      kI2C_SlaveAllEvents);

    const bool ret = osg_bool(res == kStatus_Success);
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

bool osg_board_I2c_slaveSendBuffered(osg_I2c * self, const void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    if (osg_CircularFifo_isNull(&i2c->txFifo) == true)
        return false;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&i2c->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return false;

    _osg_board_I2c_doBufferedSend(i2c, self->mode);

    return true;
}

bool osg_board_I2c_slaveReceiveBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout)
{
#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: I2C mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return false;
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);
    osg_board_I2cSlaveData transfer;
    transfer.state = -1;
    transfer.dataSize = size;
    transfer.data = buffer;

    osg_Timer timer;
    osg_TimerConfig config;
    config.type = OSG_TIMER_ONE_SHOT;
    config.callback = _osg_board_I2c_timeoutCallback;
    config.argument = &transfer.state;
    osg_Timer_ctor(&timer, &config);
    i2c->flags &= ~OSG_I2C_FLAG_HANDLE_CREATED;
    I2C_SlaveTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                  &handle->slaveHandle,
                                  _osg_board_I2c_slaveBlockingCallback,
                                  &transfer);
    const status_t res = I2C_SlaveTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                      &handle->slaveHandle,
                                                      kI2C_SlaveAllEvents);

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
            if (transfer.state == kStatus_Success)
            {
                ret = true;
                break;
            }

            if (transfer.state == kStatus_I2C_Timeout)
            {
                ret = false;
                break;
            }
        }
        
        if (ret == false)
            I2C_SlaveTransferAbort((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle);
        
        osg_Timer_stop(&timer);
    }
    
    osg_Timer_dtor(&timer);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return ret;
}

bool osg_board_I2c_slaveReceiveNonBlocking(osg_I2c * self, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return false;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_RECEIVE | OSG_I2C_FLAG_IS_RECEIVE;

    osg_board_I2cSlaveData * nbXfer = _osg_board_I2c_getNonBlockingTransferSlaveData();
    nbXfer->state = -1;
    nbXfer->dataSize = size;
    nbXfer->data = buffer;

    I2C_SlaveTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                  &handle->slaveHandle,
                                   _osg_board_I2c_slaveSendRecvCallback,
                                  &nbXfer);
    const status_t res = I2C_SlaveTransferNonBlocking((osg_board_I2cInstance *)i2c->handler,
                                                      &handle->slaveHandle,
                                                      kI2C_SlaveAllEvents);

    const bool ret = osg_bool(res == kStatus_Success);
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

Size osg_board_I2c_slaveReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (osg_CircularFifo_isNull(&i2c->rxFifo) == true)
        return 0;

    return osg_CircularFifo_popBuffer(&i2c->rxFifo, buffer, bufferSize);
}

void osg_board_I2c_startReceiveBuffered(osg_I2c * self, const uint16_t slaveAddress)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_assert(osg_CircularFifo_isNull(&i2c->rxFifo) == false, "ASSERT FAILED: I2C Rx Buffer is NULL");

    i2c->flags |= OSG_I2C_FLAG_RX_BUFF_ENABLED | OSG_I2C_FLAG_IS_RECEIVE;
    if (self->mode == OSG_I2C_MASTER_MODE)
        i2c->slaveAddress = slaveAddress;
    _osg_board_I2c_doBufferedReceive(i2c, self->mode);
}

void osg_board_I2c_stopReceiveBuffered(osg_I2c * self)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_assert(osg_CircularFifo_isNull(&i2c->rxFifo) == false, "ASSERT FAILED: I2C Rx Buffer is NULL");
    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandler(self->id);

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED)
    {
        if (self->mode == OSG_I2C_MASTER_MODE)
        {
            I2C_MasterTransferAbort((osg_board_I2cInstance *)i2c->handler, &handle->masterHandle);
        }
        else
        {
            I2C_SlaveTransferAbort((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle);
        }
    }
    osg_CircularFifo_clear(&i2c->rxFifo);
}

bool osg_board_I2c_isReceiveBufferedEnabled(osg_I2c * self)
{
    return osg_bool(_osg_board_I2c_getI2cById(self->id)->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED);
}

bool osg_board_I2c_writeMemBlocking(osg_I2c * self, const uint16_t slaveAddress, const uint16_t memAddress, const uint16_t memAddrSize, void * buffer, const Size size, const uint32_t timeout)
{
#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: I2C mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    // start, address and W
    status_t ret = I2C_MasterStart((osg_board_I2cInstance *)i2c->handler, (uint8_t)slaveAddress, kI2C_Write);
    if (ret != kStatus_Success)
        return false;

    // write memory address
    ret = I2C_MasterWriteBlocking((osg_board_I2cInstance *)i2c->handler, (uint8_t *)memAddress, (Size)memAddrSize, kI2C_TransferNoStopFlag);
    if (ret != kStatus_Success)
        return false;

    // restart, address and W
    ret = I2C_MasterRepeatedStart((osg_board_I2cInstance *)i2c->handler, (uint8_t)slaveAddress, kI2C_Write);
    if (ret != kStatus_Success)
        return false;

    // write data
    ret = I2C_MasterWriteBlocking((osg_board_I2cInstance *)i2c->handler, (uint8_t *)buffer, size, kI2C_TransferDefaultFlag);
    //if (ret != kStatus_Success)
    //    return false;

    // stop. USEFUL?
    //ret = I2C_MasterStop((osg_board_I2cInstance *)i2c->handler);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == kStatus_Success);
}

bool osg_board_I2c_readMemBlocking(osg_I2c * self, const uint16_t slaveAddress, const uint16_t memAddress, const uint16_t memAddrSize, void * buffer, const Size size, const uint32_t timeout)
{
#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    osg_assert(mutex->status == OSG_MUTEX_STATUS_CONFIGURED, "ASSERT FAILED: I2C mutex is not created.");
    const bool acq = osg_Mutex_acquire(mutex, 0);
    if (acq == false)
        return false;
#endif

    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    // start, address and W
    status_t ret = I2C_MasterStart((osg_board_I2cInstance *)i2c->handler, (uint8_t)slaveAddress, kI2C_Write);
    if (ret != kStatus_Success)
        return false;

    // write memory address
    ret = I2C_MasterWriteBlocking((osg_board_I2cInstance *)i2c->handler, (uint8_t *)memAddress, (Size)memAddrSize, kI2C_TransferNoStopFlag);
    if (ret != kStatus_Success)
        return false;

    // restart, address and R
    ret = I2C_MasterRepeatedStart((osg_board_I2cInstance *)i2c->handler, (uint8_t)slaveAddress, kI2C_Read);
    if (ret != kStatus_Success)
        return false;

    // read data
    ret = I2C_MasterReadBlocking((osg_board_I2cInstance *)i2c->handler, (uint8_t *)buffer, size, kI2C_TransferDefaultFlag);
    //if (ret != kStatus_Success)
    //    return false;

    // stop. USEFUL?
    //ret = I2C_MasterStop((osg_board_I2cInstance *)i2c->handler);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == kStatus_Success);
}

uint8_t osg_board_I2c_countI2cs()
{
    return _osg_board_I2c_getI2cs();
}
