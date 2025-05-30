#ifndef __UNIT_COMMON_H
#define __UNIT_COMMON_H

#define UNIT_TESTING_ENABLED    1 // 单元测试开关定义, 1为打开, 0为关闭

// 单元测试宏定义
// #define INPUT_TEST_FUNCTION 
// #define LOG_TEST_FUNCTION 
// #define BUZZER_TEST_FUNCTION
#define FLASH_TEST_FUNCTION

#if UNIT_TESTING_ENABLED
/**********************************************************************
 * 函数名称： unit_main_test
 * 功能描述： 单元测试主函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void unit_test_main(void);
#endif /* UNIT_TESTING_ENABLED */
#endif /* __UNIT_COMMON_H */


