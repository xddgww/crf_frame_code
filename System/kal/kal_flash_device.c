#include "kal_flash_device.h"
#include "cal_flash_device.h"
#include "driver_flash.h"

/**********************************************************************
 * 函数名称： KAL_FlashInit
 * 功能描述： 初始化FLASH设备
 * 输入参数： FlashDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_FlashInit(FlashDevice *ptFlashDevice)
{
    return CAL_FlashInit(ptFlashDevice);
}

/**********************************************************************
 * 函数名称： KAL_FlashErase
 * 功能描述： FLASH设备擦除
 * 输入参数： FlashDevice指针
 *           Address：擦除的起始地址
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_FlashErase(FlashDevice *ptFlashDevice, uint32_t Address)
{
    return CAL_FlashErase(ptFlashDevice, Address);
}

/**********************************************************************
 * 函数名称： KAL_FlashWrite
 * 功能描述： FLASH设备写入一个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_FlashWriteData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t data)
{
    return CAL_FlashWriteData(ptFlashDevice, Address, data);
}

/**********************************************************************
 * 函数名称： KAL_FlashWrite
 * 功能描述： FLASH设备写入N个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_FlashWriteNData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t *data, uint16_t num)
{
    return CAL_FlashWriteNData(ptFlashDevice, Address, data, num);
}

/**********************************************************************
 * 函数名称： KAL_FlashWriteHybrid
 * 功能描述： FLASH设备混合写入策略
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_FlashWriteHybrid(FlashDevice *ptFlashDevice, uint32_t addr, uint8_t *data, uint16_t len)
{
    return CAL_FlashWriteHybrid(ptFlashDevice, addr, data, len);
}

/**********************************************************************
 * 函数名称： KAL_FlashReadData
 * 功能描述： FLASH设备读取N个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 **********************************************************************/
int KAL_FlashReadNData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t *data, uint16_t num)
{
    return CAL_FlashReadNData(ptFlashDevice, Address, data, num);
}

/**********************************************************************
 * 函数名称： KAL_FlashReadBytes
 * 功能描述： FLASH设备读取N个uint8_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 **********************************************************************/
int KAL_FlashReadNByte(FlashDevice *ptFlashDevice, uint32_t Address, uint8_t *data, uint16_t num)
{
    return CAL_FlashReadNByte(ptFlashDevice, Address, data, num);
}
