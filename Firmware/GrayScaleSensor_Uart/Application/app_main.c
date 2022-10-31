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
#include "usart.h"
// include the header file
#include "led.h"
#include "button.h"
#include "Sensor.h"
#include "flash.h"

uint8_t Read_ButtonFn_Level(void);

void ButtonFn_Dowm_CallBack(void *btn);

void ButtonFn_Double_CallBack(void *btn);

void ButtonFn_Long_CallBack(void *btn);

void DEBUG_PIN_Init(void);

Button_t ButtonFn;
/**
 * @brief the main initialization function
 *
 */
void app_main_init(void)
{
    DEBUG_PIN_Init();
    Button_Create("ButtonFn",          //按键名字
                  &ButtonFn,           //按键句柄
                  Read_ButtonFn_Level, //按键电平检测函数接口
                  GPIO_PIN_RESET);     //触发电平
    Button_Attach(&ButtonFn, BUTTON_DOWM, ButtonFn_Dowm_CallBack);
    Button_Attach(&ButtonFn, BUTTON_DOUBLE, ButtonFn_Double_CallBack);
    Button_Attach(&ButtonFn, BUTTON_LONG, ButtonFn_Long_CallBack);
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)Usart1_RX_Buf, Usart1_RX_LEN); //串口DMA接收
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
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
        if (Usart1_RX_Buf[0] == 0x57) {
            HAL_UART_Transmit_DMA(&huart1, &Sensor_TransData_Digital, 1);
            Usart1_RX_Buf[0] = 0;
        }
    }
    if (Loop_10msTime_Flag) // 10ms轮询
    {
        Loop_10msTime_Flag = 0; //清除标志位
    }
    if (Loop_100msTime_Flag) // 100ms轮询
    {
        Loop_100msTime_Flag = 0; //清除标志位
    }
    if (Loop_500msTime_Flag) // 500ms轮询
    {
        Loop_500msTime_Flag = 0; //清除标志位
    }
}
/**
 * @brief debug pin initialization
 *
 */
void DEBUG_PIN_Init(void)
{
#ifdef DEBUG
#else
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOA, LED1_Pin | LED2_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED1_Pin | LED2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
}
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
}
/**
 * @brief double click button callback function
 *
 * @param btn
 */
void ButtonFn_Double_CallBack(void *btn)
{
    if (Sensor_Mode == SENSOR_MODE_RUN) {
        Sensor_clear_calibration();
        Sensor_Mode = SENSOR_MODE_CALIBRATE;
    } else if (Sensor_Mode == SENSOR_MODE_CALIBRATE) {
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
    HAL_DeInit(); // release all to download firmware
}
