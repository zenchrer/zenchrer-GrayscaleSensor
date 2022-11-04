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
#include "led.h"
#include "Sensor.h"
#include "flash.h"

uint8_t Loop_5msTime_Flag = 0;   // 5ms轮询时间
uint8_t Loop_10msTime_Flag = 0;  // 10ms轮询时间
uint8_t Loop_20msTime_Flag = 0;  // 20ms轮询时间
uint8_t Loop_100msTime_Flag = 0; // 100ms轮询时间
uint8_t Loop_500msTime_Flag = 0; // 500ms轮询时间

uint8_t __setid_tick = 0;
uint8_t __sensor_id = 1;
#define IIC_ID_BASE (0x50)
// I2C Pins
// RX = SDA   TX = SCL
uint8_t i2cDataRx[8];
uint8_t i2cDataTx[8];

uint32_t flash_read_buff[29];

void Sensor_I2C1_Init(uint8_t __id);

/**
 * @brief the main initialization function
 *
 */
void app_main_init(void)
{
    Button_init();
    STMFLASH_Read(FLASH_SECTOR15_START, (uint32_t *)flash_read_buff, sizeof(splitThresholds) + sizeof(__sensor_id));
    memcpy(splitThresholds, flash_read_buff, sizeof(splitThresholds));
    memcpy(&__sensor_id, flash_read_buff + sizeof(splitThresholds), sizeof(__sensor_id));
    if (__sensor_id < 1 || __sensor_id > 7) {
        __sensor_id = 1;
    }
    Sensor_I2C1_Init(IIC_ID_BASE + __sensor_id);
    HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM);    
    HAL_I2C_Slave_Receive_IT(&hi2c1, i2cDataRx, 2); // open i2c DMA receive.
    led_all_off();
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
    if (Loop_20msTime_Flag) // 20ms轮询
    {
        Loop_20msTime_Flag = 0; //清除标志位
        Button_Process();
    }
    if (Loop_100msTime_Flag) // 100ms轮询
    {
        Loop_100msTime_Flag = 0; //清除标志位
        if (__setid_tick > 0) {  // start set id timer tick
            __setid_tick--;      //
            if (__setid_tick == 0) {
                if(__sensor_id)
                memcpy(flash_read_buff + sizeof(splitThresholds), &__sensor_id, sizeof(__sensor_id));
                STMFLASH_Write(FLASH_SECTOR15_START, flash_read_buff, sizeof(splitThresholds) + sizeof(__sensor_id));
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
        Loop_20msTime_Flag = 1;
    }
    if (0 == (time % 100)) {
        Loop_100msTime_Flag = 1;
    }
    if (0 == (time % 500)) {
        Loop_500msTime_Flag = 1;
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef state = HAL_ERROR;

    i2cDataTx[0] = i2cDataRx[0];
    switch (i2cDataRx[0]) {

        case 0xee:
            i2cDataTx[1] = Sensor_TransData_Digital;
            break;
        default:
            break;
    }

    do {
        state = HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t *)i2cDataTx, 2, 1000);
    } while (state != HAL_OK);
    //        state = HAL_I2C_Slave_Transmit_DMA(&hi2c1, (uint8_t*) i2cDataTx, 5);

    do {
        state = HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)i2cDataRx, 2);
    } while (state != HAL_OK);
}

/* I2C1 init function */
void Sensor_I2C1_Init(uint8_t __id)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00300B29;
    hi2c1.Init.OwnAddress1 = (__id << 1);
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        Error_Handler();
    }

    /** I2C Fast mode Plus enable
     */
    HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
}
