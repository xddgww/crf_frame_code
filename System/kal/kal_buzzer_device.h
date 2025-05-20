#ifndef __KAL_BUZZER_DEVICE_H
#define __KAL_BUZZER_DEVICE_H

#include "buzzer_device.h"

/**********************************************************************
 * 函数名称： KAL_BuzzerDeviceInit
 * 功能描述： 初始化蜂鸣器设备
 * 输入参数： BuzzerDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_BuzzerDeviceInit(struct BuzzerDevice *ptBuzzerDevice);

/**********************************************************************
 * 函数名称： KAL_BuzzerDeviceControl
 * 功能描述： 控制蜂鸣器开关
 * 输入参数： BuzzerDevice指针，控制状态
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_BuzzerDeviceControl(struct BuzzerDevice *ptBuzzerDevice, int iStatus);

/**********************************************************************
 * 函数名称： KAL_BuzzerSetVolume
 * 功能描述： 设置蜂鸣器音量
 * 输入参数： BuzzerDevice指针，音量值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_BuzzerSetVolume(struct BuzzerDevice *ptBuzzerDevice, int iVolume);

/**********************************************************************
 * 函数名称： KAL_BuzzerSetFrequency
 * 功能描述： 设置蜂鸣器频率
 * 输入参数： BuzzerDevice指针，频率值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int KAL_BuzzerSetFrequency(struct BuzzerDevice *ptBuzzerDevice, uint16_t iFrequency);

#endif /* __KAL_BUZZER_DEVICE_H */
