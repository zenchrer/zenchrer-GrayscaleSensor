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
#include "string.h"
#include "stm32g0xx_it.h"
#include "gpio.h"
#include "adc.h"
#include "i2c.h"

// include the header file
#include "button_behavior.h"
#include "i2c_behavior.h"
#include "led.h"
#include "Sensor.h"
#include "flash.h"

uint8_t Loop_5msTime_Flag = 0;   // 5msÂÖÑ¯Ê±¼ä
uint8_t Loop_10msTime_Flag = 0;  
uint8_t Loop_20msTime_Flag = 0;  
uint8_t Loop_100msTime_Flag = 0; 
uint8_t Loop_500msTime_Flag = 0; 

uint8_t __setid_tick = 0;
uint8_t __sensor_id = 1;

uint32_t flash_read_buff[29];

/**
 * @brief the main initialization function
 *
 */
void app_main_init(void)
{
    // read the flash , set the threshold and sensor id
    STMFLASH_Read(FLASH_SECTOR15_START, (uint32_t *)flash_read_buff, sizeof(splitThresholds) + sizeof(__sensor_id));
    memcpy(splitThresholds, flash_read_buff, sizeof(splitThresholds));
    memcpy(&__sensor_id, flash_read_buff + sizeof(splitThresholds), sizeof(__sensor_id));
    if (__sensor_id < 1 || __sensor_id > 7) {
        // if sensor is not set ,set it to 1
        __sensor_id = 1;
    }
    Button_init();                                    // init the button
    Sensor_I2C_Init(IIC_ID_BASE + __sensor_id);       // init the sensor i2c
    HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM); // start the adc
    HAL_I2C_Slave_Receive_IT(&hi2c1, i2cDataRx, 2);   // open i2c receive.
    led_all_off();                                    // turn off the LED
}

/**
 * @brief the main loop function
 *
 */
void app_main_loop(void)
{
    if (Loop_5msTime_Flag) // 5ms loop
    {
        Loop_5msTime_Flag = 0; // clear flag
        HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM);
        led_loop();
    }
    if (Loop_10msTime_Flag) {
        Loop_10msTime_Flag = 0;
    }
    if (Loop_20msTime_Flag) {
        Loop_20msTime_Flag = 0;
        Î»
        Button_Process();
    }
    if (Loop_100msTime_Flag) {
        Loop_100msTime_Flag = 0;
        if (__setid_tick > 0) { // start set id mode timer tick
            __setid_tick--;     // 100ms per tick
            if (__setid_tick == 0) {
                if (((__sensor_id + IIC_ID_BASE) << 1) != hi2c1.Init.OwnAddress1) {
                    // check id,if it was changed,
                    //  write new id to the flash and restart
                    memcpy(flash_read_buff + sizeof(splitThresholds), &__sensor_id, sizeof(__sensor_id));
                    STMFLASH_Write(FLASH_SECTOR15_START, flash_read_buff, sizeof(splitThresholds) + sizeof(__sensor_id));
                    NVIC_SystemReset();
                }
                //return normal mode
                Sensor_Mode = SENSOR_MODE_RUN;
            }
        }
    }
    if (Loop_500msTime_Flag) {
        Loop_500msTime_Flag = 0;
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
        Loop_20msTime_Flag = 1;
    }
    if (0 == (time % 100)) {
        Loop_100msTime_Flag = 1;
    }
    if (0 == (time % 500)) {
        Loop_500msTime_Flag = 1;
    }
}
