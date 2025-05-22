#include "driver_flash.h"
#include "stm32g4xx_hal_flash.h"


/**********************************************************************
 * 函数名称： HAL_FlashInit
 * 功能描述： 初始化FLASH设备
 * 输入参数： FlashDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashInit(FlashDevice *ptFlashDevice)
{
    return 0;
}

int FlashErase(uint32_t Address)
{
	uint32_t FirstPage;
	uint32_t PageError = 0;

    FirstPage = ((Address-FLASH_BASE)/FLASH_PAGE_SIZE);   // 计算起始页地址
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;    // 页擦除
    EraseInitStruct.Banks = FLASH_BANK_1;                 // 擦除的bank
    EraseInitStruct.Page = FirstPage;                     // 擦除1页
    EraseInitStruct.NbPages = 1;                          //擦除多少个页

    if(HAL_FLASHEx_Erase(&EraseInitStruct, &PageError)!=HAL_OK)
    {
        return -1;
    }

    return 0;
}

/**********************************************************************
 * 函数名称： HAL_FlashErase
 * 功能描述： FLASH设备擦除
 * 输入参数： FlashDevice指针
 *           Address：擦除的起始地址
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashErase(FlashDevice *ptFlashDevice, uint32_t Address)
{
    // 1. FLASH解锁
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    if(FlashErase(Address)!=HAL_OK)
    {
        //2、FLASH上锁
        HAL_FLASH_Lock();
        return -1;
    }

    //2、FLASH上锁
    HAL_FLASH_Lock();

    return 0;
}

/**********************************************************************
 * 函数名称： HAL_FlashWrite
 * 功能描述： FLASH设备写入一个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashWriteData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t data)
{
    //1、FLASH解锁
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    //2、FLASH擦除
    FlashErase(Address); 

    //3、FLASH写入
    if(HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, Address, data)!=HAL_OK) {
        return -1;
    }

    //4、FLASH上锁
    HAL_FLASH_Lock();
    return 0;
}

/**********************************************************************
 * 函数名称： HAL_FlashWrite
 * 功能描述： FLASH设备写入N个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashWriteNData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t *data, uint16_t num)
{
    //1、FLASH解锁
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    
    //2、FLASH擦除
    FlashErase(Address); 

    //3、FLASH写入
    for(uint16_t i=0; i<num; i++)
    {
        if(HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, Address, data[i])!=HAL_OK)
        {
            HAL_FLASH_Lock();
            return -1;
        }

        Address += sizeof(uint64_t);
    }

    //4、FLASH上锁
    HAL_FLASH_Lock();
    
    return 0;
}

/**********************************************************************
 * 函数名称： HAL_FlashWriteHybrid
 * 功能描述： FLASH设备混合写入策略
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashWriteHybrid(FlashDevice *ptFlashDevice, uint32_t addr, uint8_t *data, uint16_t len) {
    uint8_t *bytes = (uint8_t *)data;
    int ret = 0;
    
    // 1. FLASH解锁
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    
    // 2. 擦除目标扇区（需根据实际擦除单位调整）
    FlashErase(addr);

    // 3. 混合写入策略
    uint16_t i = 0;
    
    // 情况1：优先处理8字节对齐块
    while (i + 8 <= len) {
        uint64_t word64;
        memcpy(&word64, bytes + i, 8); // 避免对齐问题
        
        if (HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, addr + i, word64) != HAL_OK) {
            ret = -1;
            goto exit;
        }
        i += 8; // 双字移动
    }

    // 情况2：剩余单字节写入
    while (i < len) {
        uint64_t word64;
        memcpy(&word64, bytes + i, 1); // 避免对齐问题
        // 注意：因为该STM32芯片不支持TYPEPROGRAM_BYTE单字节写入，所以这里还是双字写入
        if (HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, addr + i, word64) != HAL_OK) {
            ret = -1;
            goto exit;
        }
        i+=8;    // 单字节移动
    }

exit:

    // 4. FLASH上锁
    HAL_FLASH_Lock();

    return ret;
}

/**********************************************************************
 * 函数名称： HAL_FlashReadData
 * 功能描述： FLASH设备读取N个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 **********************************************************************/
int HAL_FlashReadNData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t *data, uint16_t num)
{
    for(uint32_t i=0; i<num; i++)
    {
        data[i] = *(volatile uint64_t*)Address;
        Address += sizeof(uint64_t);//根据读取的数据类型进行内存地址递增
    }

    return 0;
}

/**********************************************************************
 * 函数名称： HAL_FlashReadBytes
 * 功能描述： FLASH设备读取N个uint8_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 **********************************************************************/
int HAL_FlashReadNByte(FlashDevice *ptFlashDevice, uint32_t Address, uint8_t *data, uint16_t num)
{
    for(uint32_t i=0; i<num; i++)
    {
        data[i] = *(volatile uint8_t*)Address;
        Address += sizeof(uint8_t);//根据读取的数据类型进行内存地址递增
    }

    return 0;
}
