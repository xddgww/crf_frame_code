#include "driver_usart3.h"
#include "usart.h"
#include "main.h"
#include "ring_buffer.h"
#include <stdio.h>

static uint8_t received_char;

// 配置printf打印功能，使用USART3
#if defined(__ICCARM__)
__ATTRIBUTES size_t __write(int, const unsigned char *, size_t);
#endif /* __ICCARM__ */

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __ICCARM__ */

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}


/**********************************************************************
 * 函数名称： EnableUART3IRQ
 * 功能描述： 使能UART3中断
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/ 
void EnableUART3IRQ(void)
{
    // 设置中断优先级
    // HAL_NVIC_SetPriority(USART3_IRQn, 14, 1);
    // HAL_NVIC_EnableIRQ(USART3_IRQn);
    
    // __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE); 

    // 启动串口接收中断
    HAL_UART_Receive_IT(&huart3, (uint8_t*)&received_char, 1);
}

typedef void (*PcInputProcessCallback)(char c);

static PcInputProcessCallback g_fPcInputProcessCallback;


/**********************************************************************
 * 函数名称： SetPcInputProcessCallback
 * 功能描述： 设置PC输入处理回调函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/ 
void SetPcInputProcessCallback(PcInputProcessCallback func)
{
    EnableUART3IRQ();
	g_fPcInputProcessCallback = func;
}

/**********************************************************************
 * 函数名称： HAL_UART_RxCpltCallback
 * 功能描述： UART3接收完成回调函数
 * 输入参数： huart-UART句柄
 * 输出参数： 无
 * 返 回 值： 无 
 ***********************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // extern ring_buffer *GetUART3RingBuffer(void);
    // static ring_buffer *uart3_ringbuffer = NULL;

    if(huart->Instance == USART3) {
        // if (!uart3_ringbuffer) {
        //     uart3_ringbuffer = GetUART3RingBuffer();
        // }

        // // 处理数据
        // ring_buffer_write(received_char, uart3_ringbuffer);
		if (g_fPcInputProcessCallback)
		{
			g_fPcInputProcessCallback(received_char);
		}

        // 重新启用接收
        HAL_UART_Receive_IT(&huart3, (uint8_t*)&received_char, 1);
    }
}

/**********************************************************************
 * 函数名称： USART3_SendBytes
 * 功能描述： 通过UART3发出多个数据
 * 输入参数： buf-数据缓冲区
 * 输入参数： len-数据长度
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void USART3_SendBytes(char *buf, int len)
{
    HAL_UART_Transmit_IT(&huart3, (uint8_t *)buf, len);
}
