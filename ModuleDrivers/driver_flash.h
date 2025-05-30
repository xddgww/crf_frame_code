#ifndef __DRIVER_FLASH_H
#define __DRIVER_FLASH_H

#include "flash_device.h"
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//STM32G431RBT6的FLASH大小为128KB，只有63个页
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000800) /* Base @ of Page 1, 2 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08001000) /* Base @ of Page 2, 2 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08001800) /* Base @ of Page 3, 2 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08002000) /* Base @ of Page 4, 2 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08002800) /* Base @ of Page 5, 2 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08003000) /* Base @ of Page 6, 2 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08003800) /* Base @ of Page 7, 2 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08004000) /* Base @ of Page 8, 2 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08004800) /* Base @ of Page 9, 2 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08005000) /* Base @ of Page 10, 2 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08005800) /* Base @ of Page 11, 2 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08006000) /* Base @ of Page 12, 2 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x08006800) /* Base @ of Page 13, 2 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x08007000) /* Base @ of Page 14, 2 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x08007800) /* Base @ of Page 15, 2 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08008000) /* Base @ of Page 16, 2 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08008800) /* Base @ of Page 17, 2 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08009000) /* Base @ of Page 18, 2 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08009800) /* Base @ of Page 19, 2 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x0800A000) /* Base @ of Page 20, 2 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x0800A800) /* Base @ of Page 21, 2 Kbytes */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x0800B000) /* Base @ of Page 22, 2 Kbytes */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x0800B800) /* Base @ of Page 23, 2 Kbytes */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x0800C000) /* Base @ of Page 24, 2 Kbytes */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x0800C800) /* Base @ of Page 25, 2 Kbytes */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x0800D000) /* Base @ of Page 26, 2 Kbytes */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x0800D800) /* Base @ of Page 27, 2 Kbytes */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x0800E000) /* Base @ of Page 28, 2 Kbytes */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x0800E800) /* Base @ of Page 29, 2 Kbytes */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x0800F000) /* Base @ of Page 30, 2 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x0800F800) /* Base @ of Page 31, 2 Kbytes */
#define ADDR_FLASH_PAGE_32    ((uint32_t)0x08010000) /* Base @ of Page 32, 2 Kbytes */
#define ADDR_FLASH_PAGE_33    ((uint32_t)0x08010800) /* Base @ of Page 33, 2 Kbytes */
#define ADDR_FLASH_PAGE_34    ((uint32_t)0x08011000) /* Base @ of Page 34, 2 Kbytes */
#define ADDR_FLASH_PAGE_35    ((uint32_t)0x08011800) /* Base @ of Page 35, 2 Kbytes */
#define ADDR_FLASH_PAGE_36    ((uint32_t)0x08012000) /* Base @ of Page 36, 2 Kbytes */
#define ADDR_FLASH_PAGE_37    ((uint32_t)0x08012800) /* Base @ of Page 37, 2 Kbytes */
#define ADDR_FLASH_PAGE_38    ((uint32_t)0x08013000) /* Base @ of Page 38, 2 Kbytes */
#define ADDR_FLASH_PAGE_39    ((uint32_t)0x08013800) /* Base @ of Page 39, 2 Kbytes */
#define ADDR_FLASH_PAGE_40    ((uint32_t)0x08014000) /* Base @ of Page 40, 2 Kbytes */
#define ADDR_FLASH_PAGE_41    ((uint32_t)0x08014800) /* Base @ of Page 41, 2 Kbytes */
#define ADDR_FLASH_PAGE_42    ((uint32_t)0x08015000) /* Base @ of Page 42, 2 Kbytes */
#define ADDR_FLASH_PAGE_43    ((uint32_t)0x08015800) /* Base @ of Page 43, 2 Kbytes */
#define ADDR_FLASH_PAGE_44    ((uint32_t)0x08016000) /* Base @ of Page 44, 2 Kbytes */
#define ADDR_FLASH_PAGE_45    ((uint32_t)0x08016800) /* Base @ of Page 45, 2 Kbytes */
#define ADDR_FLASH_PAGE_46    ((uint32_t)0x08017000) /* Base @ of Page 46, 2 Kbytes */
#define ADDR_FLASH_PAGE_47    ((uint32_t)0x08017800) /* Base @ of Page 47, 2 Kbytes */
#define ADDR_FLASH_PAGE_48    ((uint32_t)0x08018000) /* Base @ of Page 48, 2 Kbytes */
#define ADDR_FLASH_PAGE_49    ((uint32_t)0x08018800) /* Base @ of Page 49, 2 Kbytes */
#define ADDR_FLASH_PAGE_50    ((uint32_t)0x08019000) /* Base @ of Page 50, 2 Kbytes */
#define ADDR_FLASH_PAGE_51    ((uint32_t)0x08019800) /* Base @ of Page 51, 2 Kbytes */
#define ADDR_FLASH_PAGE_52    ((uint32_t)0x0801A000) /* Base @ of Page 52, 2 Kbytes */
#define ADDR_FLASH_PAGE_53    ((uint32_t)0x0801A800) /* Base @ of Page 53, 2 Kbytes */
#define ADDR_FLASH_PAGE_54    ((uint32_t)0x0801B000) /* Base @ of Page 54, 2 Kbytes */
#define ADDR_FLASH_PAGE_55    ((uint32_t)0x0801B800) /* Base @ of Page 55, 2 Kbytes */
#define ADDR_FLASH_PAGE_56    ((uint32_t)0x0801C000) /* Base @ of Page 56, 2 Kbytes */
#define ADDR_FLASH_PAGE_57    ((uint32_t)0x0801C800) /* Base @ of Page 57, 2 Kbytes */
#define ADDR_FLASH_PAGE_58    ((uint32_t)0x0801D000) /* Base @ of Page 58, 2 Kbytes */
#define ADDR_FLASH_PAGE_59    ((uint32_t)0x0801D800) /* Base @ of Page 59, 2 Kbytes */
#define ADDR_FLASH_PAGE_60    ((uint32_t)0x0801E000) /* Base @ of Page 60, 2 Kbytes */
#define ADDR_FLASH_PAGE_61    ((uint32_t)0x0801E800) /* Base @ of Page 61, 2 Kbytes */
#define ADDR_FLASH_PAGE_62    ((uint32_t)0x0801F000) /* Base @ of Page 62, 2 Kbytes */
#define ADDR_FLASH_PAGE_63    ((uint32_t)0x0801F800) /* Base @ of Page 63, 2 Kbytes */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_15   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_18   /* End @ of user Flash area */

/**********************************************************************
 * 函数名称： HAL_FlashInit
 * 功能描述： 初始化FLASH设备
 * 输入参数： FlashDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashInit(FlashDevice *ptFlashDevice);

/**********************************************************************
 * 函数名称： HAL_FlashErase
 * 功能描述： FLASH设备擦除
 * 输入参数： FlashDevice指针
 *           Address：擦除的起始地址
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashErase(FlashDevice *ptFlashDevice, uint32_t Address);

/**********************************************************************
 * 函数名称： HAL_FlashWrite
 * 功能描述： FLASH设备写入一个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashWriteData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t data);

/**********************************************************************
 * 函数名称： HAL_FlashWriteNData
 * 功能描述： FLASH设备写入N个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashWriteNData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t *data, uint16_t num);

/**********************************************************************
 * 函数名称： HAL_FlashWriteHybrid
 * 功能描述： FLASH设备混合写入策略
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 *           Erase：擦除策略：0-不擦除；1-先擦除再写入
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_FlashWriteHybrid(FlashDevice *ptFlashDevice, uint32_t addr, uint8_t *data, uint16_t len, uint8_t Erase);

/**********************************************************************
 * 函数名称： HAL_FlashReadData
 * 功能描述： FLASH设备读取N个uint64_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 **********************************************************************/
int HAL_FlashReadNData(FlashDevice *ptFlashDevice, uint32_t Address, uint64_t *data, uint16_t num);

/**********************************************************************
 * 函数名称： HAL_FlashReadBytes
 * 功能描述： FLASH设备读取N个uint8_t类型的数据
 * 输入参数： FlashDevice指针
 *           Address：存储数据的地址
 *           data：数据数组
 *           num：数据的个数
 * 返 回 值： 0：成功；其他：失败
 **********************************************************************/
int HAL_FlashReadNByte(FlashDevice *ptFlashDevice, uint32_t Address, uint8_t *data, uint16_t num);

#endif /* __DRIVER_FLASH_H */
