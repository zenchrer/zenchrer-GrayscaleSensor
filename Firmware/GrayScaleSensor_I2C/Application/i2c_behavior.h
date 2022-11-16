/**
 * @file i2c_behavior.h
 * @author zenchrer (zenchrer@qq.com)
 * @brief 
 * @version 1.0
 * @date 2022-11-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef I2C_BEHAVIOR_H
#define I2C_BEHAVIOR_H

#include "main.h"

#define IIC_ID_BASE (0x50)//i2c device address base address

extern uint8_t i2cDataRx[8];

extern uint8_t i2cDataTx[8];

void Sensor_I2C_Init(uint8_t __id);

void I2C_SlaveDMARxCpltCallback();

void Sensor_Set_ID(uint8_t _id);

#endif // I2C_BEHAVIOR_H