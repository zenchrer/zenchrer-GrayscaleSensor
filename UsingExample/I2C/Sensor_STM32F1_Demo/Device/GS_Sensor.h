/**
 * @file GS_Sensor.h
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-11-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef GS_SENSOR_H
#define GS_SENSOR_H

#include "main.h"
#include "i2c.h"

#define sensor_i2c              hi2c1
#define SENSOR_I2C_ADDRESS_BASE (0x50)

typedef struct {
    uint8_t id;                // sensor id
    uint8_t digital_data_temp; // transfer temp
    uint8_t digital_data[7];   // sensor digital data will be 0 or 1
} GS_Sensor_t;

/**
 * @brief GS_Sensor initialization function
 * 
 * @param sensor 
 * @param sensor_id 
 */
void GS_Sensor_Init(GS_Sensor_t *sensor, uint8_t sensor_id);

/**
 * @brief GS Sensor read data function
 * 
 * @param sensor 
 */
void GS_Sensor_Read_Digital_Data(GS_Sensor_t *sensor);

#endif // GS_SENSOR_H