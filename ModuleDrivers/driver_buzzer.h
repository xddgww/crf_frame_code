#ifndef __DRIVER_BUZZER_H
#define __DRIVER_BUZZER_H

#include "buzzer_device.h"

/**********************************************************************
 * 函数名称： HAL_BuzzerDeviceInit
 * 功能描述： 初始化蜂鸣器设备
 * 输入参数： BuzzerDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerDeviceInit(struct BuzzerDevice *ptBuzzerDevice);

/**********************************************************************
 * 函数名称： HAL_BuzzerDeviceControl
 * 功能描述： 控制蜂鸣器开关
 * 输入参数： BuzzerDevice指针，控制状态
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerDeviceControl(struct BuzzerDevice *ptBuzzerDevice, int iStatus);

/**********************************************************************
 * 函数名称： HAL_BuzzerSetVolume
 * 功能描述： 设置蜂鸣器音量
 * 输入参数： BuzzerDevice指针，音量值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerSetVolume(struct BuzzerDevice *ptBuzzerDevice, int iVolume);

/**********************************************************************
 * 函数名称： HAL_BuzzerSetFrequency
 * 功能描述： 设置蜂鸣器频率
 * 输入参数： BuzzerDevice指针，频率值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int HAL_BuzzerSetFrequency(struct BuzzerDevice *ptBuzzerDevice, uint16_t iFrequency);

#endif /* __DRIVER_BUZZER_H */
