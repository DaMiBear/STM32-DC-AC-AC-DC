#include "iwdg.h"

/*******************************************************************************
* 函 数 名         : iwdg_init
* 函数功能		   : 独立看门狗模式配置初始化	   
* 输    入         : 无
* 输    出         : 无     
*******************************************************************************/
void iwdg_init()	//独立看门狗初始化配置
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能寄存器，写功能
	IWDG_SetPrescaler(IWDG_Prescaler_64);//设置IWDG预分频值//40K/64=1.6ms
	IWDG_SetReload(800);//设置IWDG重装载值  12位的（0~4095）//800*1.6ms=1.28s
	IWDG_ReloadCounter();//按照IWDG重装载寄存器的值重装载IWDG计数器
	IWDG_Enable();//使能IWDG
}
