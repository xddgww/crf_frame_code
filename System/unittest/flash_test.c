#include <stdio.h>
#include "flash_test.h"
#include "unit_common.h"
#include "flash_device.h"
#include "driver_flash.h"

#ifdef FLASH_TEST_FUNCTION
#if UNIT_TESTING_ENABLED

uint8_t data_write_buf3[SECTOR_SIZE] = {0};

// 数据备份功能测试1000次
#define TEST_NUM 1000
#define BUF_SIZE 100
uint8_t str_write_buf[BUF_SIZE] = {0};
uint8_t str_read_buf[BUF_SIZE] = {0};
uint8_t faile_num = 0;
/**********************************************************************
 * 函数名称： flash_driver_test
 * 功能描述： flash驱动测试函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void flash_driver_test(void)
{
    // 读写字符串测试
    printf("Flash Test 1 Starting...\r\n");
    uint8_t str_write_buf1[20] = "hello world!";
    HAL_FlashWriteNData(NULL, ADDR_FLASH_PAGE_40, (uint64_t*)str_write_buf1, 20);
    printf("write Str:%s\r\n" , str_write_buf1);
    uint8_t str_read_buf1[20] = {0};
    HAL_FlashReadNByte(NULL, ADDR_FLASH_PAGE_40, str_read_buf1, 20);
    printf("read Str:%s\r\n" , str_read_buf1);
    printf("Flash Test 1 Ending...\r\n\r\n");

    // 读写整形数据测试
    printf("Flash Test 2 Starting...\r\n");
    uint64_t data_write_buf2[2] = {0x123456789ABCDEF0, 0x1122334455667788};
    HAL_FlashWriteNData(NULL, 0x08019800, data_write_buf2, 2);
    printf("read Data:");
    for (int i = 0; i < 2; i++)
    {
        printf("data[%d]:%llX  ", i, data_write_buf2[i]);
    }
    printf("\r\n");
    
    uint64_t data_read_buf2[2] = {0};
    HAL_FlashReadNData(NULL, 0x08019800, data_read_buf2, 2);
    printf("read Data:");
    for (int i = 0; i < 2; i++) {
        printf("data[%d]:%llX  ", i, data_read_buf2[i]);
    }
    printf("\r\n");
    printf("Flash Test 2 Ending...\r\n\r\n");

    // 读写扇区测试
    // 通过KEIL仿真，查看是否写2k数据，是否擦除2k扇区
    printf("Flash Test 3 Starting...\r\n");
    HAL_FlashWriteHybrid(NULL, 0x08019800, data_write_buf3, SECTOR_SIZE);
    HAL_FlashErase(NULL, 0x08019800);
    printf("Flash Test 3 Ending...\r\n\r\n");
}
extern int FlashWrite(uint8_t data_type, const void *data, uint16_t length);
/**********************************************************************
 * 函数名称： flash_driver_test
 * 功能描述： flash设备测试函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void flash_device_test(void)
{
    FlashDevice* ptFlashDevice = GetFlashDevice();

    // 读写字符串测试
    printf("Flash Device Test 1 Starting...\r\n");
    for (int i = 0; i < TEST_NUM; i++) {
        // 随机生成内容
        for (int j = 0; j < BUF_SIZE; j++) {
            str_write_buf[j] = rand() % 256;
        }
        ptFlashDevice->Write(DATA_TYPE_FREQUENT, str_write_buf, BUF_SIZE);
        ptFlashDevice->Read(DATA_TYPE_FREQUENT, str_read_buf, BUF_SIZE);
        if (memcmp(str_write_buf, str_read_buf, BUF_SIZE) != 0) {
            faile_num++;
        }
        memset(str_read_buf, 0, BUF_SIZE);
        ptFlashDevice->Erase(DATA_TYPE_FREQUENT);
    }
    printf("Success Rate:%f\r\n", (TEST_NUM - faile_num)/1000.0);
    printf("Flash Device Test 1 Ending...\r\n\r\n");
}

/**********************************************************************
 * 函数名称： unit_main_test
 * 功能描述： 单元测试主函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void unit_test_main(void)
{
    // // flash驱动接口测试
    // flash_driver_test();

    // flash设备接口测试
    flash_device_test();
}

#endif /* UNIT_TESTING_ENABLED */
#endif /* FLASH_TEST_FUNCTION */
