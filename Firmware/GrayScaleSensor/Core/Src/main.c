/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "button.h"
#include "photodiode.h"
#include "flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
Button_t ButtonFn;
extern uint8_t Loop_5msTime_Flag;   // 5ms轮询时间
extern uint8_t Loop_10msTime_Flag;  // 10ms轮询时间
extern uint8_t Loop_100msTime_Flag; // 100ms轮询时间
extern uint8_t Loop_500msTime_Flag; // 500ms轮询时间

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t Read_ButtonFn_Level(void);

void ButtonFn_Dowm_CallBack(void *btn);

void ButtonFn_Double_CallBack(void *btn);

void ButtonFn_Long_CallBack(void *btn);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    MX_DMA_Init();

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    Button_Create("ButtonFn",          //按键名字
                  &ButtonFn,           //按键句柄
                  Read_ButtonFn_Level, //按键电平检测函数接口
                  GPIO_PIN_RESET);     //触发电平
    Button_Attach(&ButtonFn, BUTTON_DOWM, ButtonFn_Dowm_CallBack);
    Button_Attach(&ButtonFn, BUTTON_DOUBLE, ButtonFn_Double_CallBack);
    Button_Attach(&ButtonFn, BUTTON_LONG, ButtonFn_Long_CallBack);
    HAL_ADC_Start_DMA(&hadc1, adcValues, SENSOR_NUM);
    led_all_off();
    STMFLASH_Read(FLASH_SECTOR15_START, (uint32_t *)splitThresholds, sizeof(splitThresholds));

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
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
        }
        if (Loop_500msTime_Flag) // 500ms轮询
        {

            Loop_500msTime_Flag = 0; //清除标志位
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN            = 8;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
uint8_t Read_ButtonFn_Level(void)
{
    return HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
}

void ButtonFn_Dowm_CallBack(void *btn)
{
}

void ButtonFn_Double_CallBack(void *btn)
{
    if (Sensor_Mode == SENSOR_MODE_RUN)
    {
        photodiode_clear_calibration();
        Sensor_Mode = SENSOR_MODE_CALIBRATE;
    }
    else if (Sensor_Mode == SENSOR_MODE_CALIBRATE)
    {
        Sensor_Mode = SENSOR_MODE_RUN;
        STMFLASH_Write(FLASH_SECTOR15_START, (uint32_t *)splitThresholds, (sizeof(splitThresholds) >> 2));
    }
}

void ButtonFn_Long_CallBack(void *btn)
{
    HAL_DeInit();
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
