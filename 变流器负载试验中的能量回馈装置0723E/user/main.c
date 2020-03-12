#include "stm32f10x.h"
#include "InputCap.h"
#include "OLED.h"
#include "AD.h"
#include "PWM.h"
#include "Update_Interrupt.h"
#include "AD_DMA.h"
#include "Key_Scan.h"
#include "iwdg.h"


int main()
{
     extern int AD_DMA_Result1,AD_DMA_Result2;    //电压有效值，电流有效值
     extern  float SPWM_m;           //pwm.c 调制比
     extern int BoostDownMosDuty;    //Boost下管占空比
     
     int ShowCounter = 0;
     /*IO口模拟50HzPWM引脚设置 PA1 PA2*/
//     GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
     
     iwdg_init();   //独立看门狗初始化配置
     
     /*OLED初始化 PB7 PB6 PB5 PD2 PC12 */
     OLED_Init();
	OLED_Clear();
    // OLED_ShowString(0,0,(unsigned char *)"u = 0.00");
    
     /*键盘初始化 PA12 PA11 PA10 PA9   PC9 PC8 PC6 PC5*/
     Key_Scan_Init();
    
     /*输入捕获初始化 PB8 PB9*/
//     TIM4_Cap_Init();
    
    /*TIM8  CH2 PWM初始化 PC7 PB0*/
     time8_gpio(); 
     time8_init(0.5);
     /*TIM2 CH4 PWM初始化 PA3*/
     time2_init(0.5);
     /*ADC_DMA初始化 PA4 PA5 PA6 */
     ADC1_GPIO_Config();
	ADC1_Mode_Config();

     
      while(1)
      {
           IWDG_ReloadCounter();	   //喂狗的时间是1.28s
           Key_Scan();
           
//           Sum_AD_DMA_Result1 +=AD_DMA_Result1;
           
           if(ShowCounter == 11500)
           {
//                Ave_AD_DMA_Result1 = Sum_AD_DMA_Result1 >> 4;
//               OLED_ShowNum(0,6,Ave_AD_DMA_Result1,4,16);
           ShowCounter = 0;
//                Sum_AD_DMA_Result1 = 0;
         //  OLED_ShowNum(0,6,AD_DMA_Result1,4,16);
           OLED_ShowNum(0,6,(int)(SPWM_m*1000),4,16);
           OLED_ShowNum(64,6,BoostDownMosDuty,4,16);
           }
           ShowCounter++ ;
                                       
//         
                

           
      }
	
}
