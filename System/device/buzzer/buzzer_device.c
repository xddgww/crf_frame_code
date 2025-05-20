#include "buzzer_device.h"
#include "kal_buzzer_device.h"
#include "kal_time.h"

/**********************************************************************
 * 函数名称： BuzzerDeviceInit
 * 功能描述： 初始化蜂鸣器设备
 * 输入参数： BuzzerDevice指针
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerDeviceInit(struct BuzzerDevice *ptBuzzerDevice)
{
    ptBuzzerDevice->iStatus = BUZZER_OFF;
    ptBuzzerDevice->iVolume = BUZZER_VOL_DEFAULT;
    ptBuzzerDevice->iFrequency = BUZZER_FREQ_DEFAULT;
    return KAL_BuzzerDeviceInit(ptBuzzerDevice);
}

/**********************************************************************
 * 函数名称： BuzzerDeviceControl
 * 功能描述： 控制蜂鸣器开关
 * 输入参数： BuzzerDevice指针，控制状态
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerDeviceControl(struct BuzzerDevice *ptBuzzerDevice, int iStatus)
{
    return KAL_BuzzerDeviceControl(ptBuzzerDevice, iStatus);
}

/**********************************************************************
 * 函数名称： BuzzerSetVolume
 * 功能描述： 设置蜂鸣器音量
 * 输入参数： BuzzerDevice指针，音量值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerSetVolume(struct BuzzerDevice *ptBuzzerDevice, int iVolume)
{
    if (iVolume < BUZZER_VOL_MIN || iVolume > BUZZER_VOL_MAX) {
        return -1;
    }
    
    return KAL_BuzzerSetVolume(ptBuzzerDevice, iVolume);
}

/**********************************************************************
 * 函数名称： BuzzerSetFrequency
 * 功能描述： 设置蜂鸣器频率
 * 输入参数： BuzzerDevice指针，频率值
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerSetFrequency(struct BuzzerDevice *ptBuzzerDevice, uint16_t iFrequency)
{
    if (iFrequency < BUZZER_FREQ_MIN || iFrequency > BUZZER_FREQ_MAX) {
        return -1;
    }
    
    return KAL_BuzzerSetFrequency(ptBuzzerDevice, iFrequency);
}

/**********************************************************************
 * 函数名称： BuzzerBeep
 * 功能描述： 蜂鸣器鸣叫
 * 输入参数： BuzzerDevice指针，鸣叫持续时间(单位：ms)
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerBeep(struct BuzzerDevice *ptBuzzerDevice, int iDurationMs)
{
    if (iDurationMs <= 0) {
        return -1;
    }
    
    int ret = BuzzerDeviceControl(ptBuzzerDevice, BUZZER_ON);
    if (ret != 0) {
        return ret;
    }
    
    KAL_Delay(iDurationMs);
    
    return BuzzerDeviceControl(ptBuzzerDevice, BUZZER_OFF);
}

/**********************************************************************
 * 函数名称： BuzzerBeepPattern
 * 功能描述： 蜂鸣器鸣叫模式
 * 输入参数： BuzzerDevice指针，鸣叫次数，鸣叫持续时间(单位：ms)，静默时间(单位：ms)
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerBeepPattern(struct BuzzerDevice *ptBuzzerDevice, 
                           int iBeepTimes, int iBeepDurationMs, int iSilentDurationMs)
{
    if (iBeepTimes <= 0 || iBeepDurationMs <= 0 || iSilentDurationMs < 0) {
        return -1;
    }

    for (int i = 0; i < iBeepTimes; i++) {
        int ret = BuzzerBeep(ptBuzzerDevice, iBeepDurationMs);
        if (ret != 0) {
            return ret;
        }
        
        if (i < iBeepTimes - 1 && iSilentDurationMs > 0) {
            KAL_Delay(iSilentDurationMs);
        }
    }

    return 0;
}

/**********************************************************************
 * 函数名称： BuzzerPlayTune
 * 功能描述： 蜂鸣器播放音调
 * 输入参数： BuzzerDevice指针，音调频率数组，音调持续时间数组，音调个数
 * 返 回 值： 0：成功；其他：失败
 ***********************************************************************/
static int BuzzerPlayTune(struct BuzzerDevice *ptBuzzerDevice, 
                         const int *pFrequencies, const int *pDurations, int iNumNotes)
{
    if (!pFrequencies || !pDurations || iNumNotes <= 0) {
        return -1;
    }
    
    for (int i = 0; i < iNumNotes; i++) {
        int ret = BuzzerSetFrequency(ptBuzzerDevice, pFrequencies[i]);
        if (ret != 0) {
            return ret;
        }
        
        ret = BuzzerBeep(ptBuzzerDevice, pDurations[i]);
        if (ret != 0) {
            return ret;
        }
    }
    
    return 0;
}

static BuzzerDevice g_tBuzzerDevice = {
    BUZZER_OFF, 
    BUZZER_VOL_DEFAULT, 
    BUZZER_FREQ_DEFAULT, 
    BuzzerDeviceInit, 
    BuzzerDeviceControl, 
    BuzzerSetVolume, 
    BuzzerSetFrequency,
    BuzzerBeep,
    BuzzerBeepPattern,
    BuzzerPlayTune
};

/**********************************************************************
 * 函数名称： GetBuzzerDevice
 * 功能描述： 获取唯一的蜂鸣器设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： BuzzerDevice指针
 ***********************************************************************/
PBuzzerDevice GetBuzzerDevice(void)
{
    return &g_tBuzzerDevice;
}