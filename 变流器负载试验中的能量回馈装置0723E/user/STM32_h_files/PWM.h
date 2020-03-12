#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"
//主函数中使用如下(需包含stm32f10x_tim.c)
//int main()
//{
//	
//	time_gpio();
//	time1_init(0.5);
//  time2_init(0.60);

//return 0;
//}
void time8_init(float D);
void time8_gpio(void);
void time2_init(float D);
void Tim8_Delay_ms(int ms);
int Compute_SPWM_TABLE(int k);

#define			TIM8_OCInit						TIM_OC2Init	//TIM8通道2 PC7
#define			TIM8_OCPreload						TIM_OC2PreloadConfig	//PB0			
#define			TIM2_OCInit						TIM_OC4Init	//TIM2通道4
#define			TIM2_OCPreload						TIM_OC4PreloadConfig
#define			GPIO_Clock_PWM1					RCC_APB2Periph_GPIOC	//TIM8 通道2  对应引脚
#define			GPIO_Clock_PWM1N					RCC_APB2Periph_GPIOB	//TIM8 互补输出对应引脚
#define			GPIO_Clock_PWM2					RCC_APB2Periph_GPIOA	//TIM2 通道4  对应引脚
#define			GPIO_PWM1							GPIOC
#define			GPIO_PWM1N						GPIOB
#define			GPIO_PWM2							GPIOA
#define			GPIO_Pin_PWM1						GPIO_Pin_7
#define			GPIO_Pin_PWM1N						GPIO_Pin_0
#define 		     GPIO_Pin_PWM2						GPIO_Pin_3

#endif
