/**
 * @file i2c_behavior.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief 
 * @version 1.0
 * @date 2022-11-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "i2c_behavior.h"
#include "Sensor.h"
#include "i2c.h"

// I2C Pins
// RX = SDA   TX = SCL
uint8_t i2cDataRx[8];
uint8_t i2cDataTx[8];

/**
 * @brief i2c slave mode receive done callback
 * 
 * @param hi2c 
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef state = HAL_ERROR;

    i2cDataTx[0] = i2cDataRx[0];
    switch (i2cDataRx[0]) {

        case 0xee:
            i2cDataTx[1] = Sensor_TransData_Digital;
            break;
        default:
            break;
    }

    do {
        state = HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t *)i2cDataTx, 2, 1000);
    } while (state != HAL_OK);
    //        state = HAL_I2C_Slave_Transmit_DMA(&hi2c1, (uint8_t*) i2cDataTx, 5);

    do {
        state = HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)i2cDataRx, 2);
    } while (state != HAL_OK);
}

/**
 * @brief i2c initialization function
 * 
 * @param __id 
 */
void Sensor_I2C_Init(uint8_t __id)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00300B29;
    hi2c1.Init.OwnAddress1 = (__id << 1);
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        Error_Handler();
    }

    /** I2C Fast mode Plus enable
     */
    HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
}

/**
 * @brief reinitializes the I2C
 * 
 * @param I2cHandle 
 */
void I2C_Reset(I2C_HandleTypeDef *I2cHandle)
{
    HAL_StatusTypeDef state = HAL_ERROR;
    HAL_I2C_DeInit(I2cHandle);
    I2cHandle->State = HAL_I2C_STATE_RESET;
    if (I2cHandle == &hi2c1) {
        Sensor_I2C_Init(__sensor_id);
        do {
            state = HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)i2cDataRx, 2);
        } while (state != HAL_OK);
    }
}

/**
 * @brief i2c error callback
 * 
 * @param I2cHandle 
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
    if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF) {
        I2C_Reset(I2cHandle);
    }
}