#include "flash_device.h"
#include "kal_flash_device.h"
#include "debug_log.h"
#include <string.h>

#define FLASH_MAGIC                  0x55AA1234
#define FREQUENT_DATA_BASE_MASTER    ((uint32_t)0x08019800)  // 频繁数据存储起始主地址
#define FREQUENT_DATA_BASE_SLAVE     ((uint32_t)0x08019800)  // 频繁数据存储起始备地址
#define STATIC_DATA_BASE_MASTER      ((uint32_t)0x0801A000)  // 静态数据存储起始主地址
#define STATIC_DATA_BASE_SLAVE       ((uint32_t)0x0801A000)  // 静态数据存储起始备地址
#define FREQUENT_DATA_END1           (FREQUENT_DATA_BASE_MASTER + SECTOR_SIZE)
#define FREQUENT_DATA_END2           (FREQUENT_DATA_BASE_SLAVE + SECTOR_SIZE)
#define STATIC_DATA_END1             (STATIC_DATA_BASE_MASTER + SECTOR_SIZE)
#define STATIC_DATA_END2             (STATIC_DATA_BASE_SLAVE + SECTOR_SIZE)
#define BACKUP_COPIES                2 // flash重写次数

static uint16_t CalculateCRC16(const void *data, uint16_t len) 
{
    const uint8_t *bytes = (const uint8_t *)data;
    uint16_t sum = 0;
    
    // 参数检查
    if(!data || len < 4) return 0xFFFF; // 错误值
    
    // 计算校验和(跳过魔术字和自身字段)
    for(uint16_t i = 2; i < len - 2; i++) {
        sum += bytes[i];
    }
    
    return (uint16_t)(~sum);
}

static uint32_t GetBaseAddrMaster(uint8_t data_type) {
    return (data_type == DATA_TYPE_FREQUENT) ? FREQUENT_DATA_BASE_MASTER : STATIC_DATA_BASE_MASTER;
}

static uint32_t GetBaseAddrSlave(uint8_t data_type) {
    return (data_type == DATA_TYPE_FREQUENT) ? FREQUENT_DATA_BASE_SLAVE : STATIC_DATA_BASE_SLAVE;
}

static int ValidateEntry(const FlashDataEntry *entry) {
    if(entry->magic != FLASH_MAGIC) return 0;
    if(entry->data_length > MAX_DATA_SIZE) return 0;
    uint16_t crc = CalculateCRC16(entry->data, entry->data_length);
    return (crc == entry->crc16) ? 1 : 0;
}

/**********************************************************************
 * 函数名称： FlashWriteData
 * 功能描述： 写flash数据
 * 输入参数： data：数据指针
 *           length：数据长度
 * 输出参数： 无
 * 返 回 值： 0：成功，其他：失败
 **********************************************************************/
static int FlashWriteData(uint8_t data_type, const void *data, uint16_t length)
{
    // 参数检查
    if ((data == NULL) || (length == 0) || (length > MAX_DATA_SIZE) || 
        (data_type >= DATA_TYPE_MAX)) {
        DBG_LOGE("FlashWriteData: Invalid parameter");
        return FLASH_INVALID_PARAM;
    }

    // 准备数据条目
    FlashDataEntry entry;
    entry.magic = FLASH_MAGIC;
    entry.data_type = data_type;
    entry.data_length = length;
    entry.crc16 = CalculateCRC16(data, length);
    memcpy(entry.data, data, length);

    // 计算写入地址
    uint32_t MasterAddr = GetBaseAddrMaster(data_type);
    uint32_t SlaveAddr  = GetBaseAddrSlave(data_type);

    // 写入第一备份, 失败则重试
    for (int i = 0; i < BACKUP_COPIES; i++)
    {
        if((KAL_FlashWriteHybrid(NULL, MasterAddr, (uint8_t *)&entry, sizeof(entry)) == 0) &&
           (KAL_FlashWriteHybrid(NULL, SlaveAddr, (uint8_t *)&entry, sizeof(entry)) == 0)) {
            break;
        }
 
        if (i == (BACKUP_COPIES - 1))
        {
            DBG_LOGE("Write failed at 0x%08X or 0x%08X", MasterAddr, SlaveAddr);
            return FLASH_WRITE_FAILED;
        }
    }

    return FLASH_OK;
}

/**********************************************************************
 * 函数名称： FlashReadData
 * 功能描述： 读flash数据
 * 输入参数： data：数据指针
 *           length：数据长度
 * 输出参数： 无
 * 返 回 值： 0：成功，其他：失败
 **********************************************************************/
static int FlashReadData(uint8_t data_type, void *buffer, uint16_t max_len)
{
    // 参数检查
    if ((buffer == NULL) || (max_len == 0) || (max_len > MAX_DATA_SIZE) || 
        (data_type >= DATA_TYPE_MAX)) {
        DBG_LOGE("FlashReadData: Invalid parameter");
        return FLASH_INVALID_PARAM;
    }

    // 计算写入地址
    uint32_t MasterAddr = GetBaseAddrMaster(data_type);
    uint32_t SlaveAddr  = GetBaseAddrSlave(data_type);

    // 读取数据
    FlashDataEntry entry;
    if ((KAL_FlashReadNByte(NULL, MasterAddr, (uint8_t *)&entry, sizeof(entry)) == 0) && 
        (ValidateEntry(&entry))) {
        // 复制数据
        memcpy(buffer, entry.data, entry.data_length);
        return FLASH_OK;
    }

    if ((KAL_FlashReadNByte(NULL, SlaveAddr, (uint8_t *)&entry, sizeof(entry)) == 0) && 
        (ValidateEntry(&entry))) {
            DBG_LOGD("Read from backup");
            // 复制数据
            memcpy(buffer, entry.data, entry.data_length);
        return FLASH_OK;
    } else {
        DBG_LOGE("Read failed at 0x%08X or 0x%08X", MasterAddr, SlaveAddr);
        return FLASH_READ_FAILED;
    }
}


/**********************************************************************
 * 函数名称： FlashReadData
 * 功能描述： 读flash数据
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0：成功，其他：失败
 **********************************************************************/
static int FlashEraseData(uint8_t data_type)
{
    // 参数检查
    if (data_type >= DATA_TYPE_MAX) {
        DBG_LOGE("FlashEraseData: Invalid parameter");
        return FLASH_INVALID_PARAM;
    }

    // 计算写入地址
    uint32_t MasterAddr = GetBaseAddrMaster(data_type);
    uint32_t SlaveAddr  = GetBaseAddrSlave(data_type);

    // 擦除数据
    if (KAL_FlashErase(NULL, MasterAddr) != 0) {
        DBG_LOGE("Erase failed at 0x%08X", MasterAddr);
        return FLASH_ERASE_FAILED;
    }
    if (KAL_FlashErase(NULL, SlaveAddr) != 0) {
        DBG_LOGE("Erase failed at 0x%08X", SlaveAddr);
        return FLASH_ERASE_FAILED;
    }

    return FLASH_OK;
}

static FlashDevice g_flash_manager = {
    .Write = FlashWriteData,
    .Read = FlashReadData,
    .Erase = FlashEraseData,
};

/**********************************************************************
 * 函数名称： GetFlashDevice
 * 功能描述： 获取唯一的flash设备
 * 输入参数： 无
 * 返 回 值： FlashDevice指针
 **********************************************************************/
FlashDevice* GetFlashDevice(void) {
    return &g_flash_manager;
}
