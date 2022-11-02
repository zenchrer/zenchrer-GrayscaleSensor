/**
 * @file Sensor.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Sensor.h"
extern ADC_HandleTypeDef hadc1;

uint32_t adcValues[SENSOR_NUM] = {0}, maxValues[SENSOR_NUM] = {0}, minValues[SENSOR_NUM] = {0}, splitThresholds[SENSOR_NUM] = {0};
uint8_t Sensor_Mode = SENSOR_MODE_RUN;
uint8_t Sensor_TransData_Digital = 0;
void Sensor_clear_calibration()
{
    // Clear sensor calibration values for next calibration
    for (uint8_t i = 0; i < SENSOR_NUM; i++) {
        maxValues[i] = 0;
    }
    for (uint8_t i = 0; i < SENSOR_NUM; i++) {
        minValues[i] = 1023;
    }
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Stop_DMA(&hadc1); // stop dma
    if (Sensor_Mode == SENSOR_MODE_CALIBRATE) {
        // record the maximum values
        for (uint8_t i = 0; i < SENSOR_NUM; i++) {
            maxValues[i] = maxValues[i] < adcValues[i] ? adcValues[i] : maxValues[i];
        }
        // record the minimum values
        for (uint8_t i = 0; i < SENSOR_NUM; i++) {
            minValues[i] = minValues[i] > adcValues[i] ? adcValues[i] : minValues[i];
        }
        // calculate thresholds
        for (uint8_t i = 0; i < SENSOR_NUM; i++) {
            splitThresholds[i] = ((minValues[i] + maxValues[i]) >> 1);
        }
    }
    // set every bit of digital transform data
    uint8_t sensor_digital_data_temp = 0;
    for (uint8_t i = 0; i < SENSOR_NUM; i++) {
        sensor_digital_data_temp |= ((adcValues[i] > splitThresholds[i]) << i);
    }
    Sensor_TransData_Digital = sensor_digital_data_temp;
    HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM); // enable dma
}
