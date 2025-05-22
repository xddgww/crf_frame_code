#include <stdio.h>
#include "unit_common.h"
#include "input_test.h"
#include "input_system.h"
#include "input_buffer.h"

#ifdef INPUT_TEST_FUNCTION
#if UNIT_TESTING_ENABLED
/**********************************************************************
 * 函数名称： input_test
 * 功能描述： 输入系统单元测试函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void input_test(void)
{
	InputEvent event;
	
	AddInputDevices();
	InitInputDevices();
	while (1)
	{
		if (GetInputEvent(&event) == 0)
		{
			printf("get input event:\r\n");
			printf("type: %d\r\n", event.eType);
			printf("time: %d\r\n", event.time);
			printf("function: %s\r\n", event.function);
			printf("param: %s\r\n", event.param);
			printf("str: %s\r\n", event.str);
			// printf("key : %d\r\n", event.iKey);
			// printf("pressure : %d\r\n", event.iPressure);
		} 
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
    // 输入系统单元测试函数
    input_test();
}
#endif /* UNIT_TESTING_ENABLED */
#endif /* INPUT_TEST_FUNCTION */
