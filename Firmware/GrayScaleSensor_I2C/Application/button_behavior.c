/**
 * @file button_behavior.c
 * @author zenchrer (zenchrer@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "button_behavior.h"
#include "app_main.h"
#include "button.h"
#include "Sensor.h"
#include "flash.h"
#include "gpio.h"

Button_t ButtonFn;

/**
 * @brief read button state function
 *
 * @return uint8_t
 */
uint8_t Read_ButtonFn_Level(void)
{
    return HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
}
/**
 * @brief button press down callback function
 *
 * @param btn
 */
void ButtonFn_Dowm_CallBack(void *btn)
{
    if (Sensor_Mode == SENSOR_MODE_RUN) {
        // only sensor in running mode can enter setid mode
        // first triggered can watch the sensor id
        Sensor_Mode = SENSOR_MODE_SETID;
        __setid_tick = 30;
    } else if (Sensor_Mode == SENSOR_MODE_SETID) {
        // if it was enter the setid mode, click the button to plus sensor id
        // if sensor id is larger than max id , it will be reset
        __sensor_id += __sensor_id == 7 ? -6 : 1;
        __setid_tick = 30;
    }
}
/**
 * @brief double click button callback function
 *
 * @param btn
 */
void ButtonFn_Double_CallBack(void *btn)
{
    if (Sensor_Mode == SENSOR_MODE_RUN) {
        // only sensor in running mode can enter calibration
        Sensor_clear_calibration();
        Sensor_Mode = SENSOR_MODE_CALIBRATE;
    } else if (Sensor_Mode == SENSOR_MODE_CALIBRATE) {
        // calibration done, write calibration data to flash
        Sensor_Mode = SENSOR_MODE_RUN;
        STMFLASH_Write(FLASH_SECTOR15_START, (uint32_t *)splitThresholds, (sizeof(splitThresholds) >> 2));
    }
}
/**
 * @brief long press button callback function
 *
 * @param btn
 */
void ButtonFn_Long_CallBack(void *btn)
{
    if (Sensor_Mode == SENSOR_MODE_RUN) {
        // release the GPIOA
        __HAL_RCC_GPIOA_FORCE_RESET();
        __HAL_RCC_GPIOA_RELEASE_RESET();
        // reinit the none SWD pins
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(GPIOA, LED6_Pin | LED4_Pin | LED3_Pin, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = LED6_Pin | LED4_Pin | LED3_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        // set sensor mode
        Sensor_Mode = SENSOR_MODE_ENABLE_SWD;
    } else if (Sensor_Mode == SENSOR_MODE_ENABLE_SWD) {
        // reinit gpio pins
        MX_GPIO_Init();
        // set sensor mode
        Sensor_Mode = SENSOR_MODE_RUN;
    }
}

/**
 * @brief init button and link button callback function
 *
 */
void Button_init(void)
{
    Button_Create("ButtonFn",          //按键名字
                  &ButtonFn,           //按键句柄
                  Read_ButtonFn_Level, //按键电平检测函数接口
                  GPIO_PIN_RESET);     //触发电平
    // link button callback function
    Button_Attach(&ButtonFn, BUTTON_DOWM, ButtonFn_Dowm_CallBack);
    Button_Attach(&ButtonFn, BUTTON_DOUBLE, ButtonFn_Double_CallBack);
    Button_Attach(&ButtonFn, BUTTON_LONG, ButtonFn_Long_CallBack);
}