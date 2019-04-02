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
#include "../../../../../osg/include/osg.h"

void osg_board_I2c_ctor(osg_I2c * self, const osg_I2cConfig * config)
{
    if (_osg_board_I2c_checkI2c(self->id) == false)
    {
        osg_error("ERROR: I2C port NOT available on this device!");
    }

    if (_osg_board_I2c_validatePins(self->id, self->sdaPin.id, self->sclPin.id) == false)
        osg_error("ERROR: Impossible to use these PINs for this I2C port on this board!");

    _osg_board_I2c_enableI2cClk(self->id);
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    i2c->handler = _osg_board_I2c_getI2cHandler(self->id);

    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;
    handler->Instance = _osg_board_I2c_getI2cInstance(self->id);

    handler->Init.Timing = config->clockSpeed;
    handler->Init.AddressingMode = _osg_board_I2c_decodeAddressingMode(config->addressingMode);
    handler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    handler->Init.GeneralCallMode = config->enableGeneralCall == true ? I2C_GENERALCALL_ENABLE : I2C_GENERALCALL_DISABLE;
    handler->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    handler->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    handler->Init.OwnAddress1 = (config->ownAddress << 1);
    handler->Init.OwnAddress2 = 0;

    i2c->flags = OSG_I2C_FLAG_DEFAULT;

    _osg_board_I2c_enableI2c(handler);

    if (HAL_I2C_Init(handler) != HAL_OK)
    {
        osg_error("ERROR: I2C init failed.");
    }

    // Configure Analogue filter
    if (HAL_I2CEx_ConfigAnalogFilter(handler, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
    {
        osg_error("ERROR: I2C analog filter init failed.");
    }

    // Configure Digital filter
    if (HAL_I2CEx_ConfigDigitalFilter(handler, 0) != HAL_OK)
    {
        osg_error("ERROR: I2C digital filter init failed.");
    }

    _osg_board_I2c_enableI2cInterrupt(self->id);

#if I2C_MUTEX_ENABLED
    osg_Mutex * mutex = _osg_board_I2c_getMutex(self->id);
    if (mutex->status != OSG_MUTEX_STATUS_CONFIGURED)
    {
        _osg_board_I2c_configureMutex(self->id);
        osg_MutexConfig conf;
        conf.attributes = OSG_MUTEX_RECURSIVE_ENABLED | OSG_MUTEX_RELEASE_WHEN_THREAD_EXIT;
        conf.name = "I2C mutex.";
        osg_Mutex_ctor(mutex, &conf);
        mutex->status = OSG_MUTEX_STATUS_CONFIGURED;
    }
#endif
}

void osg_board_I2c_dtor(osg_I2c * self)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;
    HAL_I2C_DeInit(handler);
    _osg_board_I2c_resetI2c(self->id);
    _osg_board_I2c_disableI2cClk(self->id);
    _osg_board_I2c_disableI2cInterrupt(self->id);

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
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(handler,
                                                          (slaveAddress << 1),
                                                          (uint8_t *)buffer,
                                                          (uint16_t)size,
                                                          timeout);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

bool osg_board_I2c_masterSendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_SEND | OSG_I2C_FLAG_IS_SEND;

    const HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(handler,
                                                                (slaveAddress << 1),
                                                                (uint8_t *)buffer,
                                                                (uint16_t)size);

    const bool ret = osg_bool(status == HAL_OK);
    if (ret == false) i2c->flags = restore;

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
    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return false;
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(handler,
                                                         (slaveAddress << 1),
                                                         (uint8_t *)buffer,
                                                         (uint16_t)size,
                                                         timeout);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

bool osg_board_I2c_masterReceiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return false;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_RECEIVE | OSG_I2C_FLAG_IS_RECEIVE;

    const HAL_StatusTypeDef status = HAL_I2C_Master_Receive_IT(handler,
                                                            (slaveAddress << 1),
                                                            (uint8_t *)buffer,
                                                            (uint16_t)size);

    const bool ret = osg_bool(status == HAL_OK);
    if (ret == false) i2c->flags = restore;

    return ret;
}

Size osg_board_I2c_masterReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (osg_CircularFifo_isNull(&i2c->rxFifo) == true)
        return false;

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
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Slave_Transmit(handler,
                                                         (uint8_t *)buffer,
                                                         (uint16_t)size,
                                                         timeout);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

bool osg_board_I2c_slaveSendNonBlocking(osg_I2c * self, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_SEND | OSG_I2C_FLAG_IS_SEND;

    const HAL_StatusTypeDef status = HAL_I2C_Slave_Transmit_IT(handler,
                                                               (uint8_t *)buffer,
                                                               (uint16_t)size);

    const bool ret = osg_bool(status == HAL_OK);
    if (ret == false) i2c->flags = restore;

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
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Slave_Receive(handler,
                                                        (uint8_t *)buffer,
                                                        (uint16_t)size,
                                                        timeout);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

bool osg_board_I2c_slaveReceiveNonBlocking(osg_I2c * self, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return false;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_RECEIVE | OSG_I2C_FLAG_IS_RECEIVE;

    const HAL_StatusTypeDef status = HAL_I2C_Slave_Receive_IT(handler,
                                                              (uint8_t *)buffer,
                                                              (uint16_t)size);

    const bool ret = osg_bool(status == HAL_OK);
    if (ret == false) i2c->flags = restore;

    return ret;
}

Size osg_board_I2c_slaveReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (osg_CircularFifo_isNull(&i2c->rxFifo) == true)
        return false;

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
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *) i2c->handler;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED)
    {
        if (self->mode == OSG_I2C_MASTER_MODE)
        {
            const HAL_StatusTypeDef ret = HAL_I2C_Master_Abort_IT(handler, (i2c->slaveAddress << 1));
            if (ret == HAL_OK)
            {
                while (!(i2c->flags & OSG_I2C_FLAG_RX_ABORTED))
                {
                    // waiting for abort
                }

                i2c->flags &= ~OSG_I2C_FLAG_IS_RECEIVE;
                i2c->flags &= ~OSG_I2C_FLAG_RX_BUFF_ENABLED;
                i2c->flags &= ~OSG_I2C_FLAG_RX_ABORTED;
            }
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
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(handler,
                                                    (slaveAddress << 1),
                                                    memAddress,
                                                    memAddrSize,
                                                    (uint8_t *)buffer,
                                                    (uint16_t)size,
                                                    timeout);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == HAL_OK);
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
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(handler,
                                                   (slaveAddress << 1),
                                                   memAddress,
                                                   memAddrSize,
                                                   (uint8_t *)buffer,
                                                   (uint16_t)size,
                                                   timeout);

#if I2C_MUTEX_ENABLED
    const bool rel = osg_Mutex_release(mutex);
    osg_assert(rel == true, "ASSERT FAIL: Impossible to release I2C mutex.");
#endif

    return osg_bool(ret == HAL_OK);
}

uint8_t osg_board_I2c_countI2cs()
{
    return _osg_board_I2c_getI2cs();
}
