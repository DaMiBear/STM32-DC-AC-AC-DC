#include "PWM.h"
#include "OLED.h"
#include "math.h"
#include "AD_DMA.h"
#define             PI		3.1415926
//SPWM表
volatile int  SPWM_TABLE_A[2000] = {
6, 17, 28, 39, 50, 61, 72, 83, 94, 106,
117, 128, 139, 150, 160, 171, 182, 193, 204, 215,
225, 236, 247, 257, 268, 278, 289, 299, 309, 319,
330, 340, 350, 360, 370, 380, 389, 399, 409, 418,
428, 437, 446, 455, 465, 474, 482, 491, 500, 509,
517, 526, 534, 542, 550, 558, 566, 574, 582, 589,
597, 604, 611, 618, 625, 632, 639, 646, 652, 658,
665, 671, 677, 683, 688, 694, 699, 705, 710, 715,
720, 724, 729, 733, 738, 742, 746, 750, 754, 757,
761, 764, 767, 770, 773, 776, 778, 781, 783, 785,
787, 789, 790, 792, 793, 794, 795, 796, 797, 797,
798, 798, 798, 798, 798, 797, 797, 796, 795, 794,
793, 792, 790, 789, 787, 785, 783, 781, 778, 776,
773, 770, 767, 764, 761, 757, 754, 750, 746, 742,
738, 733, 729, 724, 720, 715, 710, 705, 699, 694,
688, 683, 677, 671, 665, 658, 652, 646, 639, 632,
625, 618, 611, 604, 597, 589, 582, 574, 566, 558,
550, 542, 534, 526, 517, 509, 500, 491, 482, 474,
465, 455, 446, 437, 428, 418, 409, 399, 389, 380,
370, 360, 350, 340, 330, 319, 309, 299, 289, 278,
268, 257, 247, 236, 225, 215, 204, 193, 182, 171,
160, 150, 139, 128, 117, 106, 94, 83, 72, 61,
50, 39, 28, 17, 6, -6, -17, -28, -39, -50,
-61, -72, -83, -94, -106, -117, -128, -139, -150, -160,
-171, -182, -193, -204, -215, -225, -236, -247, -257, -268,
-278, -289, -299, -309, -319, -330, -340, -350, -360, -370,
-380, -389, -399, -409, -418, -428, -437, -446, -455, -465,
-474, -482, -491, -500, -509, -517, -526, -534, -542, -550,
-558, -566, -574, -582, -589, -597, -604, -611, -618, -625,
-632, -639, -646, -652, -658, -665, -671, -677, -683, -688,
-694, -699, -705, -710, -715, -720, -724, -729, -733, -738,
-742, -746, -750, -754, -757, -761, -764, -767, -770, -773,
-776, -778, -781, -783, -785, -787, -789, -790, -792, -793,
-794, -795, -796, -797, -797, -798, -798, -798, -798, -798,
-797, -797, -796, -795, -794, -793, -792, -790, -789, -787,
-785, -783, -781, -778, -776, -773, -770, -767, -764, -761,
-757, -754, -750, -746, -742, -738, -733, -729, -724, -720,
-715, -710, -705, -699, -694, -688, -683, -677, -671, -665,
-658, -652, -646, -639, -632, -625, -618, -611, -604, -597,
-589, -582, -574, -566, -558, -550, -542, -534, -526, -517,
-509, -500, -491, -482, -474, -465, -455, -446, -437, -428,
-418, -409, -399, -389, -380, -370, -360, -350, -340, -330,
-319, -309, -299, -289, -278, -268, -257, -247, -236, -225,
-215, -204, -193, -182, -171, -160, -150, -139, -128, -117,
-106, -94, -83, -72, -61, -50, -39, -28, -17, -6};

unsigned int ms_n = 0;  //延时

void Tim8_Delay_ms(int ms)
{
     int n = 0;
     ms_n = 0;
     n= ms/0.05;    //TIM1定时器每50us把ms_n +1
     while(ms_n != n);
}
//初始化定时器
void time8_init(float D)     					 //D---Duty
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
     NVIC_InitTypeDef NVIC_InitStructure;
	//打开外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);

	//*************************************************************************
	//定时器设置22.5KHz
	//*************************************************************************
	TIM_TimeBaseStructure.TIM_Period = 1599;			//用于设置在下一个更新事件装入活动的自动装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 1;		//设置用来作为TIMx时钟频率除数的预分频值 频率 = (72000)/(Prescaler*Period)KHz
	TIM_TimeBaseStructure.TIM_ClockDivision =  0;	//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//选择计数模式 TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //
	TIM_TimeBaseInit(TIM8 , &TIM_TimeBaseStructure);	//根据以上功能对定时器进行初始化
	// TIM_ARRPreloadConfig(TIM1, ENABLE); 
	//主定时器为PWM1模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//选择输出比较状态，使能输出比较状态
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	//使能输出比较N状态												
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//互补输出极性
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //   //MOE=0时，重置TIM输出比较空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//	//MOE=0时，重置TIM输出比较空闲N状态
	TIM_OCInitStructure.TIM_Pulse = (1600*D);				//占空比为50%（设置待装入捕获比较寄存器的脉冲值）Pulse=(Period*Duty)
	TIM8_OCInit(TIM8,&TIM_OCInitStructure);					//CH占空比为50%  @@@@使用了宏定义
	TIM8_OCPreload(TIM8,TIM_OCPreload_Enable);				//使能的预装载寄存器	
     
     NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;                   //使能指定IRQ通道
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;         //设置中断先占优先级
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                //从优先级
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   //使能NVIC IRQChannel中定义的IRQ通道
     NVIC_Init(&NVIC_InitStructure);
     
	TIM_Cmd(TIM8,ENABLE);		//使能TIM8主输出																					//使能定时器
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
		
	TIM_ITConfig(      //使能或者失能指定的TIM中断  
    TIM8,            //TIM8 
    TIM_IT_Update  | //TIM 更新中断源  
    TIM_IT_Trigger,  //TIM 触发中断源   
    ENABLE           //使能  
    ); 
   
     
}

volatile int SPWMTableN = 0; //SPWM表数组下标
volatile int SPWMDuty = 0;   //放入占空比寄存器的值
float SPWM_m = 0.625;     //调制比
volatile int SPWM_k = 450;   //载波比
int VoltageValueDifference = 0;    //检测电压与目标电压的差值
int RefVoltage = 1193;        //参考电压    默认25V
extern int AD_DMA_Result1,AD_DMA_Result2;
int Sum = 0,Ave = 1193,AveN = 0;

int RefCurrent = 496;         //参考电流    默认1A
int CurrentValueDifference = 0;    //检测电流与目标电流的差值
int Tim2Duty = 800;                //装入Tim2占空比寄存器的值
int BoostDownMosDuty = 800;        //Boost下管的占空比
int Result2Ave = 0,Result2Sum = 0;  //AD2平均值，求和
void TIM8_UP_IRQHandler(void)   
{      
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源   
    { 
         ms_n++;    //用于延时函数
         if(ms_n == 65535)
              ms_n = 0;
        
          read_ADC_DMA();  
          /*电压有效值反馈 电流有效值反馈*/
         Sum +=AD_DMA_Result1;          //求和再求平均值
         Result2Sum +=AD_DMA_Result2;
         AveN++;
         if(AveN == 50)
         {
              //电压求平均值
              Ave = Sum/50;
              Sum = 0;
             
               //电流求平均值
              Result2Ave = Result2Sum/50;
              Result2Sum = 0;
              
           AveN = 0;
           VoltageValueDifference = Ave - RefVoltage;  //实际电压 - 目标电压
           if(VoltageValueDifference > 2 &&VoltageValueDifference <= 20)    //实际电压大于目标电压
                SPWM_m -= 0.0001;  //减小峰值，也就是减小调制比
           else if(VoltageValueDifference < -2 && VoltageValueDifference >= -20)   
                SPWM_m +=0.0001;
           else if(VoltageValueDifference > 20 )
                SPWM_m -=0.0003;
           else if(VoltageValueDifference < -20)
                SPWM_m +=0.0003;
           if(SPWM_m > 1)     //调制比不能大于1
                SPWM_m = 1;
           if(SPWM_m <= 0)
                SPWM_m = 0.001;
           
           
             /*电流有效值反馈*/    
           CurrentValueDifference = Result2Ave - RefCurrent; //实际电流 - 目标电流
           if(CurrentValueDifference > 10 && CurrentValueDifference <= 50)   //实际电流大于目标电流0.02A 但小于0.1A
                BoostDownMosDuty -=3;        //减小Boost下管的占空比，减小输入电流
           else if(CurrentValueDifference < -10 && CurrentValueDifference >= -50)
                BoostDownMosDuty +=3;
           else if(CurrentValueDifference >50)
           {
               BoostDownMosDuty -=15;
           }
           else if(CurrentValueDifference < -50)
           {
               BoostDownMosDuty +=15;
           }
           //控制占空比
           if(BoostDownMosDuty < 2)
                BoostDownMosDuty = 2;
           if(BoostDownMosDuty > 1595)
                BoostDownMosDuty = 1595;
           Tim2Duty = 1600 - BoostDownMosDuty;
           TIM_SetCompare4(TIM2,Tim2Duty);             //改变占空比
          }
         /******************/
          
          SPWMDuty =SPWM_m * SPWM_TABLE_A[SPWMTableN] + 800;    //通过乘以调制比m获得载入寄存器的值+1600*1/2

         SPWMTableN++;   
         TIM_SetCompare2(TIM8,SPWMDuty);             //改变占空比
         if(SPWMTableN == SPWM_k)              //达到载波比数组下标清零
         {
               SPWMTableN = 0;
         }
    }       
     TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源       
} 
//计算SPWM_TABLE表函数    k载波比

int Compute_SPWM_TABLE(int k)
{
     int n=1;
     float D = 0;           //占空比
  
     for(n=1;n<=k;n++)
     {
          D = (k/(4*PI))*(cos((n-1)*2*PI/k)-cos(n*2*PI/k));         //公式计算得到占空比
          SPWM_TABLE_A[n-1]=D*1600*0.9975;         //占空比*TIM_Period=寄存器载入的值，然后缩小适当倍数
     }    
     
      
     return 1;
}

//TIM2用于输出不同频率的PWM
void time2_init(float D)
{
  TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	//打开外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	//*************************************************************************
	//定时器设置22.5kHz
	//*************************************************************************
	TIM_TimeBaseStructure.TIM_Period = 1599;				//用于设置在下一个更新事件装入活动的自动装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 1;			//设置用来作为TIMx时钟频率除数的预分频值 频率 (72000)/(Prescaler*Period)KHz
	TIM_TimeBaseStructure.TIM_ClockDivision =  0;			//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//选择计数模式 TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //     
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);		//根据以上功能对定时器进行初始化
	//主定时器为PWM1模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//选择输出比较状态，使能输出比较状态
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;	//失能输出比较N状态												
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//互补输出极性
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //  //MOE=0时，重置TIM输出比较空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//	//MOE=0时，重置TIM输出比较空闲N状态
	TIM_OCInitStructure.TIM_Pulse = (1599*D);					//占空比为50%（设置待装入捕获比较寄存器的脉冲值）Pulse=(Period*Duty)
	TIM2_OCInit(TIM2,&TIM_OCInitStructure);						//CH2占空比为50%
	TIM2_OCPreload(TIM2,TIM_OCPreload_Enable);					//使能的预装载寄存器	
		
 
	TIM_Cmd(TIM2,ENABLE);																							//使能定时器
}
//*****************************************************************************
//定义输出引脚
//*****************************************************************************
void time8_gpio()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(GPIO_Clock_PWM1|GPIO_Clock_PWM1N ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PWM1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PWM1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PWM1N  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PWM1N,&GPIO_InitStructure);
  
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PWM2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PWM2,&GPIO_InitStructure);
     
     
}
