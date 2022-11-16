/**
 * @file Sensor.h
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SENSOR_H
#define SENSOR_H

#include "app_main.h"

#define SENSOR_NUM 7
extern uint32_t adcValues[SENSOR_NUM], splitThresholds[SENSOR_NUM];
extern uint8_t Sensor_Mode, Sensor_TransData_Digital;
enum ENUM_SENSOR_MODEENUM
{
    SENSOR_MODE_RUN,
    SENSOR_MODE_CALIBRATE,
    SENSOR_MODE_SETID,
    SENSOR_MODE_ENABLE_SWD,
    SENSOR_MODE_END
};

void Sensor_clear_calibration(void);

#endif // SENSOR_H
