#include "buzzer_test.h"
#include "unit_common.h"
#include "buzzer_device.h"
#include "stdio.h"

#ifdef BUZZER_TEST_FUNCTION
#if UNIT_TESTING_ENABLED
/**********************************************************************
 * 函数名称： buzzer_test
 * 功能描述： 蜂鸣器测试函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void buzzer_test(void)
{
    // 获取蜂鸣器设备
    PBuzzerDevice ptBuzzer = GetBuzzerDevice();
    if (ptBuzzer) {
        // 初始化
        ptBuzzer->Init(ptBuzzer);
        
        // 设置音量70%
        ptBuzzer->SetVolume(ptBuzzer, 70);
        
        // 蜂鸣500ms
        ptBuzzer->Beep(ptBuzzer, 500);
        
        // 蜂鸣3次，每次200ms，间隔100ms
        ptBuzzer->BeepPattern(ptBuzzer, 3, 200, 100);
        
        // 播放简单旋律
        int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523}; // C4到C5
        int durations[] = {200, 200, 200, 200, 200, 200, 200, 400};
        ptBuzzer->PlayTune(ptBuzzer, frequencies, durations, 8);
    }
}

/**********************************************************************
 * 函数名称： unit_main_test
 * 功能描述： 单元测试主函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void unit_test_main(void)
{
    // 蜂鸣器测试函数调用
    buzzer_test();
}
#endif /* UNIT_TESTING_ENABLED */
#endif /* BUZZER_TEST_FUNCTION */
