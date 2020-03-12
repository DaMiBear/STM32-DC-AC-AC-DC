#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
//主函数使用如下(包含stm32f10x_adc.h)
//int main()
//{
//  ADC1_Init
//  read_ADC();
//  ADC2_Init();
//  OLED_ShowNum(32,0,AD_a,1,12);            // y  x   number length    bits
//          OLED_ShowNum(48,0,AD_b,1,12);
//          OLED_ShowNum(56,0,AD_c,1,12);
//          OLED_ShowNum(65,0,AD_d,1,12);
//          delay(10);
//return 0;
//}
#define uint unsigned int 
#define uchar unsigned char
 //在连接端口改变时，需要改变一下宏定义   
#define   AD_GPIO		       GPIOA		//ADC使用的IO
#define   AD1                   GPIO_Pin_4	//IO引脚
#define   AD2                   GPIO_Pin_5
#define   AD_Channel		  ADC_Channel_4//对应通道
#define   AD2_Channel           ADC_Channel_5
void ADC1_Init(void);
void ADC2_Init(void);
void read_ADC(void);

#endif
