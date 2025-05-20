#ifndef __BUZZER_DEVICE_H
#define __BUZZER_DEVICE_H

#include <stdint.h>

/* 蜂鸣器状态定义 */
#define BUZZER_OFF        0
#define BUZZER_ON         1

/* 蜂鸣器音量范围 */
#define BUZZER_VOL_MIN    0
#define BUZZER_VOL_MAX    100
#define BUZZER_VOL_DEFAULT 50

/* 蜂鸣器频率范围(Hz) */
#define BUZZER_FREQ_MIN   20
#define BUZZER_FREQ_MAX   20000
#define BUZZER_FREQ_DEFAULT 1000

typedef struct BuzzerDevice {
    int iStatus;        // 当前状态: BUZZER_OFF/BUZZER_ON
    int iVolume;        // 当前音量 (BUZZER_VOL_MIN-BUZZER_VOL_MAX)
    uint16_t iFrequency;     // 当前频率 (Hz)
    
    /* 初始化蜂鸣器设备, 成功则返回0 */
    int (*Init)(struct BuzzerDevice *ptBuzzerDevice);
    
    /* 控制蜂鸣器开关, iStatus取值: BUZZER_ON/BUZZER_OFF */
    int (*Control)(struct BuzzerDevice *ptBuzzerDevice, int iStatus);
    
    /* 设置蜂鸣器音量, iVolume取值: BUZZER_VOL_MIN-BUZZER_VOL_MAX */
    int (*SetVolume)(struct BuzzerDevice *ptBuzzerDevice, int iVolume);
    
    /* 设置蜂鸣器频率, iFrequency取值: BUZZER_FREQ_MIN-BUZZER_FREQ_MAX */
    int (*SetFrequency)(struct BuzzerDevice *ptBuzzerDevice, uint16_t iFrequency);
    
    /* 蜂鸣器发声指定时间, iDurationMs为持续时间(毫秒) */
    int (*Beep)(struct BuzzerDevice *ptBuzzerDevice, int iDurationMs);
    
    /* 蜂鸣器按指定模式发声 
     * iBeepTimes: 蜂鸣次数
     * iBeepDurationMs: 每次蜂鸣持续时间(毫秒)
     * iSilentDurationMs: 每次静音间隔时间(毫秒)
     */
    int (*BeepPattern)(struct BuzzerDevice *ptBuzzerDevice, 
                      int iBeepTimes, int iBeepDurationMs, int iSilentDurationMs);
    
    /* 播放指定频率的声音序列 
     * pFrequencies: 频率数组
     * pDurations: 持续时间数组(毫秒)
     * iNumNotes: 音符数量
     */
    int (*PlayTune)(struct BuzzerDevice *ptBuzzerDevice, 
                   const int *pFrequencies, const int *pDurations, int iNumNotes);
    
} BuzzerDevice, *PBuzzerDevice;

/**********************************************************************
 * 函数名称： GetBuzzerDevice
 * 功能描述： 获取唯一的蜂鸣器设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： BuzzerDevice指针
 ***********************************************************************/
PBuzzerDevice GetBuzzerDevice(void);

#endif /* __BUZZER_DEVICE_H */
