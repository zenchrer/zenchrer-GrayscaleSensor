/**
 * @file photodiode.h
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PHOTODIODE_H
#define PHOTODIODE_H
#include "main.h"

#define SENSOR_NUM 7
extern uint32_t adcValues[SENSOR_NUM], splitThresholds[SENSOR_NUM];
extern uint8_t Sensor_Mode;
enum ENUM_SENSOR_MODEENUM
{
    SENSOR_MODE_RUN,
    SENSOR_MODE_CALIBRATE
};

void photodiode_clear_calibration(void);

#endif // PHOTODIODE_H
