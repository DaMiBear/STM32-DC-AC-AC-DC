#include "PWM.h"
#include "OLED.h"
#include "math.h"
#include "AD_DMA.h"
#define             PI		3.1415926
//SPWM��
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

unsigned int ms_n = 0;  //��ʱ

void Tim8_Delay_ms(int ms)
{
     int n = 0;
     ms_n = 0;
     n= ms/0.05;    //TIM1��ʱ��ÿ50us��ms_n +1
     while(ms_n != n);
}
//��ʼ����ʱ��
void time8_init(float D)     					 //D---Duty
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
     NVIC_InitTypeDef NVIC_InitStructure;
	//������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);

	//*************************************************************************
	//��ʱ������22.5KHz
	//*************************************************************************
	TIM_TimeBaseStructure.TIM_Period = 1599;			//������������һ�������¼�װ�����Զ�װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 1;		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ Ƶ�� = (72000)/(Prescaler*Period)KHz
	TIM_TimeBaseStructure.TIM_ClockDivision =  0;	//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//ѡ�����ģʽ TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //
	TIM_TimeBaseInit(TIM8 , &TIM_TimeBaseStructure);	//�������Ϲ��ܶԶ�ʱ�����г�ʼ��
	// TIM_ARRPreloadConfig(TIM1, ENABLE); 
	//����ʱ��ΪPWM1ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ѡ������Ƚ�״̬��ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	//ʹ������Ƚ�N״̬												
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//�����������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //   //MOE=0ʱ������TIM����ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//	//MOE=0ʱ������TIM����ȽϿ���N״̬
	TIM_OCInitStructure.TIM_Pulse = (1600*D);				//ռ�ձ�Ϊ50%�����ô�װ�벶��ȽϼĴ���������ֵ��Pulse=(Period*Duty)
	TIM8_OCInit(TIM8,&TIM_OCInitStructure);					//CHռ�ձ�Ϊ50%  @@@@ʹ���˺궨��
	TIM8_OCPreload(TIM8,TIM_OCPreload_Enable);				//ʹ�ܵ�Ԥװ�ؼĴ���	
     
     NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;                   //ʹ��ָ��IRQͨ��
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;         //�����ж���ռ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                //�����ȼ�
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   //ʹ��NVIC IRQChannel�ж����IRQͨ��
     NVIC_Init(&NVIC_InitStructure);
     
	TIM_Cmd(TIM8,ENABLE);		//ʹ��TIM8�����																					//ʹ�ܶ�ʱ��
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
		
	TIM_ITConfig(      //ʹ�ܻ���ʧ��ָ����TIM�ж�  
    TIM8,            //TIM8 
    TIM_IT_Update  | //TIM �����ж�Դ  
    TIM_IT_Trigger,  //TIM �����ж�Դ   
    ENABLE           //ʹ��  
    ); 
   
     
}

volatile int SPWMTableN = 0; //SPWM�������±�
volatile int SPWMDuty = 0;   //����ռ�ձȼĴ�����ֵ
float SPWM_m = 0.625;     //���Ʊ�
volatile int SPWM_k = 450;   //�ز���
int VoltageValueDifference = 0;    //����ѹ��Ŀ���ѹ�Ĳ�ֵ
int RefVoltage = 1193;        //�ο���ѹ    Ĭ��25V
extern int AD_DMA_Result1,AD_DMA_Result2;
int Sum = 0,Ave = 1193,AveN = 0;

int RefCurrent = 496;         //�ο�����    Ĭ��1A
int CurrentValueDifference = 0;    //��������Ŀ������Ĳ�ֵ
int Tim2Duty = 800;                //װ��Tim2ռ�ձȼĴ�����ֵ
int BoostDownMosDuty = 800;        //Boost�¹ܵ�ռ�ձ�
int Result2Ave = 0,Result2Sum = 0;  //AD2ƽ��ֵ�����
void TIM8_UP_IRQHandler(void)   
{      
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ   
    { 
         ms_n++;    //������ʱ����
         if(ms_n == 65535)
              ms_n = 0;
        
          read_ADC_DMA();  
          /*��ѹ��Чֵ���� ������Чֵ����*/
         Sum +=AD_DMA_Result1;          //�������ƽ��ֵ
         Result2Sum +=AD_DMA_Result2;
         AveN++;
         if(AveN == 50)
         {
              //��ѹ��ƽ��ֵ
              Ave = Sum/50;
              Sum = 0;
             
               //������ƽ��ֵ
              Result2Ave = Result2Sum/50;
              Result2Sum = 0;
              
           AveN = 0;
           VoltageValueDifference = Ave - RefVoltage;  //ʵ�ʵ�ѹ - Ŀ���ѹ
           if(VoltageValueDifference > 2 &&VoltageValueDifference <= 20)    //ʵ�ʵ�ѹ����Ŀ���ѹ
                SPWM_m -= 0.0001;  //��С��ֵ��Ҳ���Ǽ�С���Ʊ�
           else if(VoltageValueDifference < -2 && VoltageValueDifference >= -20)   
                SPWM_m +=0.0001;
           else if(VoltageValueDifference > 20 )
                SPWM_m -=0.0003;
           else if(VoltageValueDifference < -20)
                SPWM_m +=0.0003;
           if(SPWM_m > 1)     //���ƱȲ��ܴ���1
                SPWM_m = 1;
           if(SPWM_m <= 0)
                SPWM_m = 0.001;
           
           
             /*������Чֵ����*/    
           CurrentValueDifference = Result2Ave - RefCurrent; //ʵ�ʵ��� - Ŀ�����
           if(CurrentValueDifference > 10 && CurrentValueDifference <= 50)   //ʵ�ʵ�������Ŀ�����0.02A ��С��0.1A
                BoostDownMosDuty -=3;        //��СBoost�¹ܵ�ռ�ձȣ���С�������
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
           //����ռ�ձ�
           if(BoostDownMosDuty < 2)
                BoostDownMosDuty = 2;
           if(BoostDownMosDuty > 1595)
                BoostDownMosDuty = 1595;
           Tim2Duty = 1600 - BoostDownMosDuty;
           TIM_SetCompare4(TIM2,Tim2Duty);             //�ı�ռ�ձ�
          }
         /******************/
          
          SPWMDuty =SPWM_m * SPWM_TABLE_A[SPWMTableN] + 800;    //ͨ�����Ե��Ʊ�m�������Ĵ�����ֵ+1600*1/2

         SPWMTableN++;   
         TIM_SetCompare2(TIM8,SPWMDuty);             //�ı�ռ�ձ�
         if(SPWMTableN == SPWM_k)              //�ﵽ�ز��������±�����
         {
               SPWMTableN = 0;
         }
    }       
     TIM_ClearITPendingBit(TIM8, TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ       
} 
//����SPWM_TABLE����    k�ز���

int Compute_SPWM_TABLE(int k)
{
     int n=1;
     float D = 0;           //ռ�ձ�
  
     for(n=1;n<=k;n++)
     {
          D = (k/(4*PI))*(cos((n-1)*2*PI/k)-cos(n*2*PI/k));         //��ʽ����õ�ռ�ձ�
          SPWM_TABLE_A[n-1]=D*1600*0.9975;         //ռ�ձ�*TIM_Period=�Ĵ��������ֵ��Ȼ����С�ʵ�����
     }    
     
      
     return 1;
}

//TIM2���������ͬƵ�ʵ�PWM
void time2_init(float D)
{
  TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	//������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	//*************************************************************************
	//��ʱ������22.5kHz
	//*************************************************************************
	TIM_TimeBaseStructure.TIM_Period = 1599;				//������������һ�������¼�װ�����Զ�װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 1;			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ Ƶ�� (72000)/(Prescaler*Period)KHz
	TIM_TimeBaseStructure.TIM_ClockDivision =  0;			//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//ѡ�����ģʽ TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //     
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);		//�������Ϲ��ܶԶ�ʱ�����г�ʼ��
	//����ʱ��ΪPWM1ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ѡ������Ƚ�״̬��ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;	//ʧ������Ƚ�N״̬												
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//�����������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //  //MOE=0ʱ������TIM����ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//	//MOE=0ʱ������TIM����ȽϿ���N״̬
	TIM_OCInitStructure.TIM_Pulse = (1599*D);					//ռ�ձ�Ϊ50%�����ô�װ�벶��ȽϼĴ���������ֵ��Pulse=(Period*Duty)
	TIM2_OCInit(TIM2,&TIM_OCInitStructure);						//CH2ռ�ձ�Ϊ50%
	TIM2_OCPreload(TIM2,TIM_OCPreload_Enable);					//ʹ�ܵ�Ԥװ�ؼĴ���	
		
 
	TIM_Cmd(TIM2,ENABLE);																							//ʹ�ܶ�ʱ��
}
//*****************************************************************************
//�����������
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
