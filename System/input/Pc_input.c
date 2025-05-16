#include <string.h>
#include <kal_time.h>
#include "Pc_input.h"
#include "input_system.h"
#include "input_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef void (*PcInputProcessCallback)(char c);
extern void SetPcInputProcessCallback(PcInputProcessCallback func);
static char g_PCDataBuff[INTPU_BUF_LEN];            // 输入缓冲区
static int g_DataBuffIndex = 0;                     // 输入缓冲区索引

/**********************************************************************
 * 函数名称： parse_at_command
 * 功能描述： 解析AT指令
 * 输入参数： at_cmd-AT指令字符串
 * 输出参数： function-功能部分
 *            param-参数部分
 * 返 回 值： true-成功, false-失败
 ***********************************************************************/ 
bool parse_at_command(const char* at_cmd, char* function, char* param) {
    // 检查输入指针是否有效
    if (at_cmd == NULL || function == NULL || param == NULL) {
        return false;
    }
    
    // 检查是否以"AT+"开头
    if (strncmp(at_cmd, "AT+", 3) != 0) {
        return false;
    }
    
    const char* p = at_cmd + 3; // 跳过"AT+"部分
    const char* equal_pos = strchr(p, '=');
    
    // 检查是否有'='字符
    if (equal_pos == NULL) {
        return false;
    }
    
    // 检查是否以'.'结尾
    if (p[strlen(p) - 1] != '.') {
        return false;
    }
    
    // 提取功能部分（'='之前的内容）
    size_t func_len = equal_pos - p;
    if (func_len == 0) {
        return false; // 功能部分不能为空
    }
    strncpy(function, p, func_len);
    function[func_len] = '\0'; // 确保字符串结尾
    
    // 提取参数部分（'='和'.'之间的内容）
    const char* param_start = equal_pos + 1;
    size_t param_len = strlen(p) - (param_start - p) - 1; // 减去'.'的位置
    
    if (param_len == 0) {
        return false; // 参数部分不能为空
    }
    
    strncpy(param, param_start, param_len);
    param[param_len] = '\0'; // 确保字符串结尾
    
    return true;
}

/**********************************************************************
 * 函数名称： AtDataProcessCallback
 * 功能描述： AT指令数据回调函数: 得到AT指令数据,构造为InputEvent
 * 输入参数： c-AT指令数据
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/ 
static void AtDataProcessCallback(char c)
{
    char *buf = g_PCDataBuff;
    int i = g_DataBuffIndex;
    InputEvent event;

    buf[i] = c;
    g_DataBuffIndex++;

    if (buf[i] == '.')
    {
        if (parse_at_command(buf, event.function, event.param)) {
            printf("FUNC: %s\n", event.function);
            printf("PARAM: %s\n", event.param);

            /* 接收完数据 */
            /* 构造InputEvent并上报 */
            /* 构造InputEvent */
            event.time    = KAL_GetTime();  /* kal */
            event.eType = INPUT_EVENT_TYPE_PC;
            strncpy(event.str, buf, i+1);
            event.str[i+1] = '\0';
            
            /* 放入环形缓冲区 */
            PutInputEvent(&event);
        }

        g_DataBuffIndex = 0;
        memset(buf, 0, sizeof(g_PCDataBuff));
    }
}


/**********************************************************************
 * 函数名称： PcInputInit
 * 功能描述： 初始化PC输入设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0-成功, 其他值-失败
 ***********************************************************************/ 
int PcInputInit(void)
{
    /* 网络硬件相关的设置在网络系统里进行 */
    
    /* 给USART3设置中断函数提供一个回调函数 */
    SetPcInputProcessCallback(AtDataProcessCallback);
    
    return 0;
}

/* 为PC输入设备构造一个InputDevice */
static InputDevice g_tPcInputDevice = {
    "PC",
    NULL,
    PcInputInit,
    NULL,
};

/**********************************************************************
 * 函数名称： AddInputDevicePc
 * 功能描述： 注册PC输入设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void AddInputDevicePc(void)
{
    InputDeviceRegister(&g_tPcInputDevice);
}

