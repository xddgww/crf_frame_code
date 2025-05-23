#include "flash_device.h"
#include "kal_flash_device.h"
#include "debug_log.h"
#include <string.h>


#define USE_LOAD_BALANCE             1  // 开启负载均衡，使得写操作均匀分布在主备份上
#if USE_LOAD_BALANCE
uint32_t bank_min_size = 512;               // 扇区大小,必须是4或者8的倍数
uint32_t frequent_write_pos = 0;            // 频繁数据写入位置
uint32_t static_write_pos = 0;              // 静态数据写入位置
uint32_t last_frequent_addr = 0;            // 最后写入的频繁数据地址
uint32_t last_static_addr = 0;              // 最后写入的静态数据地址
#endif

#define FLASH_MAGIC                  0x55AA1234
#define FREQUENT_DATA_BASE_MASTER    ((uint32_t)0x08019800)  // 频繁数据存储起始主地址
#define FREQUENT_DATA_BASE_SLAVE     ((uint32_t)0x0801A000)  // 频繁数据存储起始备地址
#define STATIC_DATA_BASE_MASTER      ((uint32_t)0x0801A800)  // 静态数据存储起始主地址
#define STATIC_DATA_BASE_SLAVE       ((uint32_t)0x0801B000)  // 静态数据存储起始备地址
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


static int CheckAndEraseIfFull(uint8_t data_type) {
    uint32_t write_pos = (data_type == DATA_TYPE_FREQUENT) ? 
                         frequent_write_pos : static_write_pos;

    // 检查是否接近写满(预留一个entry的空间)
    if((write_pos + bank_min_size) > SECTOR_SIZE) {
        // 擦除整个区域
        FlashEraseData(data_type);
        printf("FlashEraseData: erase all\n");

        if (data_type == DATA_TYPE_FREQUENT) {
            frequent_write_pos = 0;
        } else {
            static_write_pos = 0;
        }

        
        // 清除缓存
        if(data_type == DATA_TYPE_FREQUENT) {
            last_frequent_addr = 0;
        } else {
            last_static_addr = 0;
        }
        
        // if(SaveMetadata() != 0) {
        //     DBG_LOGE("Save metadata failed");
        //     return FLASH_WRITE_FAILED;
        // }
    }
    return FLASH_OK;
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
    uint8_t Erase = 1;
    uint32_t WritePos = 0;

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

    // 是否开启负载均衡
#if USE_LOAD_BALANCE
        CheckAndEraseIfFull(data_type);
        WritePos = (data_type == DATA_TYPE_FREQUENT) ? frequent_write_pos : static_write_pos;
        Erase   = 0;
#endif
    printf("FlashWriteData: WritePos = %d\n", WritePos);


    // 写入第一备份, 失败则重试
    for (int i = 0; i < BACKUP_COPIES; i++)
    {
        if((KAL_FlashWriteHybrid(NULL, MasterAddr + WritePos, (uint8_t *)&entry, sizeof(entry), Erase) == 0) &&
           (KAL_FlashWriteHybrid(NULL, SlaveAddr + WritePos, (uint8_t *)&entry, sizeof(entry), Erase) == 0)) {
            break;
        }
 
        if (i == (BACKUP_COPIES - 1))
        {
            DBG_LOGE("Write failed at 0x%08X or 0x%08X", MasterAddr, SlaveAddr);
            return FLASH_WRITE_FAILED;
        }
    }


    // 更新负载均衡位置
#if USE_LOAD_BALANCE
        if (data_type == DATA_TYPE_FREQUENT) {
            frequent_write_pos += bank_min_size;
        } else {
            static_write_pos += bank_min_size;
        }
#endif

    return FLASH_OK;
}

int FlashWriteMasterData(uint8_t data_type, const void *data, uint16_t length)
{
    
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

#if USE_LOAD_BALANCE
    bank_min_size = 512;
    frequent_write_pos = 0;
    static_write_pos = 0;
    last_frequent_addr = 0;
    last_static_addr = 0;
#endif

    return &g_flash_manager;
}
