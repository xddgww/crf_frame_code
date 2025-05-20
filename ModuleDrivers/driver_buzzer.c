#include "driver_buzzer.h"
#include "tim.h"
#include "stdio.h"

/**********************************************************************
 * 函数名称： HAL_BuzzerDeviceInit
 * 功能描述： 初始化蜂鸣器设备
 * 输入参数： BuzzerDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerDeviceInit(struct BuzzerDevice *ptBuzzerDevice)
{
    return 0;
}

/**********************************************************************
 * 函数名称： HAL_BuzzerDeviceControl
 * 功能描述： 控制蜂鸣器开关
 * 输入参数： BuzzerDevice指针，控制状态
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerDeviceControl(struct BuzzerDevice *ptBuzzerDevice, int iStatus)
{
    if (ptBuzzerDevice == NULL) {
        return -1;
    }

    if ((ptBuzzerDevice->iStatus == BUZZER_OFF) && (iStatus == BUZZER_ON)) {
        HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
        ptBuzzerDevice->iStatus = BUZZER_ON;
    } else if ((ptBuzzerDevice->iStatus == BUZZER_ON) && (iStatus == BUZZER_OFF)) {
        HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_2);
        ptBuzzerDevice->iStatus = BUZZER_OFF;
    }

    return 0;
}

/**********************************************************************
 * 函数名称： HAL_BuzzerSetVolume
 * 功能描述： 设置蜂鸣器音量
 * 输入参数： BuzzerDevice指针，音量值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerSetVolume(struct BuzzerDevice *ptBuzzerDevice, int iVolume)
{
    if (ptBuzzerDevice == NULL) {
        return -1;
    }

    uint32_t pulse = (htim8.Instance->ARR + 1) * iVolume / 100;         // 计算CCR值（duty范围0-100%）
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, pulse);                // 动态修改CCR
    ptBuzzerDevice->iVolume = iVolume;                                  // 更新音量值

    printf("BuzzerSetVolume: %d\n", iVolume);
    return 0;
}

/**********************************************************************
 * 函数名称： HAL_BuzzerSetFrequency
 * 功能描述： 设置蜂鸣器频率
 * 输入参数： BuzzerDevice指针，频率值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerSetFrequency(struct BuzzerDevice *ptBuzzerDevice, uint16_t iFrequency)
{
    if (ptBuzzerDevice == NULL) {
        return -1;
    }

    
    uint32_t arr = 1000000 / iFrequency - 1;                            // 计算自动重装载值和占空比
    uint32_t psc = 150 - 1;                                             // 保持预分频器不变
    
    __HAL_TIM_SET_AUTORELOAD(&htim8, arr);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, arr / 2); // 50%占空比
    return 0;
}
