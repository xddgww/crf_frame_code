#ifndef __INPUT_SYSTEM_H
#define __INPUT_SYSTEM_H

#ifndef NULL
#define NULL (void *)0
#endif

#define TIME_T int              // 时间类型定义
#define INTPU_BUF_LEN 50        // 输入缓冲区长度

typedef enum
{
    INPUT_EVENT_TYPE_PC,        // 终端信息输入
    INPUT_EVENT_TYPE_RF,        // 红外射频信息输入
    INPUT_EVENT_TYPE_KEY,       // 按键信息输入
    INPUT_EVENT_TYPE_VCC,       // 电源电压信息输入
    INPUT_EVENT_TYPE_MOTOR      // 电机信息输入
}INPUT_EVENT_TYPE;

typedef struct InputEvent {
    TIME_T time;
    INPUT_EVENT_TYPE eType;
    char function[10];
    char param[10];
    // int iX;
    // int iY;
    // int iKey;
    // int iPressure;
    char str[INTPU_BUF_LEN];
}InputEvent, *PInputEvent;

typedef struct InputDevice {
    char *name;
    int (*GetInputEvent)(PInputEvent ptInputEvent);
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    struct InputDevice *pNext;
}InputDevice, *PInputDevice;


/**********************************************************************
 * 函数名称： AddInputDevices
 * 功能描述： 注册多个输入设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void AddInputDevices(void);


/**********************************************************************
 * 函数名称： InitInputDevices
 * 功能描述： 初始化所有的输入设备
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void InitInputDevices(void);

/**********************************************************************
 * 函数名称： InputDeviceRegister
 * 功能描述： 注册一个输入设备
 * 输入参数： ptInputDevice-输入设备
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void InputDeviceRegister(PInputDevice ptInputDevice);


#endif /* __INPUT_SYSTEM_H */



