#ifndef __FLASH_MANAGER_H
#define __FLASH_MANAGER_H

#include <stdint.h>

// 数据类型定义
enum {
    DATA_TYPE_FREQUENT,          // 频繁修改的数据
    DATA_TYPE_STATIC,            // 很少修改的数据
    //...
    DATA_TYPE_MAX                // 数据类型最大值，用于边界检查
};

// 存储配置
#define BACKUP_COPIES          2   // 每个数据的备份份数
#define MAX_DATA_SIZE          256 // 最大支持的数据长度
#define SECTOR_SIZE            2048 // Flash块大小(2KB)
#define METADATA_ADDR          ((uint32_t)0x08019000) // 元数据存储固定位置

// 错误码定义
enum {
    FLASH_OK = 0,
    FLASH_INVALID_PARAM,    // 参数错误
    FLASH_WRITE_FAILED,     // 写数据失败
    FLASH_READ_FAILED,      // 读数据失败
    FLASH_CRC_ERROR,        // CRC校验失败
    FLASH_NO_VALID_DATA,    // 无有效数据
    FLASH_ERASE_FAILED      // 擦除失败
};

// 数据条目结构
#pragma pack(push, 1)
typedef struct {
    uint32_t magic;          // 魔数 0x55AA1234
    uint8_t  data_type;      // 数据类型
    uint16_t data_id;        // 数据版本ID
    uint16_t data_length;    // 数据长度
    uint16_t crc16;          // 数据CRC校验
    uint8_t  data[MAX_DATA_SIZE]; // 数据内容
} FlashDataEntry;
#pragma pack(pop)

// Flash管理器接口
typedef struct {
    // 写入数据 (自动双备份)
    int (*Write)(uint8_t data_type, const void *data, uint16_t length);
    
    // 读取数据 (返回最新有效版本)
    int (*Read)(uint8_t data_type, void *buffer, uint16_t max_len);
    
    // 擦除数据
    int (*Erase)(uint8_t data_type);
} FlashDevice;

// 获取Flash管理实例
FlashDevice* GetFlashDevice(void);

#endif /* __FLASH_MANAGER_H */
