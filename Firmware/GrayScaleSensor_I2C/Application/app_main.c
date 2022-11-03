/**
 * @file app_main.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-10-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "app_main.h"
#include "main.h"
#include "stm32g0xx_it.h"
#include "gpio.h"
#include "adc.h"
// include the header file
#include "button_behavior.h"
#include "led.h"
#include "Sensor.h"
#include "flash.h"

uint8_t Loop_5msTime_Flag = 0;   // 5ms轮询时间
uint8_t Loop_10msTime_Flag = 0;  // 10ms轮询时间
uint8_t Loop_100msTime_Flag = 0; // 100ms轮询时间
uint8_t Loop_500msTime_Flag = 0; // 500ms轮询时间

uint8_t __setid_tick = 0;
uint8_t __sensor_id=1;
/**
 * @brief the main initialization function
 *
 */
void app_main_init(void)
{
    Button_init();
    HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM);
    led_all_off();
    STMFLASH_Read(FLASH_SECTOR15_START, (uint32_t *)splitThresholds, sizeof(splitThresholds));
}

/**
 * @brief the main loop function
 *
 */
void app_main_loop(void)
{
    if (Loop_5msTime_Flag) // 5ms轮询
    {
        Loop_5msTime_Flag = 0; //清除标志位
        led_loop();
    }
    if (Loop_10msTime_Flag) // 10ms轮询
    {
        Loop_10msTime_Flag = 0; //清除标志位
    }
    if (Loop_100msTime_Flag) // 100ms轮询
    {
        Loop_100msTime_Flag = 0; //清除标志位
        if (__setid_tick > 0) {//start set id timer tick
            __setid_tick--; //
            if (__setid_tick == 0) {
                Sensor_Mode = SENSOR_MODE_RUN;
            }
        }
    }
    if (Loop_500msTime_Flag) // 500ms轮询
    {
        Loop_500msTime_Flag = 0; //清除标志位
    }
}

void app_tick_handler(void)
{
    static uint32_t time;
    time++;
    if (0 == (time % 5)) {
        Loop_5msTime_Flag = 1;
    }
    if (0 == (time % 10)) {
        Loop_10msTime_Flag = 1;
    }
    if (0 == (time % 20)) {
        Button_Process();
    }
    if (0 == (time % 100)) {
        Loop_100msTime_Flag = 1;
    }
    if (0 == (time % 500)) {
        Loop_500msTime_Flag = 1;
    }
}
