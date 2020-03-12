#include "InputCap.h"
#include "stm32f10x.h"
#include "OLED.h"
#include "PWM.h"       
void TIM4_Cap_Init()
{
     //结构体声明
     GPIO_InitTypeDef GPIO_InitStructure;
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
     TIM_ICInitTypeDef  TIM4_ICInitStructure;
     NVIC_InitTypeDef NVIC_InitStructureIC;
     //使能时钟TIM4和GPIOB
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //优先级分组方式:最高2位用于指定抢占式优先级，最低2位用于指定响应优先级。
     //设置端口为上拉输入模式
     GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
     GPIO_Init(GPIOB, &GPIO_InitStructure);
     GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);
     //定时器设置   1200kHz
     TIM_TimeBaseStructure.TIM_Period = 29; //设置计数值的最大值
     TIM_TimeBaseStructure.TIM_Prescaler = 9;     //计数寄存器的频率的分频系数
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //时钟分割
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数向上计数 
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  //初始化
     //定时器输入捕获设置 通道3
     TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3;             //选择通道3
     TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  //上升沿触发还是下降沿触发的设置
     TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TT1上
     TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;        //配置输入分频 不分频
     TIM4_ICInitStructure.TIM_ICFilter = 0xf;                     //滤波的问题 采样频率fSAMPLING=fDTS/32， N=8
     TIM_ICInit(TIM4, &TIM4_ICInitStructure);
     //通道2
     TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4;               
     TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
     TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
     TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       
     TIM4_ICInitStructure.TIM_ICFilter = 0xf; 
     TIM_ICInit(TIM4, &TIM4_ICInitStructure);
     //中断设置
     NVIC_InitStructureIC.NVIC_IRQChannel = TIM4_IRQn;  //TIM4发生中断
     NVIC_InitStructureIC.NVIC_IRQChannelPreemptionPriority = 2;  //先占中断等级
     NVIC_InitStructureIC.NVIC_IRQChannelSubPriority = 0;         //从中断等级
     NVIC_InitStructureIC.NVIC_IRQChannelCmd = ENABLE; //使能中断通道
     NVIC_Init(&NVIC_InitStructureIC);  //初始化 
     
     TIM_ITConfig(TIM4,TIM_IT_CC3 | TIM_IT_CC4,ENABLE);//使能各个中断
     TIM_Cmd(TIM4,ENABLE ); //使能定时器TIM4
     
     
}
extern int UpNum,DownNum;
//输入捕获---边沿中断程序
void TIM4_IRQHandler(void)
{
//	uint16_t count = 0;
     //CH3通道（PB8）下降沿发生触发事件
     if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) 
     {
     /*处理代码*/
//			count = TIM4->CNT;
//			 count = TIM_GetCounter(TIM4);
//          GPIO_SetBits(GPIOA,GPIO_Pin_1);
//          GPIO_ResetBits(GPIOA,GPIO_Pin_2);
         
     }
     //CH4通道（PB9）上升沿触发触发事件
    if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) 
    { 
     /* 处理代码 **/
//        GPIO_SetBits(GPIOA,GPIO_Pin_2);
//        GPIO_ResetBits(GPIOA,GPIO_Pin_1);
       
    }
     TIM_ClearITPendingBit(TIM4, TIM_IT_CC3 | TIM_IT_CC4); //清除中断标志
}
