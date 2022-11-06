/**
 * @file GS_Sensor.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-11-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "GS_Sensor.h"
#include "string.h"

/**
 * @brief GS_Sensor initialization function
 *
 * @param sensor
 * @param sensor_id
 */
void GS_Sensor_Init(GS_Sensor_t *sensor, uint8_t sensor_id)
{
    if (sensor_id > 7 || sensor_id < 1) {
        return;
    }
    sensor->id = sensor_id;
    sensor->digital_data_temp = 0x00;
    memset(sensor->digital_data, 0x00, sizeof(sensor->digital_data));
}

/**
 * @brief GS Sensor read data function
 *
 * @param sensor
 */
void GS_Sensor_Read_Digital_Data(GS_Sensor_t *sensor)
{
    HAL_StatusTypeDef state = HAL_ERROR;
    uint8_t i2cRxData[8] = {0};
    uint8_t i2cTxData[8] = {0};

    i2cTxData[0] = 0xee;
    //read data on i2c
    do {
        state = HAL_I2C_Master_Transmit(&hi2c1, ((SENSOR_I2C_ADDRESS_BASE + sensor->id) << 1), i2cTxData, 2, 1);
    } while (state != HAL_OK);
    
    do {
        state = HAL_I2C_Master_Receive(&hi2c1, ((SENSOR_I2C_ADDRESS_BASE + sensor->id) << 1), i2cRxData, 2, 1);
    } while (state != HAL_OK);
    //check first frame
    if (i2cRxData[0] == i2cTxData[0]) {
        sensor->digital_data_temp = i2cRxData[1];
        for (uint8_t i = 0; i < 7; i++) {
            sensor->digital_data[i] = (((sensor->digital_data_temp) & (1 << i)) >> i);
        }
    } else {
        return;
    }
}