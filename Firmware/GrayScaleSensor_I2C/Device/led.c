/**
 * @file led.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "led.h"
#include "Sensor.h"
/**
 * @brief turn off all led 
 * 
 */
void led_all_off(void)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET);
}
/**
 * @brief turn on all led 
 * 
 */
void led_all_on()
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_RESET);
}
/**
 * @brief led follow the sensor
 * 
 */
void led_follow_sensor(void)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, adcValues[0] > splitThresholds[0] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, adcValues[1] > splitThresholds[1] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, adcValues[2] > splitThresholds[2] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, adcValues[3] > splitThresholds[3] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, adcValues[4] > splitThresholds[4] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, adcValues[5] > splitThresholds[5] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, adcValues[6] > splitThresholds[6] ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
/**
 * @brief led in calibration mode
 * 
 */
void led_calibrate_sensor(void)
{
    static uint16_t blink_tick = 0;
    blink_tick++;
    if (blink_tick > 90 && blink_tick < 100) {
        led_all_on();
    } else {
        led_all_off();
    }
    if (blink_tick == 100) {
        blink_tick = 0;
    }
}
/**
 * @brief led display id
 * 
 */
void led_set_id(void)
{
    static uint16_t blink_tick = 0;
    blink_tick++;
    if (blink_tick > 50 && blink_tick < 100) {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, __sensor_id > 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, __sensor_id > 1 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, __sensor_id > 2 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, __sensor_id > 3 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, __sensor_id > 4 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, __sensor_id > 5 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, __sensor_id > 6 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    } else {
        led_all_off();
    }
    if (blink_tick == 100) {
        blink_tick = 0;
    }
}

void led_loop(void)
{
    if (Sensor_Mode == SENSOR_MODE_CALIBRATE) {
        led_calibrate_sensor();
    }
    if (Sensor_Mode == SENSOR_MODE_RUN) {
        led_follow_sensor();
    }
    if (Sensor_Mode == SENSOR_MODE_SETID) {
        led_set_id();
    }
    if (Sensor_Mode == SENSOR_MODE_ENABLE_SWD) {
        led_follow_sensor();
    }
}
