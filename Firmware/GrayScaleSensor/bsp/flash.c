#include "flash.h"

/*************************************************************
** Function name:       STMFLASH_ReadWord
** Descriptions:        读操作
** Input parameters:    要读取的地址
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
    return *(uint32_t *)faddr;
}

/*************************************************************
** Function name:       STMFLASH_Read
** Descriptions:        连续读取操作
** Input parameters:    NumToRead 要读取的数据个数，单位为uint32
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void STMFLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead)
{
    uint32_t i;
    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = STMFLASH_ReadWord(ReadAddr); //读取4个字节.
        ReadAddr += 4;                            //偏移4个字节.
    }
}

/*************************************************************
** Function name:       STM32G0_GetFlashSector
** Descriptions:        获取G030芯片中addr所在的扇区
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t STM32G0_GetFlashSector(uint32_t addr)
{
    if (addr >= FLASH_SECTOR14_START && addr <= FLASH_SECTOR14_END)
    {
        return 14;
    }
    else if (addr >= FLASH_SECTOR15_START && addr <= FLASH_SECTOR15_END)
    {
        return 15;
    }
    return 16;
}

/*************************************************************
** Function name:       STMFLASH_Write
** Descriptions:        STM32写操作
** Input parameters:    pBuffer 传入需要存储的指针、 NumToWrite 需要写入的字节数量/8(单次最少写入双字大小 8字节)
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    uint32_t PageError            = 0;
    uint32_t addrx                = 0;
    uint32_t endaddr              = 0;
    if (WriteAddr < FLASH_DATA_ADDR_MIN || WriteAddr % 4) return; //非法地址 禁止写入的范围
    HAL_FLASH_Unlock();                                           //解锁
    addrx   = WriteAddr;                                          //写入的起始地址
    endaddr = WriteAddr + NumToWrite * 8;                         //写入的结束地址
    if (addrx < FLASH_DATA_ADDR_MAX)
    {
        while (addrx < endaddr) //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        {
            if (STMFLASH_ReadWord(addrx) != 0XFFFFFFFF) //有非0XFFFFFFFF的地方,要擦除这个扇区
            {
                FlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;         //擦除类型，页擦除
                FlashEraseInit.Page      = STM32G0_GetFlashSector(addrx); //从哪页开始擦除
                FlashEraseInit.NbPages   = 1;                             //一次只擦除一页
                // SEGGER_RTT_printf(0, "Flash Erase page is %d\r\n", FlashEraseInit.Page);
                if (HAL_FLASHEx_Erase(&FlashEraseInit, &PageError) != HAL_OK)
                {
                    // SEGGER_RTT_printf(0, "Flash Erase err\r\n");
                    break; //发生错误了
                }
                FLASH_WaitForLastOperation(FLASH_WAITETIME); //等待上次操作完成
            }
            else
            {
                addrx += 4;
            }
        }
    }
    FlashStatus = FLASH_WaitForLastOperation(FLASH_WAITETIME); //等待上次操作完成
    if (FlashStatus == HAL_OK)
    {
        while (WriteAddr < endaddr) //写数据
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, *(uint64_t *)pBuffer) != HAL_OK) //写入数据
            {
                break; //写入异常
            }
            WriteAddr += 8; //地址加8
            pBuffer += 2;   // buff传进来的是32位的,所以这里＋2 便是8个字节
        }
        // SEGGER_RTT_printf(0, "Flash write ok\r\n");
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME); //等待上次操作完成
    HAL_FLASH_Lock();                            //上锁
}
