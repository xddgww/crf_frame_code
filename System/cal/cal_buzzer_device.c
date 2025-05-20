#include "cal_buzzer_device.h"
#include "driver_buzzer.h"

/**********************************************************************
 * 函数名称： CAL_BuzzerDeviceInit
 * 功能描述： 初始化蜂鸣器设备
 * 输入参数： BuzzerDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int CAL_BuzzerDeviceInit(struct BuzzerDevice *ptBuzzerDevice)
{
    return HAL_BuzzerDeviceInit(ptBuzzerDevice);
}


/**********************************************************************
 * 函数名称： CAL_BuzzerDeviceControl
 * 功能描述： 控制蜂鸣器开关
 * 输入参数： BuzzerDevice指针，控制状态
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int CAL_BuzzerDeviceControl(struct BuzzerDevice *ptBuzzerDevice, int iStatus)
{
    return HAL_BuzzerDeviceControl(ptBuzzerDevice, iStatus);
}


/**********************************************************************
 * 函数名称： CAL_BuzzerSetVolume
 * 功能描述： 设置蜂鸣器音量
 * 输入参数： BuzzerDevice指针，音量值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int CAL_BuzzerSetVolume(struct BuzzerDevice *ptBuzzerDevice, int iVolume)
{
    return HAL_BuzzerSetVolume(ptBuzzerDevice, iVolume);;
}


/**********************************************************************
 * 函数名称： CAL_BuzzerSetFrequency
 * 功能描述： 设置蜂鸣器频率
 * 输入参数： BuzzerDevice指针，频率值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
int CAL_BuzzerSetFrequency(struct BuzzerDevice *ptBuzzerDevice, uint16_t iFrequency)
{
    return HAL_BuzzerSetFrequency(ptBuzzerDevice, iFrequency);
}