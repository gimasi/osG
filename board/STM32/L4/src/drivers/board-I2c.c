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
#include "board/drivers/board-I2c.h"
#include <osg.h>
#include <stm32l4xx_hal.h>

typedef I2C_HandleTypeDef osg_board_I2cHandler;
typedef I2C_TypeDef osg_board_I2cInstance;

#ifdef I2C0
#define COUNT_I2C0 1
#else
#define COUNT_I2C0 0
#endif
#ifdef I2C1
#define COUNT_I2C1 1
#else
#define COUNT_I2C1 0
#endif
#ifdef I2C2
#define COUNT_I2C2 1
#else
#define COUNT_I2C2 0
#endif
#ifdef I2C3
#define COUNT_I2C3 1
#else
#define COUNT_I2C3 0
#endif
#ifdef I2C4
#define COUNT_I2C4 1
#else
#define COUNT_I2C4 0
#endif

#define TOT_I2C (COUNT_I2C0 + COUNT_I2C1 + COUNT_I2C2 + COUNT_I2C3 + COUNT_I2C4)

static uint8_t _osg_board_I2c_getI2cs()
{
    return (uint8_t)TOT_I2C;
}

static osg_I2cImpl _osg_board_I2c_devices[TOT_I2C];
static osg_board_I2cHandler _osg_board_I2c_handlers[TOT_I2C];

static osg_I2cImpl * _osg_board_I2c_getI2cByHandler(void * handler)
{
    for (int i = 0; i < TOT_I2C; ++i)
    {
        if (_osg_board_I2c_devices[i].handler == handler) return &_osg_board_I2c_devices[i];
    }

    return NULL;
}

Bool _osg_board_I2c_checkI2c(const osg_I2cId i2c)
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
            return TRUE;
        default:
            return FALSE;
    }
}

static osg_board_I2cInstance * _osg_board_I2c_getI2cInstance(const osg_I2cId id)
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

static uint8_t _osg_board_I2c_getDeviceIndex(const osg_I2cId id)
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

static osg_I2cImpl * _osg_board_I2c_getI2cById(const osg_I2cId id)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(id);
    return &_osg_board_I2c_devices[index];
}

static osg_board_I2cHandler * _osg_board_I2c_getI2cHandler(const osg_I2cId id)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(id);
    return &_osg_board_I2c_handlers[index];
}

static void _osg_board_I2c_doBufferedSend(osg_I2cImpl * i2c, const osg_I2cMode mode)
{
    if (osg_CircularFifo_isEmpty(&i2c->txFifo)) return;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_SEND;
    i2c->flags |= OSG_I2C_FLAG_IS_SEND;
    void * buffer = NULL;
    Size size = 0;
    osg_CircularFifo_getFilledBuffer(&i2c->txFifo, &buffer, &size);

    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;
    HAL_StatusTypeDef ok;
    if (mode == OSG_I2C_MASTER_MODE)
        ok = HAL_I2C_Master_Transmit_IT(handler, i2c->slaveAddress, (uint8_t *)buffer, (uint16_t)size);
    else
        ok = HAL_I2C_Slave_Transmit_IT(handler, (uint8_t *)buffer, (uint16_t)size);


    if (ok != HAL_OK) i2c->flags = restore;
}

static void _osg_board_I2c_doBufferedReceive(osg_I2cImpl * i2c, const osg_I2cMode mode)
{
    if (osg_CircularFifo_isNull(&i2c->rxFifo)
            || !(i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED))
        return;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_RECEIVE;
    i2c->flags |= OSG_I2C_FLAG_IS_RECEIVE;

    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;
    HAL_StatusTypeDef ok;
    if (mode == OSG_I2C_MASTER_MODE)
        ok = HAL_I2C_Master_Receive_IT(handler, i2c->slaveAddress, &i2c->rxByte, 1);
    else
        ok = HAL_I2C_Slave_Receive_IT(handler, &i2c->rxByte, 1);

    if (ok != HAL_OK) i2c->flags = restore;
}

static HAL_I2C_ModeTypeDef _osg_board_I2c_decodeMode(const osg_I2cMode mode)
{
    switch (mode)
    {
        case OSG_I2C_MASTER_MODE:
            return HAL_I2C_MODE_MASTER;
        case OSG_I2C_SLAVE_MODE:
            return HAL_I2C_MODE_SLAVE;
        default:
            osg_error("ERROR: Invalid I2C mode.");
    }

    return HAL_I2C_MODE_NONE;
}

static uint32_t _osg_board_I2c_decodeAddressingMode(const osg_I2cAddressingMode addrMode)
{
    switch (addrMode)
    {
        case OSG_I2C_7BIT_ADDRESS:
            return I2C_ADDRESSINGMODE_7BIT;
        case OSG_I2C_10BIT_ADDRESS:
            return I2C_ADDRESSINGMODE_10BIT;
        default:
            osg_error("ERROR: Invalid addressing mode");
    }

    // suppress warning
    return 0;
}

static void _osg_board_I2c_enableI2cClk(const osg_I2cId id)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    switch(id)
    {
#ifdef I2C0
        case OSG_I2C0:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C0;
            PeriphClkInit.I2c1ClockSelection = RCC_I2C0CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_I2C0_CLK_ENABLE();
            break;
        }
#endif
#ifdef I2C1
        case OSG_I2C1:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
            PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_I2C1_CLK_ENABLE();
            break;
        }
#endif
#ifdef I2C2
        case OSG_I2C2:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
            PeriphClkInit.I2c1ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_I2C2_CLK_ENABLE();
            break;
        }
#endif
#ifdef I2C3
        case OSG_I2C3:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C3;
            PeriphClkInit.I2c1ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_I2C3_CLK_ENABLE();
            break;
        }
#endif
#ifdef I2C4
        case OSG_I2C4:
        {
            PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C4;
            PeriphClkInit.I2c1ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
            if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            {
                // Initialization Error
                osg_error("ERROR: Initialization Error.");
            }
            __HAL_RCC_I2C4_CLK_ENABLE();
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid I2C Port in I2C initialization.");
            break;
    }
}

static void _osg_board_I2c_enableI2cInterrupt(const osg_I2cId id)
{
    switch(id)
    {
#ifdef I2C0
        case OSG_I2C0:
        {
            HAL_NVIC_SetPriority(I2C0_EV_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C0_EV_IRQn);
            HAL_NVIC_SetPriority(I2C0_ER_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C0_ER_IRQn);
            break;
        }
#endif
#ifdef I2C1
        case OSG_I2C1:
        {
            HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
            break;
        }
#endif
#ifdef I2C2
        case OSG_I2C2:
        {
            HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
            HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
            break;
        }
#endif
#ifdef I2C3
        case OSG_I2C3:
        {
            HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
            HAL_NVIC_SetPriority(I2C3_ER_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
            break;
        }
#endif
#ifdef I2C4
        case OSG_I2C4:
        {
            HAL_NVIC_SetPriority(I2C4_EV_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C4_EV_IRQn);
            HAL_NVIC_SetPriority(I2C4_ER_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(I2C4_ER_IRQn);
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid I2C Port in I2C initialization.");
            break;
    }
}

static void _osg_board_I2c_resetI2c(const osg_I2cId id)
{
    switch(id)
    {
#ifdef I2C0
        case OSG_I2C0:
        {
            __HAL_RCC_I2C0_FORCE_RESET();
            __HAL_RCC_I2C0_RELEASE_RESET();
            __HAL_RCC_I2C0_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(I2C0_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C0_ER_IRQn);
            break;
        }
#endif
#ifdef I2C1
        case OSG_I2C1:
        {
            __HAL_RCC_I2C1_FORCE_RESET();
            __HAL_RCC_I2C1_RELEASE_RESET();
            __HAL_RCC_I2C1_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
            break;
        }
#endif
#ifdef I2C2
        case OSG_I2C2:
        {
            __HAL_RCC_I2C2_FORCE_RESET();
            __HAL_RCC_I2C2_RELEASE_RESET();
            __HAL_RCC_I2C2_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
            break;
        }
#endif
#ifdef I2C3
        case OSG_I2C3:
        {
            __HAL_RCC_I2C3_FORCE_RESET();
            __HAL_RCC_I2C3_RELEASE_RESET();
            __HAL_RCC_I2C3_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
            break;
        }
#endif
#ifdef I2C4
        case OSG_I2C4:
        {
            __HAL_RCC_I2C4_FORCE_RESET();
            __HAL_RCC_I2C4_RELEASE_RESET();
            __HAL_RCC_I2C4_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(I2C4_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C4_ER_IRQn);
            break;
        }
#endif
        default:
            osg_error("ERROR: Invalid I2C Port in I2C reset.");
    }
}

static Bool _osg_board_I2c_validatePins(const osg_I2cId i2cId, const osg_GpioId sda, const osg_GpioId scl)
{
    switch (i2cId)
    {
#ifdef I2C1
        case OSG_I2C1:
        {
            if (sda == OSG_GPIO_PB7
                || sda == OSG_GPIO_PB9)
            {
                if (scl == OSG_GPIO_PB6
                    || scl == OSG_GPIO_PB8)
                {
                    return TRUE;
                }
            }

            break;
        }
#endif
#ifdef I2C2
        case OSG_I2C2:
        {
            if (sda == OSG_GPIO_PB11
                || sda == OSG_GPIO_PB14)
            {
                if (scl == OSG_GPIO_PB10
                    || scl == OSG_GPIO_PB13)
                {
                    return TRUE;
                }
            }

            break;
        }
#endif
#ifdef I2C3
        case OSG_I2C3:
        {
            if (sda == OSG_GPIO_PC1)
            {
                if (scl == OSG_GPIO_PC0)
                {
                    return TRUE;
                }
            }

            break;
        }
#endif
        default:
            return FALSE;
    }

    return FALSE;
}

osg_GpioAlternateFunction _osg_board_I2c_getGpioAlternateFunction(const osg_I2cId i2cId)
{
    switch (i2cId)
    {
#ifdef I2C0
        case OSG_I2C0:
        {
            return OSG_GPIO_ALTERNATE_I2C0;
        }
#endif
#ifdef I2C1
        case OSG_I2C1:
        {
            return OSG_GPIO_ALTERNATE_I2C1;
        }
#endif
#ifdef I2C2
        case OSG_I2C2:
        {
            return OSG_GPIO_ALTERNATE_I2C2;
        }
#endif
#ifdef I2C3
        case OSG_I2C3:
        {
            return OSG_GPIO_ALTERNATE_I2C3;
        }
#endif
#ifdef I2C4
        case OSG_I2C4:
        {
            return OSG_GPIO_ALTERNATE_I2C4;
        }
#endif
        default:
            osg_error("ERROR: Impossible to set correct I2C Alternate Function to Pin.");
    }

    // suppress warning
    return OSG_GPIO_ALTERNATE_DEFAULT;
}

void osg_board_I2c_ctor(osg_I2c * self, const uint32_t clockSpeed, const osg_I2cAddressingMode addressingMode, const Bool enableGeneralCall, const uint16_t ownAddress, void * circularTxBuffer, Size txBufferSize, void * circularRxBuffer, Size rxBufferSize)
{
    if (_osg_board_I2c_checkI2c(self->id) == FALSE)
    {
        osg_error("ERROR: I2C port NOT available on this device!");
    }

    if (_osg_board_I2c_validatePins(self->id, self->sdaPin.id, self->sclPin.id) == FALSE)
        osg_error("ERROR: Impossible to use these PINs for this I2C port on this board!");

    _osg_board_I2c_enableI2cClk(self->id);
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    i2c->handler = _osg_board_I2c_getI2cHandler(self->id);

    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;
    handler->Instance = _osg_board_I2c_getI2cInstance(self->id);

    handler->Init.Timing = clockSpeed;
    handler->Init.AddressingMode = _osg_board_I2c_decodeAddressingMode(addressingMode);
    handler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    handler->Init.GeneralCallMode = enableGeneralCall == TRUE ? I2C_GENERALCALL_ENABLE : I2C_GENERALCALL_DISABLE;
    handler->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    handler->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    handler->Init.OwnAddress1 = ownAddress;
    handler->Init.OwnAddress2 = 0;

    osg_CircularFifo_ctor(&i2c->rxFifo, circularRxBuffer, rxBufferSize, OSG_FIFO_OVERWRITE_NO_ERROR);
    osg_CircularFifo_ctor(&i2c->txFifo, circularTxBuffer, txBufferSize, OSG_FIFO_ERROR);
    i2c->flags = OSG_I2C_FLAG_DEFAULT;

    __HAL_I2C_ENABLE(handler);

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
}

void osg_board_I2c_dtor(osg_I2c * self)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;
    HAL_I2C_DeInit(handler);
    _osg_board_I2c_resetI2c(self->id);

    i2c->txCompleteNb = NULL;
    i2c->rxCompleteNb = NULL;
    i2c->txCompleteBuffered = NULL;
    i2c->rxCompleteBuffered = NULL;
    osg_CircularFifo_dtor(&i2c->txFifo);
    osg_CircularFifo_dtor(&i2c->rxFifo);
    i2c->handler = NULL;
}

Bool osg_board_I2c_masterSendBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size, const uint32_t timeout)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(handler,
                                                          slaveAddress,
                                                          (uint8_t *)buffer,
                                                          (uint16_t)size,
                                                          timeout);

    return ret == HAL_OK ? TRUE : FALSE;
}

Bool osg_board_I2c_masterSendNonBlocking(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_SEND | OSG_I2C_FLAG_IS_SEND;

    const HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(handler,
                                                                slaveAddress,
                                                                (uint8_t *)buffer,
                                                                (uint16_t)size);

    const Bool ret = status == HAL_OK ? TRUE : FALSE;
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

Bool osg_board_I2c_masterSendBuffered(osg_I2c * self, const uint16_t slaveAddress, const void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    if (osg_CircularFifo_isNull(&i2c->txFifo) == TRUE)
        return FALSE;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&i2c->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return FALSE;

    i2c->slaveAddress = slaveAddress;
    _osg_board_I2c_doBufferedSend(i2c, self->mode);

    return TRUE;
}

Bool osg_board_I2c_masterReceiveBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size, const uint32_t timeout)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return FALSE;
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(handler,
                                                         slaveAddress,
                                                         (uint8_t *)buffer,
                                                         (uint16_t)size,
                                                         timeout);

    return ret == HAL_OK ? TRUE : FALSE;
}

Bool osg_board_I2c_masterReceiveNonBlocking(osg_I2c * self, const uint16_t slaveAddress, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return FALSE;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_RECEIVE | OSG_I2C_FLAG_IS_RECEIVE;

    const HAL_StatusTypeDef status = HAL_I2C_Master_Receive_IT(handler,
                                                            slaveAddress,
                                                            (uint8_t *)buffer,
                                                            (uint16_t)size);

    const Bool ret = status == HAL_OK ? TRUE : FALSE;
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

Size osg_board_I2c_masterReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    return osg_CircularFifo_popBuffer(&i2c->rxFifo, buffer, bufferSize);
}

Bool osg_board_I2c_slaveSendBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Slave_Transmit(handler,
                                                         (uint8_t *)buffer,
                                                         (uint16_t)size,
                                                         timeout);

    return ret == HAL_OK ? TRUE : FALSE;
}

Bool osg_board_I2c_slaveSendNonBlocking(osg_I2c * self, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_SEND | OSG_I2C_FLAG_IS_SEND;

    const HAL_StatusTypeDef status = HAL_I2C_Slave_Transmit_IT(handler,
                                                               (uint8_t *)buffer,
                                                               (uint16_t)size);

    const Bool ret = status == HAL_OK ? TRUE : FALSE;
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

Bool osg_board_I2c_slaveSendBuffered(osg_I2c * self, const void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);

    if (osg_CircularFifo_isNull(&i2c->txFifo) == TRUE)
        return FALSE;

    Size bytesWrote = osg_CircularFifo_pushBuffer(&i2c->txFifo, buffer, bufferSize);
    if (bytesWrote == 0)
        return FALSE;

    _osg_board_I2c_doBufferedSend(i2c, self->mode);

    return TRUE;
}

Bool osg_board_I2c_slaveReceiveBlocking(osg_I2c * self, void * buffer, const size_t size, const uint32_t timeout)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return FALSE;
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    const HAL_StatusTypeDef ret = HAL_I2C_Slave_Receive(handler,
                                                        (uint8_t *)buffer,
                                                        (uint16_t)size,
                                                        timeout);

    return ret == HAL_OK ? TRUE : FALSE;
}

Bool osg_board_I2c_slaveReceiveNonBlocking(osg_I2c * self, void * buffer, const size_t size)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *)i2c->handler;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED) return FALSE;
    const osg_I2cFlags restore = i2c->flags;
    i2c->flags |= OSG_I2C_FLAG_IS_NB_RECEIVE | OSG_I2C_FLAG_IS_RECEIVE;

    const HAL_StatusTypeDef status = HAL_I2C_Slave_Receive_IT(handler,
                                                              (uint8_t *)buffer,
                                                              (uint16_t)size);

    const Bool ret = status == HAL_OK ? TRUE : FALSE;
    if (!ret)
    {
        i2c->flags = restore;
    }

    return ret;
}

Size osg_board_I2c_slaveReceiveBuffered(osg_I2c * self, void * buffer, const Size bufferSize)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    return osg_CircularFifo_popBuffer(&i2c->rxFifo, buffer, bufferSize);
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

void osg_board_I2c_startReceiveBuffered(osg_I2c * self, const uint16_t slaveAddress)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_assert(osg_CircularFifo_isNull(&i2c->rxFifo) == FALSE, "ASSERT FAILED: Tx Buffer is NULL");
    
    i2c->flags |= OSG_I2C_FLAG_RX_BUFF_ENABLED | OSG_I2C_FLAG_IS_RECEIVE;
    if (self->mode == OSG_I2C_MASTER_MODE)
        i2c->slaveAddress = slaveAddress;
    _osg_board_I2c_doBufferedReceive(i2c, self->mode);
}

void osg_board_I2c_stopReceiveBuffered(osg_I2c * self)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cById(self->id);
    osg_board_I2cHandler * handler = (osg_board_I2cHandler *) i2c->handler;

    if (i2c->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED)
    {
        if (self->mode == OSG_I2C_MASTER_MODE)
        {
            const HAL_StatusTypeDef ret = HAL_I2C_Master_Abort_IT(handler, i2c->slaveAddress);
            if (ret == HAL_OK)
            {
                while (!(i2c->flags & OSG_I2C_FLAG_RX_ABORTED))
                {
                    // waiting for abort
                }

                i2c->flags &= ~OSG_I2C_FLAG_IS_RECEIVE;
                i2c->flags &= ~OSG_I2C_FLAG_RX_BUFF_ENABLED;
            }
        }
    }
    osg_CircularFifo_clear(&i2c->rxFifo);
}

Bool osg_board_I2c_isReceiveBufferedEnabled(osg_I2c * self)
{
    if (_osg_board_I2c_getI2cById(self->id)->flags & OSG_I2C_FLAG_RX_BUFF_ENABLED)
        return TRUE;

    return FALSE;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByHandler(hi2c);
    if (i2c->flags & OSG_I2C_FLAG_IS_NB_SEND)
    {
        if (i2c->txCompleteNb != NULL) i2c->txCompleteNb();
    }
    else
    {
        osg_board_I2cHandler * handler = (osg_board_I2cHandler *) i2c->handler;
        osg_CircularFifo_popBytes(&i2c->txFifo, handler->XferSize - handler->XferCount);
        if (i2c->txCompleteBuffered != NULL) i2c->txCompleteBuffered();
    }
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_SEND;
    _osg_board_I2c_doBufferedSend(i2c, OSG_I2C_MASTER_MODE);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByHandler(hi2c);
    if (i2c->flags & OSG_I2C_FLAG_IS_NB_RECEIVE)
    {
        if (i2c->rxCompleteNb != NULL) i2c->rxCompleteNb();
    }
    else
    {
        osg_board_I2cHandler * handler = (osg_board_I2cHandler *) i2c->handler;
        osg_CircularFifo_pushBuffer(&i2c->rxFifo, &i2c->rxByte, 1);
        if (i2c->rxCompleteBuffered != NULL) i2c->rxCompleteBuffered();
    }
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_RECEIVE;
    _osg_board_I2c_doBufferedReceive(i2c, OSG_I2C_MASTER_MODE);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByHandler(hi2c);
    if (i2c->flags & OSG_I2C_FLAG_IS_NB_SEND)
    {
        if (i2c->txCompleteNb != NULL) i2c->txCompleteNb();
    }
    else
    {
        osg_board_I2cHandler * handler = (osg_board_I2cHandler *) i2c->handler;
        osg_CircularFifo_popBytes(&i2c->txFifo, handler->XferSize - handler->XferCount);
        if (i2c->txCompleteBuffered != NULL) i2c->txCompleteBuffered();
    }
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_SEND;
    _osg_board_I2c_doBufferedSend(i2c, OSG_I2C_SLAVE_MODE);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByHandler(hi2c);
    if (i2c->flags & OSG_I2C_FLAG_IS_NB_RECEIVE)
    {
        if (i2c->rxCompleteNb != NULL) i2c->rxCompleteNb();
    }
    else
    {
        osg_board_I2cHandler * handler = (osg_board_I2cHandler *) i2c->handler;
        osg_CircularFifo_pushBuffer(&i2c->rxFifo, &i2c->rxByte, 1);
        if (i2c->rxCompleteBuffered != NULL) i2c->rxCompleteBuffered();
    }
    i2c->flags &= ~OSG_I2C_FLAG_IS_NB_RECEIVE;
    _osg_board_I2c_doBufferedReceive(i2c, OSG_I2C_SLAVE_MODE);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    osg_error("ERROR: I2C communication error.");
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
    osg_I2cImpl * i2c = _osg_board_I2c_getI2cByHandler(hi2c);
    i2c->flags |= OSG_I2C_FLAG_RX_ABORTED;
}

#ifdef I2C0
void I2C0_EV_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C0);
    HAL_I2C_EV_IRQHandler(&_osg_board_I2c_handlers[index]);
}
void I2C0_ER_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C0);
    HAL_I2C_ER_IRQHandler(&_osg_board_I2c_handlers[index])
}
#endif
#ifdef I2C1
void I2C1_EV_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C1);
    HAL_I2C_EV_IRQHandler(&_osg_board_I2c_handlers[index]);
}
void I2C1_ER_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C1);
    HAL_I2C_ER_IRQHandler(&_osg_board_I2c_handlers[index]);
}
#endif
#ifdef I2C2
void I2C2_EV_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C2);
    HAL_I2C_EV_IRQHandler(&_osg_board_I2c_handlers[index]);
}
void I2C2_ER_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C2);
    HAL_I2C_ER_IRQHandler(&_osg_board_I2c_handlers[index]);
}
#endif
#ifdef I2C3
void I2C3_EV_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C3);
    HAL_I2C_EV_IRQHandler(&_osg_board_I2c_handlers[index]);
}
void I2C3_ER_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C3);
    HAL_I2C_ER_IRQHandler(&_osg_board_I2c_handlers[index]);
}
#endif
#ifdef I2C4
void I2C4_EV_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C4);
    HAL_I2C_EV_IRQHandler(&_osg_board_I2c_handlers[index]);
}
void I2C4_ER_IRQHandler(void)
{
    uint8_t index = _osg_board_I2c_getDeviceIndex(OSG_I2C4);
    HAL_I2C_ER_IRQHandler(&_osg_board_I2c_handlers[index]);
}
#endif
