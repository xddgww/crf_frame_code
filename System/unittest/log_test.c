#include "unit_common.h"
#include "log_test.h"
#include "debug_log.h"

#ifdef LOG_TEST_FUNCTION
#if UNIT_TESTING_ENABLED


const char *log_level_string[] = 
{
    [DBG_LOG_ERROR]     = "Error",
    [DBG_LOG_WARNING]   = "Warning",
    [DBG_LOG_INFO]      = "Info",
    [DBG_LOG_DEBUG]     = "Debug",
};

/**********************************************************************
 * 函数名称： print_hex_table
 * 功能描述： 以16进制打印数据
 * 输入参数： uint8_t *data 打印的数据数据
 *           uint16_t len  数据长度 
 * 输出参数： 输出格式 = %02X
 * 返 回 值： 无
 ***********************************************************************/
void print_hex_table(uint8_t *data, uint16_t len)
{
#if DBG_ENABLE
    // 打印标题行
    PRINT_ANSI_COLOR(ANSI_COLOR_CYAN);
    printf("     ");
    for (size_t i = 0; i < 0x10; i++)
    {
        printf("%02X ", (unsigned int)i);
    }
    PRINT_ANSI_COLOR(ANSI_COLOR_RESET);
    printf("\n");
	// 计算行数
	size_t rows = (len + 15) / 16; 
	for (size_t i = 0; i < rows; i++)
	{
		// 输出标题列
        PRINT_ANSI_COLOR(ANSI_COLOR_CYAN);
        printf("%04X ", (unsigned int)i * 16);
        // 输出数据列
        PRINT_ANSI_COLOR(ANSI_COLOR_MAGENTA);
		size_t j;
		for (j = 0; j < 16 && i * 16 + j < len; j++)
		{
			printf("%02X ", data[i * 16 + j]);
		}
		// 补齐空白列
		for (; j < 16; j++)
		{
			printf("   ");
		}
		printf("\n");
        PRINT_ANSI_COLOR(ANSI_COLOR_RESET);
	}
#endif
}

/**********************************************************************
 * 函数名称： debug_log_demo
 * 功能描述： 日志打印测试
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void debug_log_demo(void)
{
#if DBG_ENABLE == 0
    printf("## Debug Print Disable, NO log print\n");
#else
    printf("Current log level is [%s]\n", log_level_string[DBG_LOG_LEVEL]);
#endif
    // 定义项目名称
    #define PROJECT_NAME_ART        " ____ ____ ____ ____ ____ ____ ____ ____ \n||L |||O |||G |||- |||D |||E |||M |||O ||\n||__|||__|||__|||__|||__|||__|||__|||__||\n|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|\n"
    // ** 自定义打印 不受级别控制
    ADVANCED_LOG(ANSI_COLOR_YELLOW, PROJECT_NAME_ART);
    // 中文打印测试
    DBG_LOGI("中文测试输出, 需要确保终端支持UTF-8的显示, 否则显示乱码\n");
    // 输出所有等级测试，可以尝试修改打印等级的宏以观察输出效果 DBG_LOG_LEVEL
    DBG_LOGD("The Log Level is %s", log_level_string[DBG_LOG_DEBUG]);
    DBG_LOGI("The Log Level is %s", log_level_string[DBG_LOG_INFO]);
    DBG_LOGW("The Log Level is %s", log_level_string[DBG_LOG_WARNING]);
    DBG_LOGE("The Log Level is %s", log_level_string[DBG_LOG_ERROR]);
    // 打印16进制
    uint8_t array[100] = {0};
    for (size_t i = 0; i < sizeof(array); i++)
    {
        array[i] = i;
    }
    print_hex_table(array, sizeof(array));
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
    // 日志测试函数
    debug_log_demo();
}
#endif // UNIT_TESTING_ENABLED
#endif // LOG_TEST_FUNCTION
