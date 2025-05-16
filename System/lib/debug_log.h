#ifndef DEBUG_LOG_H_
#define DEBUG_LOG_H_
#include <stdint.h>
#include <string.h>

/**
 * @brief   尽可能确保显示的终端支持ANSI的转移序列以及UTF-8的显示
 * @note    当终端不支持ANSI的转移序列，则无法正常显示颜色信息，
 *          可以通过设置宏COLOR_ENABLE为0关闭颜色输出
 * @note    当终端不支持UTF-8，则关于中文的显示会乱码
*/

/************************** 用户自定义宏 *********************************/
/**
 * @brief   调试输出总开关
 * @note    处于禁用状态时关闭所有输出
*/
#define DBG_ENABLE          1

/**
 * @brief   是否启用颜色输出
 * @note    如果显示的终端不支持ANSI转义序列，可以将此项设置为0以关闭颜色输出
*/
#define COLOR_ENABLE        1

/**
 * @brief   指定 输出打印等级
 * @note    1. 只会打印当前等级及其以上的等级的信息
 * @note    2. 假设 指定的打印等级 = DBG_LOG_WARNING,
 *          则只会打印 DBG_LOG_WARNING 和 DBG_LOG_ERROR两个等级的信息
*/
#define DBG_LOG_LEVEL       DBG_LOG_DEBUG


/*************************** 调试输出保留宏 ********************************/
/**
 * @brief   输出的等级从高到低的排序: [错误] > [警告] > [普通] > [调试]
*/
// [错误]输出等级 
#define DBG_LOG_ERROR       1
// [警告]输出等级
#define DBG_LOG_WARNING     2
// [普通]输出等级
#define DBG_LOG_INFO        3
// [调试]输出等级
#define DBG_LOG_DEBUG       4

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"

#if COLOR_ENABLE
#define PRINT_ANSI_COLOR(...) printf(__VA_ARGS__)
#else
#define PRINT_ANSI_COLOR(...)
#endif


// 调试输出总开关 处于打开状态
#if DBG_ENABLE
// 日志打印格式
#define DBG_LOG(color, ...)                 \
        PRINT_ANSI_COLOR(color);            \
        printf("[%s]: ", __func__);         \
        printf(__VA_ARGS__);                \
        PRINT_ANSI_COLOR(ANSI_COLOR_RESET); \
        printf("\n");                       

// [调试]等级控制
#if DBG_LOG_LEVEL >= DBG_LOG_DEBUG
#define DBG_LOGD(...) DBG_LOG(ANSI_COLOR_BLUE, __VA_ARGS__);
#else
#define DBG_LOGD(...)
#endif

// [普通]等级控制
#if DBG_LOG_LEVEL >= DBG_LOG_INFO
#define DBG_LOGI(...) DBG_LOG(ANSI_COLOR_GREEN, __VA_ARGS__);
#else
#define DBG_LOGI(...)
#endif

// [警告]等级控制
#if DBG_LOG_LEVEL >= DBG_LOG_WARNING
#define DBG_LOGW(...) DBG_LOG(ANSI_COLOR_YELLOW, __VA_ARGS__);
#else
#define DBG_LOGW(...)
#endif

// [错误]等级控制
#if DBG_LOG_LEVEL >= DBG_LOG_ERROR
#define DBG_LOGE(...) DBG_LOG(ANSI_COLOR_RED, __VA_ARGS__);
#else
#define DBG_LOGE(...)
#endif
#else
#define DBG_LOGD(...)
#define DBG_LOGI(...)
#define DBG_LOGW(...)
#define DBG_LOGE(...)
#endif

/**
 * @brief   自定义打印
 * @note    不受级别控制，用于打印系统开机提示等关键信息
 * @note    第一个参数传入颜色，第二个参数传入需要打印的信息
 * @note    最好传入两个宏以避免警告
*/
#define ADVANCED_LOG(color, ...) \
    PRINT_ANSI_COLOR(color); \
    printf(__VA_ARGS__); \
    PRINT_ANSI_COLOR(ANSI_COLOR_RESET); \
    printf("\n");

#endif
