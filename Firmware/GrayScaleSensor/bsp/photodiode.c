/**
 * @file photodiode.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "photodiode.h"
extern ADC_HandleTypeDef hadc1;

uint32_t adcValues[SENSOR_NUM] = {0}, maxValues[SENSOR_NUM] = {0}, minValues[SENSOR_NUM] = {0}, splitThresholds[SENSOR_NUM] = {0};
uint8_t Sensor_Mode = SENSOR_MODE_RUN;

void photodiode_clear_calibration()
{
    for (uint8_t i = 0; i < SENSOR_NUM; i++)
    {
        maxValues[i] = 0;
    }
    for (uint8_t i = 0; i < SENSOR_NUM; i++)
    {
        minValues[i] = 1023;
    }
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Stop_DMA(&hadc1); //Í£Ö¹DMA
    if (Sensor_Mode == SENSOR_MODE_CALIBRATE)
    {
        for (uint8_t i = 0; i < SENSOR_NUM; i++)
        {
            maxValues[i] = maxValues[i] < adcValues[i] ? adcValues[i] : maxValues[i];
        }
        for (uint8_t i = 0; i < SENSOR_NUM; i++)
        {
            minValues[i] = minValues[i] > adcValues[i] ? adcValues[i] : minValues[i];
        }
        for (uint8_t i = 0; i < SENSOR_NUM; i++)
        {
            splitThresholds[i] = ((minValues[i] + maxValues[i]) >> 1);
        }
    }

    HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM); //¿ªÆôDMA
}
