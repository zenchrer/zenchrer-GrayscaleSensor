#ifndef APP_MAIN_H
#define APP_MAIN_H
#include "main.h"

extern uint8_t __setid_tick,__sensor_id;
extern uint32_t flash_read_buff[29];

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

/**
 * @brief fsm tick handler function
 *
 */
void app_tick_handler(void);

#endif // APP_MAIN_H
