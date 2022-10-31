#ifndef APP_MAIN_H
#define APP_MAIN_H
#include "main.h"
#define DEBUG
#ifdef DEBUG
#else
#define LED2_Pin       GPIO_PIN_13
#define LED2_GPIO_Port GPIOA
#define LED1_Pin       GPIO_PIN_14
#define LED1_GPIO_Port GPIOA
#endif // DEBUG
/**
 * @brief the main initialization function
 *
 */
void app_main_init(void);

/**
 * @brief the main loop function
 *
 */
void app_main_loop(void);

#endif // APP_MAIN_H

