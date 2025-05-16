#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "input_buffer.h"

// 是否使用RTOS队列
#define USE_RTOS_QUEUE 0	// 1-使用RTOS队列，0-不使用

#if USE_RTOS_QUEUE
#include "FreeRTOS.h"
#include "queue.h"
static QueueHandle_t g_xQueueInput;
#else
#define BUFFER_SIZE 10        /* 环形缓冲区的大小 */
typedef struct
{
    InputEvent buffer[BUFFER_SIZE];  /* 缓冲区空间 */
    volatile unsigned int pW;           /* 写地址 */
    volatile unsigned int pR;           /* 读地址 */
} InputEventBuffer;

static InputEventBuffer g_tInputBuffer;
#endif

/**********************************************************************
 * 函数名称： InitInputQueue
 * 功能描述： 初始化环形缓冲区
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0-成功, 其他值-失败
 ***********************************************************************/ 
int InitInputQueue(void)
{
#if USE_RTOS_QUEUE
	g_xQueueInput = xQueueCreate(BUFFER_SIZE, sizeof(InputEvent));
	if (g_xQueueInput == NULL)
	{
		printf("can not create queue\r\n");
		return -1;
	}
	else
	{
		return 0;
	}
#else
    g_tInputBuffer.pR = 0;
	g_tInputBuffer.pW = 0;
    memset(g_tInputBuffer.buffer, 0, BUFFER_SIZE);
	return 0;
#endif
	
}

/**********************************************************************
 * 函数名称： PutInputEvent
 * 功能描述： 往环形缓冲区放入数据
 * 输入参数： ptInputEvent-输入事件
 * 输出参数： 无
 * 返 回 值： 0-成功, 其他值-失败
 ***********************************************************************/ 
int PutInputEvent(PInputEvent ptInputEvent)
{
#if USE_RTOS_QUEUE
	/* 写队列 */
	xQueueSendFromISR(g_xQueueInput, ptInputEvent, NULL);
	return 0;
#else
    int i = (g_tInputBuffer.pW + 1) % BUFFER_SIZE;

	/* 防御式编程 */
	if (!ptInputEvent)
		return -1;
	
    if(i != g_tInputBuffer.pR)    // 环形缓冲区没有写满
    {
        g_tInputBuffer.buffer[g_tInputBuffer.pW] = *ptInputEvent;
        g_tInputBuffer.pW = i;
		return 0;
    }
	return -1;
#endif
}

/**********************************************************************
 * 函数名称： GetInputEvent
 * 功能描述： 从环形缓冲区读取数据
 * 输入参数： 无
 * 输出参数： ptInputEvent-用来保存输入事件
 * 返 回 值： 0-成功, 其他值-失败
 ***********************************************************************/ 
int GetInputEvent(PInputEvent ptInputEvent)
{
#if USE_RTOS_QUEUE
	/* 读队列 */
	xQueueReceive(g_xQueueInput, ptInputEvent, portMAX_DELAY);
	return 0;
#else	
	/* 防御式编程 */
	if (!ptInputEvent)
		return -1;

	/* 环形缓冲区空, 则返回-1 */
    if(g_tInputBuffer.pR == g_tInputBuffer.pW)
    {
        return -1;
    }
    else
    {
        *ptInputEvent = g_tInputBuffer.buffer[g_tInputBuffer.pR];
        g_tInputBuffer.pR = (g_tInputBuffer.pR + 1) % BUFFER_SIZE;
        return 0;
    }
#endif
}

