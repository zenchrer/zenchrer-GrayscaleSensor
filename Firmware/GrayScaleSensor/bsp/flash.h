#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"
#include "string.h"

#define FLASH_SECTOR14_START        0x08007000
#define FLASH_SECTOR14_END          0x080077FF
#define FLASH_SECTOR15_START        0x08007800
#define FLASH_SECTOR15_END          0x08007FFF

#define FLASH_DATA_ADDR_MIN         FLASH_SECTOR14_START
#define FLASH_DATA_ADDR_MAX         FLASH_SECTOR15_END

#define FLASH_DATA_ADDR_BASE        FLASH_DATA_ADDR_MIN
#define CALIBRATION_L_LR_VALUE_ADDR FLASH_DATA_ADDR_BASE + 0
#define CALIBRATION_L_UD_VALUE_ADDR FLASH_DATA_ADDR_BASE + 12
#define CALIBRATION_R_LR_VALUE_ADDR FLASH_DATA_ADDR_BASE + 24
#define CALIBRATION_R_UD_VALUE_ADDR FLASH_DATA_ADDR_BASE + 36
#define FLASH_WAITETIME             1000

// bind addr长度为5字节  实际占用2个uint32
#define BIND_MSG_BASE FLASH_SECTOR15_START
#define BIND_ADDR     BIND_MSG_BASE + 0

void STMFLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead);
void STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite);

#endif
