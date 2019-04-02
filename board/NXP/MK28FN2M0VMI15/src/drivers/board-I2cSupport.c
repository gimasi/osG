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
#include "board-I2cSupport.h"

#define TOT_I2C FSL_FEATURE_SOC_I2C_COUNT

static osg_I2cImpl _osg_board_I2c_devices[TOT_I2C];
static osg_board_I2cHandler _osg_board_I2c_handlers[TOT_I2C];
static osg_board_I2cSlaveData nbXfer;
#if I2C_MUTEX_ENABLED
    static osg_Mutex osg_rtos_I2c_mutexes[TOT_I2C];
    static osg_rtos_Mutex osg_rtos_I2c_mutexesId[TOT_I2C];
#endif


// ///////////////////////////////////////////////////////
// SUPPORT TO INTERNAL DATA STRUCTURES
// ///////////////////////////////////////////////////////

uint8_t _osg_board_I2c_getI2cs(void)
{
    return (uint8_t)TOT_I2C;
}

uint8_t _osg_board_I2c_getDeviceIndex(const osg_I2cId id)
{
    uint8_t index = 0;

#ifdef I2C0
    if (id == OSG_I2C0)
        return index;
    index++;
#endif
#ifdef I2C1
    if (id == OSG_I2C1)
        return index;
    index++;
#endif
#ifdef I2C2
    if (id == OSG_I2C2)
        return index;
    index++;
#endif
#ifdef I2C3
    if (id == OSG_I2C3)
        return index;
    index++;
#endif
#ifdef I2C4
    if (id == OSG_I2C4)
        return index;
    index++;
#endif

    return TOT_I2C;
}

osg_I2cImpl * _osg_board_I2c_getI2cByInstance(void * handler)
{
    for (int i = 0; i < TOT_I2C; ++i)
    {
        if (_osg_board_I2c_devices[i].handler == handler) return &_osg_board_I2c_devices[i];
    }

    return NULL;
}

osg_I2cImpl * _osg_board_I2c_getI2cById(const osg_I2cId id)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(id);
    return &_osg_board_I2c_devices[index];
}

osg_board_I2cInstance * _osg_board_I2c_getI2cInstance(const osg_I2cId id)
{
    switch (id)
    {
#ifdef I2C0
        case OSG_I2C0:
            return I2C0;
#endif
#ifdef I2C1
        case OSG_I2C1:
            return I2C1;
#endif
#ifdef I2C2
        case OSG_I2C2:
            return I2C2;
#endif
#ifdef I2C3
        case OSG_I2C3:
            return I2C3;
#endif
#ifdef I2C4
        case OSG_I2C4:
            return I2C4;
#endif
        default:
            osg_error("ERROR: Invalid I2C port");
    }

    return NULL;
}

osg_board_I2cHandler * _osg_board_I2c_getI2cHandler(const osg_I2cId id)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(id);
    return &_osg_board_I2c_handlers[index];
}

osg_board_I2cHandler * _osg_board_I2c_getI2cHandlerByInstance(void * handler)
{
    for (int i = 0; i < TOT_I2C; ++i)
    {
        if (_osg_board_I2c_devices[i].handler == handler) return &_osg_board_I2c_handlers[i];
    }

    return NULL;
}

osg_board_I2cSlaveData * _osg_board_I2c_getNonBlockingTransferSlaveData(void)
{
    return &nbXfer;
}

#if I2C_MUTEX_ENABLED
void _osg_board_I2c_configureMutex(const osg_I2cId id)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(id);
    osg_rtos_I2c_mutexes[index].handler = &osg_rtos_I2c_mutexesId[index];
}

osg_Mutex * _osg_board_I2c_getMutex(const osg_I2cId id)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(id);
    return &osg_rtos_I2c_mutexes[index];
}
#endif


// ///////////////////////////////////////////////////////
// CHECK BOARD PROPERTY
// ///////////////////////////////////////////////////////

bool _osg_board_I2c_checkI2c(const osg_I2cId i2c)
{
    switch (i2c)
    {
#ifdef I2C0
        case OSG_I2C0:
#endif
#ifdef I2C1
        case OSG_I2C1:
#endif
#ifdef I2C2
        case OSG_I2C2:
#endif
#ifdef I2C3
        case OSG_I2C3:
#endif
#ifdef I2C4
        case OSG_I2C4:
#endif
            return true;
        default:
            return false;
    }
}

bool _osg_board_I2c_validatePins(const osg_I2cId i2cId, const osg_GpioId sda, const osg_GpioId scl)
{
    switch (i2cId)
    {
#ifdef I2C0
        case OSG_I2C0:
        {
            if (sda == OSG_GPIO_PE18
                || sda == OSG_GPIO_PA21
                || sda == OSG_GPIO_PB1
                || sda == OSG_GPIO_PB3
                || sda == OSG_GPIO_PD3
                || sda == OSG_GPIO_PD9)
            {
                if (scl == OSG_GPIO_PE19
                    || scl == OSG_GPIO_PA20
                    || scl == OSG_GPIO_PB0
                    || scl == OSG_GPIO_PB2
                    || scl == OSG_GPIO_PD2
                    || scl == OSG_GPIO_PD8)
                {
                    return true;
                }
            }

            break;
        }
#endif
#ifdef I2C1
        case OSG_I2C1:
        {
            if (sda == OSG_GPIO_PE0
                || sda == OSG_GPIO_PA9
                || sda == OSG_GPIO_PC11)
            {
                if (scl == OSG_GPIO_PE1
                    || scl == OSG_GPIO_PA8
                    || scl == OSG_GPIO_PC10)
                {
                    return true;
                }
            }

            break;
        }
#endif
#ifdef I2C2
        case OSG_I2C2:
        {
            if (sda == OSG_GPIO_PE22
                || sda == OSG_GPIO_PA7
                || sda == OSG_GPIO_PA10
                || sda == OSG_GPIO_PB11)
            {
                if (scl == OSG_GPIO_PE23
                    || scl == OSG_GPIO_PA6
                    || scl == OSG_GPIO_PA11
                    || scl == OSG_GPIO_PB10)
                {
                    return true;
                }
            }

            break;
        }
#endif
#ifdef I2C3
        case OSG_I2C3:
        {
            if (sda == OSG_GPIO_PE10
                || sda == OSG_GPIO_PA1
                || sda == OSG_GPIO_PA30
                || sda == OSG_GPIO_PC28)
            {
                if (scl == OSG_GPIO_PE11
                    || scl == OSG_GPIO_PA2
                    || scl == OSG_GPIO_PA31
                    || scl == OSG_GPIO_PC29)
                {
                    return true;
                }
            }

            break;
        }
#endif
        default:
            return false;
    }

    return false;
}


// ///////////////////////////////////////////////////////
// DECODE FROM OSG TO NXP
// ///////////////////////////////////////////////////////

i2c_slave_address_mode_t _osg_board_I2c_decodeAddressingMode(const osg_I2cAddressingMode addrMode)
{
    switch (addrMode)
    {
        case OSG_I2C_7BIT_ADDRESS:
            return kI2C_Address7bit;
        case OSG_I2C_10BIT_ADDRESS:
            return kI2C_RangeMatch;
        default:
            osg_error("ERROR: Invalid addressing mode");
    }

    // suppress warning
    return kI2C_Address7bit;
}

//port_mux_t _osg_board_I2c_decodePinMuxAlt(const osg_GpioId pin)
//{
//    switch (pin)
//    {
//        case OSG_GPIO_PA6:
//        case OSG_GPIO_PA7:
//        case OSG_GPIO_PA8:
//        case OSG_GPIO_PA9:
//        case OSG_GPIO_PA10:
//        case OSG_GPIO_PA11:
//        case OSG_GPIO_PA20:
//        case OSG_GPIO_PA21:
//        case OSG_GPIO_PA30:
//        case OSG_GPIO_PA31:
//        case OSG_GPIO_PB0:
//        case OSG_GPIO_PB1:
//        case OSG_GPIO_PB2:
//        case OSG_GPIO_PB3:
//        case OSG_GPIO_PC10:
//        case OSG_GPIO_PC11:
//        case OSG_GPIO_PC28:
//        case OSG_GPIO_PC29:
//        case OSG_GPIO_PD8:
//        case OSG_GPIO_PD9:
//        case OSG_GPIO_PE10:
//        case OSG_GPIO_PE11:
//        case OSG_GPIO_PE22:
//        case OSG_GPIO_PE23:
//            return kPORT_MuxAlt2;
//        case OSG_GPIO_PA1:
//        case OSG_GPIO_PA2:
//        case OSG_GPIO_PB10:
//        case OSG_GPIO_PB11:
//        case OSG_GPIO_PE18:
//        case OSG_GPIO_PE19:
//            return kPORT_MuxAlt4;
//        case OSG_GPIO_PE0:
//        case OSG_GPIO_PE1:
//            return kPORT_MuxAlt6;
//        case OSG_GPIO_PD2:
//        case OSG_GPIO_PD3:
//            return kPORT_MuxAlt7;
//        default:
//            osg_error("ERROR: PIN can't be used for I2C.");
//    }
//
//    return kPORT_PinDisabledOrAnalog;
//}

uint32_t _osg_board_I2c_getI2cClk(const osg_I2cId id)
{
    switch (id)
    {
#ifdef I2C0_CLK_SRC
        case OSG_I2C0:
            return CLOCK_GetFreq(I2C0_CLK_SRC);
#endif
#ifdef I2C1_CLK_SRC
        case OSG_I2C1:
            return CLOCK_GetFreq(I2C1_CLK_SRC);
#endif
#ifdef I2C2_CLK_SRC
        case OSG_I2C2:
            return CLOCK_GetFreq(I2C2_CLK_SRC);
#endif
#ifdef I2C3_CLK_SRC
        case OSG_I2C3:
            return CLOCK_GetFreq(I2C3_CLK_SRC);
#endif
#ifdef I2C4_CLK_SRC
        case OSG_I2C4:
            return CLOCK_GetFreq(I2C4_CLK_SRC);
#endif
        default:
            osg_error("ERROR: Invalid I2C port");
    }

    return (clock_name_t)0;
}


// ///////////////////////////////////////////////////////
// ENCODE TO OSG
// ///////////////////////////////////////////////////////

//osg_GpioAlternateFunction _osg_board_I2c_encodeAlternateFunction(const osg_I2cId i2cId)
//{
//    switch (i2cId)
//    {
//#ifdef I2C0
//        case OSG_I2C0:
//        {
//            return OSG_GPIO_ALTERNATE_I2C0;
//        }
//#endif
//#ifdef I2C1
//        case OSG_I2C1:
//        {
//            return OSG_GPIO_ALTERNATE_I2C1;
//        }
//#endif
//#ifdef I2C2
//        case OSG_I2C2:
//        {
//            return OSG_GPIO_ALTERNATE_I2C2;
//        }
//#endif
//#ifdef I2C3
//        case OSG_I2C3:
//        {
//            return OSG_GPIO_ALTERNATE_I2C3;
//        }
//#endif
//#ifdef I2C4
//        case OSG_I2C4:
//        {
//            return OSG_GPIO_ALTERNATE_I2C4;
//        }
//#endif
//        default:
//            osg_error("ERROR: Impossible to set correct I2C Alternate Function to Pin.");
//    }
//
//    // suppress warning
//    return OSG_GPIO_ALTERNATE_DEFAULT;
//}


// ///////////////////////////////////////////////////////
// SUPPORT TO TRANSACTION
// ///////////////////////////////////////////////////////

void _osg_board_I2c_doBufferedSend(osg_I2cImpl * i2c, const osg_I2cMode mode)
{
    if (osg_CircularFifo_isEmpty(&i2c->txFifo)) return;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_SEND;
    i2c->flags |= OSG_I2C_FLAG_IS_SEND;
    void * buffer = NULL;
    Size size = 0;
    osg_CircularFifo_getFilledBuffer(&i2c->txFifo, &buffer, &size);

    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandlerByInstance(i2c->handler);
    status_t ok;

    if (mode == OSG_I2C_MASTER_MODE)
    {
        i2c_master_transfer_t masterXfer;
        masterXfer.slaveAddress = i2c->slaveAddress;
        masterXfer.direction = kI2C_Write;
        masterXfer.subaddress = 0;
        masterXfer.subaddressSize = 0;
        masterXfer.data = (void *)buffer;
        masterXfer.dataSize = size;
        masterXfer.flags = kI2C_TransferDefaultFlag;

        I2C_MasterTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                       &handle->masterHandle,
                                       _osg_board_I2c_masterSendCallback,
                                       NULL);

        ok = I2C_MasterTransferNonBlocking((osg_board_I2cInstance *)i2c->handler, &handle->masterHandle, &masterXfer);
    }
    else
    {
        nbXfer.data = buffer;
        nbXfer.dataSize = size;

        I2C_SlaveTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                      &handle->slaveHandle,
                                      _osg_board_I2c_slaveSendRecvCallback,
                                      &nbXfer);
        ok = I2C_SlaveTransferNonBlocking((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle, kI2C_SlaveAllEvents);
    }


    if (ok != kStatus_Success) i2c->flags = restore;
}

void _osg_board_I2c_doBufferedReceive(osg_I2cImpl * i2c, const osg_I2cMode mode)
{
    if (osg_CircularFifo_isNull(&i2c->rxFifo)
            || !(i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED))
        return;
    
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_RECEIVE;
    i2c->flags |= OSG_I2C_FLAG_IS_RECEIVE;

    osg_board_I2cHandler * handle = _osg_board_I2c_getI2cHandlerByInstance(i2c->handler);
    status_t ok;

    if (mode == OSG_I2C_MASTER_MODE)
    {
        i2c_master_transfer_t masterXfer;
        masterXfer.slaveAddress = i2c->slaveAddress;
        masterXfer.direction = kI2C_Read;
        masterXfer.subaddress = 0;
        masterXfer.subaddressSize = 0;
        masterXfer.data = &i2c->rxByte;
        masterXfer.dataSize = 1;
        masterXfer.flags = kI2C_TransferDefaultFlag;

        I2C_MasterTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                       &handle->masterHandle,
                                       _osg_board_I2c_masterRecvCallback,
                                       NULL);
        ok = I2C_MasterTransferNonBlocking((osg_board_I2cInstance *)i2c->handler, &handle->masterHandle, &masterXfer);
    }
    else
    {
        nbXfer.data = &i2c->rxByte;
        nbXfer.dataSize = 1;

        I2C_SlaveTransferCreateHandle((osg_board_I2cInstance *)i2c->handler,
                                      &handle->slaveHandle,
                                      _osg_board_I2c_slaveSendRecvCallback,
                                      &nbXfer);
        ok = I2C_SlaveTransferNonBlocking((osg_board_I2cInstance *)i2c->handler, &handle->slaveHandle, kI2C_SlaveAllEvents);
    }

    if (ok != kStatus_Success) i2c->flags = restore;
}

void _osg_board_I2c_masterBlockingCallback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    int * pState = (int *)userData;
    *pState = status;
}

void _osg_board_I2c_masterSendCallback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByInstance(base);
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        if (i2c->flags & OSG_I2C_FLAG_IS_NB_SEND)
        {
            i2c->flags &= ~OSG_I2C_FLAG_IS_SEND;
            if (i2c->txCompleteNb != NULL) i2c->txCompleteNb();
        }
        else
        {
            osg_CircularFifo_popBytes(&i2c->txFifo, handle->transferSize);
            if (i2c->txCompleteBuffered != NULL) i2c->txCompleteBuffered();
        }
        i2c->flags &= ~OSG_I2C_FLAG_IS_NB_SEND;
        _osg_board_I2c_doBufferedSend(i2c, OSG_I2C_MASTER_MODE);
    }
}

void _osg_board_I2c_masterRecvCallback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByInstance(base);
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        if (i2c->flags & OSG_I2C_FLAG_IS_NB_RECEIVE)
        {
            i2c->flags &= ~OSG_I2C_FLAG_IS_RECEIVE;
            if (i2c->rxCompleteNb != NULL) i2c->rxCompleteNb();
        }
        else
        {
            osg_CircularFifo_pushBuffer(&i2c->rxFifo, &i2c->rxByte, handle->transferSize);
            if (i2c->rxCompleteBuffered != NULL) i2c->rxCompleteBuffered();
        }
        i2c->flags &= ~OSG_I2C_FLAG_IS_NB_RECEIVE;
        _osg_board_I2c_doBufferedReceive(i2c, OSG_I2C_MASTER_MODE);
    }
}

void _osg_board_I2c_slaveBlockingCallback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
    osg_board_I2cSlaveData * transfer = (osg_board_I2cSlaveData *)userData;
    
    switch (xfer->event)
    {
        case kI2C_SlaveAddressMatchEvent:
            xfer->data = transfer->data;
            xfer->dataSize = transfer->dataSize;
            break;
        
        case kI2C_SlaveTransmitEvent:
            break;

        case kI2C_SlaveReceiveEvent:
            break;
        
        case kI2C_SlaveCompletionEvent:
            xfer->data = NULL;
            xfer->dataSize = 0;
            transfer->state = xfer->completionStatus;
            break;
        
        default:
            break;
    }   
}

void _osg_board_I2c_slaveSendRecvCallback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByInstance(base);
    osg_board_I2cSlaveData * transfer = (osg_board_I2cSlaveData *)userData;
    
    switch (xfer->event)
    {
        case kI2C_SlaveAddressMatchEvent:
            xfer->data = transfer->data;
            xfer->dataSize = transfer->dataSize;
            break;

        case kI2C_SlaveReceiveEvent:
            break;
        
        case kI2C_SlaveTransmitEvent:
            break;
        
        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            transfer->state = xfer->completionStatus;

            if (i2c->flags & OSG_I2C_FLAG_IS_RECEIVE)
            {
                if (xfer->completionStatus == kStatus_Success)
                {
                    if (i2c->flags & OSG_I2C_FLAG_IS_NB_RECEIVE)
                    {
                        i2c->flags &= ~OSG_I2C_FLAG_IS_RECEIVE;
                        if (i2c->rxCompleteNb != NULL) i2c->rxCompleteNb();
                    }
                    else
                    {
                        osg_CircularFifo_pushBuffer(&i2c->rxFifo, transfer->data, transfer->dataSize);
                        if (i2c->rxCompleteBuffered != NULL) i2c->rxCompleteBuffered();
                    }
                    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_RECEIVE;
                    _osg_board_I2c_doBufferedReceive(i2c, OSG_I2C_SLAVE_MODE);
                }
                else
                {
                    osg_error("ERROR: I2C Communication error.");
                }
            }
            
            if (i2c->flags & OSG_I2C_FLAG_IS_SEND)
            {
                xfer->data = NULL;
                xfer->dataSize = 0;
                if (xfer->completionStatus == kStatus_Success)
                {
                    if (i2c->flags & OSG_I2C_FLAG_IS_NB_SEND)
                    {
                        i2c->flags &= ~OSG_I2C_FLAG_IS_SEND;
                        if (i2c->txCompleteNb != NULL) i2c->txCompleteNb();
                    }
                    else
                    {
                        osg_CircularFifo_popBytes(&i2c->txFifo, xfer->transferredCount);
                        if (i2c->txCompleteBuffered != NULL) i2c->txCompleteBuffered();
                    }
                    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_SEND;
                    _osg_board_I2c_doBufferedSend(i2c, OSG_I2C_SLAVE_MODE);
                }
                else
                {
                    osg_error("ERROR: I2C Communication error.");
                }
            }
            break;

        default:
            break;
    }
}

void _osg_board_I2c_timeoutCallback(void * argument)
{
    int * pState = (int *)argument;
    *pState = kStatus_I2C_Timeout;
}
