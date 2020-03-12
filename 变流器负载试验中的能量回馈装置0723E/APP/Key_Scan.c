#include "Key_Scan.h"
#include "OLED.h"
#include "PWM.h"
void Key_Scan_Init()
{
	//ѡ������Ϊ�������
	GPIO_InitTypeDef GPIO_InitStructure;
     //ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
     //ѡ������Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_6| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			
	GPIO_Init(GPIOC,&GPIO_InitStructure);		
				
			
	
}

char num1[10]={0},num2[10]={0};//�����������飬num1��ʾ��ѹ��num2��ʾ����
char num=0;				//�м���
char num_n=0;				//������±�
char show_x=0;				//���ڰ�����������ʾ����
unsigned char a=0;			//������������ĵ�һ�����͵ڶ����� a=0�������һ������a=1������ڶ�����
float  num1_result=0,num2_result=0;//num1_result->��num2_result->

extern int RefVoltage; //main.c �ο���ѹ
extern int RefCurrent;  //main.c �ο�����
extern float SPWM_m; 
void Key_Dealy()
{
     int n,t;
     for(t=0;t<100;t++)
     for(n=0;n<2900;n++);
          
}
void Key_Scan()
{
	/**********************************************************
                              ��һ��
     **********************************************************/
          X1_H;
		X2_L;X3_L;X4_L;
     /***********************************************************/
      if(ReadY1State)
      {
           Key_Dealy();
		if(ReadY1State)		//111111111111111111111111111111
		{  
			while(ReadY1State);
			num = 1;	
			if(a==0)
			{	
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
		}
     }
      /***********************************************************/
      if(ReadY2State)
      {
           Key_Dealy();
		if(ReadY2State)   //22222222222222222222222222222222
		{	 
               while(ReadY2State);
			num = 2;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}	
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
		}
     }
      /***********************************************************/
      if(ReadY3State)
      {
           Key_Dealy();
		if(ReadY3State)   //33333333333333333333333333333333
		{	
               while(ReadY3State);
			num = 3;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
			
		}
     }
      /***********************************************************/
      if(ReadY4State)
      {
           Key_Dealy();
		if(ReadY4State) 	//(1��,4��)��ʾȷ�����������ѹ
          {		
               while(ReadY4State);
              if(num1[3]==0)                //��������������
              if(num1[1]==0&&num_n==1)	//�����һ������һλ����
               {
                    num1[1]=num1[0];//��ԭ��ʮλ���� ��ֵ����λ
                    num1[0]=0;			//��ֵ��ɺ��ʮλ����
               }
			num_n=0;				//�����±�����
			show_x=0;
			a=1;					//��a=1 �Ժ�����Ķ��ǶԵڶ�������ֵ
               
               OLED_ShowString(0,0,(unsigned char *)"U");	
        	
		}
     }
      /**********************************************************
                         �ڶ���
     **********************************************************/
		X2_H;
		X1_L;X3_L;X4_L;
     if(ReadY1State)
     {
          Key_Dealy();
		if(ReadY1State)		//444444444444444444444444
		{    
               while(ReadY1State);
			num = 4;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
		}
     }
     /***********************************************************/
     if(ReadY2State)
     {
          Key_Dealy();
		if(ReadY2State)   //555555555555555555555555
		{		
               while(ReadY2State);
			num = 5;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
			
		}
     }
     /***********************************************************/
     if(ReadY3State) 
     {
          Key_Dealy();
		if(ReadY3State)   //66666666666666666666666
		{	  
               while(ReadY3State);
			num = 6;			
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
			
		}
     }
     /***********************************************************/
     if(ReadY4State) 
     {
          
          Key_Dealy();
		if(ReadY4State)   //  2��4��
		{	  	
               while(ReadY4State);
			num_n=0;				//�����±�����
			show_x=0;
			a=1;					//��a=1 �Ժ�����Ķ��ǶԵڶ�������ֵ
               
               RefCurrent = 496;
            //   SPWM_m -=0.01;
			
		}
     }
     /**********************************************************
                         ������
     **********************************************************/
		X3_H;
		X1_L;X2_L;X4_L;
     if(ReadY1State)	
     {
         Key_Dealy();
		if(ReadY1State)		//77777777777777777777777
		{ 
               while(ReadY1State);
			num = 7;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
		}
     }
     /***********************************************************/
     if(ReadY2State)  
     {
          Key_Dealy();
		if(ReadY2State)   //888888888888888888888888
		{  
               while(ReadY2State);
			num = 8;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
		}
     }
     /***********************************************************/
     if(ReadY3State) 
     {
          Key_Dealy();
		if(ReadY3State)   //9999999999999999999999999
		{	  
               while(ReadY3State);               
			num = 9;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
		}
     }
     /***********************************************************/
     if(ReadY4State) 
     {
          Key_Dealy();
		if(ReadY4State)   //3��4��
		{  	
               while(ReadY4State);
			num_n=0;				//�����±�����
			show_x=0;
			a=1;					//��a=1 �Ժ�����Ķ��ǶԵڶ�������ֵ
			
               RefCurrent = 993;
             //  SPWM_m +=0.01;
		}
     }
     /**********************************************************
                         ������
     **********************************************************/
		X4_H;
		X1_L;X2_L;X3_L;
     if(ReadY1State)
     {
          Key_Dealy();
		if(ReadY1State)		//................
		{  
               while(ReadY1State);
			if(a==0)						//�����һ����
			{
				num1[num_n]='.';
				OLED_ShowString(16+num_n*8,0,(uchar *)".");
				if(num1[1]=='.')		//��������ֻ������һ������
				{
					//����������һλ
					num1[2]=num1[1]; 
					num1[1]=num1[0];
					num1[0]=0;		//ʮλ��0
					num_n++;//�����±�Ϊ3���������С������
				}
			}
			if(a==1)					//����ڶ�����
			{
				num2[num_n]='.';
				OLED_ShowString(16+num_n*8,2,(uchar *)".");
				if(num2[1]=='.')		//��������ֻ������һ������
				{
					//����������һλ
					num2[2]=num2[1];
					num2[1]=num2[0];
					num2[0]=0;		//ʮλ��0
					num_n++;			
				}
			}
			num_n++;
			show_x++;
		}
     }
     /***********************************************************/
     if(ReadY2State) 
     {
          Key_Dealy();
		if(ReadY2State)   //0000000000000000000000
		{  	
               while(ReadY2State);
			num = 0;
			if(a==0)
			{
				num1[num_n]=num;//�����鵱ǰnum_n��Ӧ���±꿪ʼ��ֵ
				OLED_ShowNum(16+show_x*8,0,num,1,16);
				num_n++;		//�±�+1
				show_x++;
			}
			if(a==1)	//����������������a==1��ʼ�Եڶ�������ֵ
			{
				num2[num_n]=num;
				OLED_ShowNum(16+show_x*8,2,num,1,16);
				num_n++;
				show_x++;
			}
			
		}
     }
     /***********************************************************/
     if(ReadY3State) 
     {
          Key_Dealy();
		if(ReadY3State)  //DEL 
		{	 
               int i=0;
               while(ReadY3State);
			num_n=0;//�����±��ʼ��
			a=0;
			show_x=0;
			for(i=0;i<=3;i++)  //��������
			{
				num1[i]=0;
				num2[i]=0;
			}
			OLED_Clear();//����
             // SoftReset();
			
		}
     }
     /***********************************************************/
     if(ReadY4State)
     {
       
          extern int SPWM_k ;   //�ز���(PWM.c)
          
          Key_Dealy();
		if(ReadY4State)   //=================
		{  	               
               while(ReadY4State);
			
			if( num2[3]==0)		//��������������
			{
				if(num2[1]==0&&num_n==1)	//����ڶ�������һλ����
				{
					num2[1]=num2[0];//��ԭ��ʮλ���� ��ֵ����λ
					num2[0]=0;			//��ֵ��ɺ��ʮλ����
				}
			}
               num_n=0;				//�����±�����
			show_x=0;				//��ʾ�±�����
			num1_result=(float)num1[0]*10+(float)num1[1]+(float)num1[3]*0.1;//�������е�ÿ�����ϲ�Ϊ������XX.X
			num2_result=(float)num2[0]*10+(float)num2[1]+(float)num2[3]*0.1;
//               OLED_ShowNum(0,6,(int)(num1_result * 10),3,16);
//               OLED_ShowNum(64,6,(int)(num2_result * 10),3,16);
               RefVoltage = num1_result * 47.739;      //47.739 = 4096/26/3.3
               SPWM_k = (int)(22500 / num2_result);
               if( SPWM_k >= 1300 || SPWM_k <= 0 ) //�������Խ�硣
               {
                    OLED_ShowString(0,4,(unsigned char *)"Error!");
                    SPWM_k = 450;
                    SPWM_m = 1;
               }
               OLED_ShowString(0,2,(unsigned char *)"f");        //Ƶ�����������ʾf
               TIM_Cmd(TIM8,DISABLE);
               Compute_SPWM_TABLE(SPWM_k);    
               
               OLED_ShowString(56,0,(unsigned char *)"Done");
               TIM_Cmd(TIM8,ENABLE);
		}
     }

}


