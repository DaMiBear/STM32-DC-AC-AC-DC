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
     extern int AD_DMA_Result1,AD_DMA_Result2;    //��ѹ��Чֵ��������Чֵ
     extern  float SPWM_m;           //pwm.c ���Ʊ�
     extern int BoostDownMosDuty;    //Boost�¹�ռ�ձ�
     
     int ShowCounter = 0;
     /*IO��ģ��50HzPWM�������� PA1 PA2*/
//     GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
     
     iwdg_init();   //�������Ź���ʼ������
     
     /*OLED��ʼ�� PB7 PB6 PB5 PD2 PC12 */
     OLED_Init();
	OLED_Clear();
    // OLED_ShowString(0,0,(unsigned char *)"u = 0.00");
    
     /*���̳�ʼ�� PA12 PA11 PA10 PA9   PC9 PC8 PC6 PC5*/
     Key_Scan_Init();
    
     /*���벶���ʼ�� PB8 PB9*/
//     TIM4_Cap_Init();
    
    /*TIM8  CH2 PWM��ʼ�� PC7 PB0*/
     time8_gpio(); 
     time8_init(0.5);
     /*TIM2 CH4 PWM��ʼ�� PA3*/
     time2_init(0.5);
     /*ADC_DMA��ʼ�� PA4 PA5 PA6 */
     ADC1_GPIO_Config();
	ADC1_Mode_Config();

     
      while(1)
      {
           IWDG_ReloadCounter();	   //ι����ʱ����1.28s
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
