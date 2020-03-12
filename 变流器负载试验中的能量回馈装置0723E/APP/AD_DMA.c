#include "AD_DMA.h"
#include "OLED.h"
/*定义数组变量ADC_ConvertedValue[4],分别放AD1通道10和11转换的数据*/
__IO uint16_t ADC_ConvertedValue[4];
/*记录转换后每位数的值*/
unsigned char AD_DMA_a1,AD_DMA_b1,AD_DMA_c1,AD_DMA_d1;
unsigned char AD_DMA_a2,AD_DMA_b2,AD_DMA_c2,AD_DMA_d2;
unsigned char AD_DMA_a3,AD_DMA_b3,AD_DMA_c3,AD_DMA_d3;
//unsigned char AD4_a,AD4_b,AD4_c;
/*设定全局变量用于反馈控制      *电感电流(有正负)PA0*    *输出电压PA1*    *输出电流(有正负)PC3* */
int AD_DMA_Result1=0;
int AD_DMA_Result2 = 0;
int AD_DMA_Result3 = 0;
int AD_DMA_Result3_Arr[50]={0};   //存放数据求中位数
/*冒泡算法*/
static void BubbleSort(int *arr, int n)
{
     int i=0,j=0;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
        {
            //如果前面的数比后面大，进行交换
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
}
/*
* 函数名：ADC1_GPIO_Config
* 描述 ：使能ADC1和DMA1的时钟,设置PA4，PA5, PA6为模拟输入
* 输入 : 无
* 输出 ：无
* 调用 ：内部调用
*/
void ADC1_GPIO_Config(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable DMA clock */
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
     /* Enable ADC1 and GPIOC & GPIOA clock */
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA ,ENABLE);
     
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 ;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
     
}

/* 函数名：ADC1_Mode_Config
* 描述 ：配置ADC1的工作模式为DMA模式
* 输入 : 无
* 输出 ：无
* 调用 ：内部调用
*/
void ADC1_Mode_Config(void)
{
      DMA_InitTypeDef  DMA_InitStructure;
      ADC_InitTypeDef  ADC_InitStructure;
      /* DMA channel1 configuration */
      DMA_DeInit(DMA1_Channel1);
      /*定义DMA外设基地址,即为存放转换结果的寄存器*/
      DMA_InitStructure.DMA_PeripheralBaseAddr =ADC1_DR_Address;
      /*定义内存基地址*/
      DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&ADC_ConvertedValue;
      /*定义AD外设作为数据传输的来源*/
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      /*指定DMA通道的DMA缓存的大小,即需要开辟几个内存空间，本实验有3个转换通道，所以开辟3个*/
      DMA_InitStructure.DMA_BufferSize = 2;
      /*设定寄存器地址固定*/
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      /*设定内存地址递加，即每次DMA都是将该外设寄存器中的值传到2个内存空间中*/
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      /*设定外设数据宽度*/	
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
      /*设定内存的的宽度*/
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
      /*设定DMA工作再循环缓存模式*/
      DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
      /*设定DMA选定的通道软件优先级*/
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
      DMA_Init(DMA1_Channel1, &DMA_InitStructure);
      /* Enable DMA channel1 */
       DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// ADC_DeInit(ADC1);//初始化ADC1
	/*设置ADC工作在独立模式*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
     /*规定AD转换工作在扫描模式，即对多个通道采样*/	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;
	/*设定AD转化在连续模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/*不使用外部促发转换*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/*采集的数据在寄存器中以右对齐的方式存放*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/*设定要转换的AD通道数目*/
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
     /*配置ADC1的通道1,2,13的转换先后顺序以及采样时间为为239.5个采样周期 */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_239Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_239Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_13Cycles5);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/*复位校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

	
void read_ADC_DMA()
{
     unsigned char i;
     //最终读取值 比如3.3V为3300
     unsigned int result1 = 0;     //
     unsigned int result2 = 0;   //
     unsigned int result3 = 0;   //

     for(i=0;i<4;i++)
     {
          result1 +=ADC_ConvertedValue[0];
          result2 +=ADC_ConvertedValue[1];
//          result3 +=ADC_ConvertedValue[2];		
     }
     result1 = result1 >>2;            //取4次采样的平均值
     result2 = result2 >>2;
  //   result3 = result3 >>2;

//     result1 = (unsigned int)(((unsigned long)result1)*3300>>12);//把采集到的数值转换成电压数据
//     result2 = (unsigned int)(((unsigned long)result2)*3300>>12);
    // result3 = (unsigned int)(((unsigned long)result3)*3300>>12);	
     /***************************
     *       result1 数据处理 
     ****************************/
     //赋值给全局变量
     AD_DMA_Result1 = result1; //电压有效值
     //化为小数显示电感电流值
//     AD_DMA_a1 = AD_DMA_Result1/1000;                //a.bcd
//     AD_DMA_b1 = (AD_DMA_Result1/100)%10;
//     AD_DMA_c1 = (AD_DMA_Result1/10)%10;
//     AD_DMA_d1 = AD_DMA_Result1%10;
     /***************************
     *       result2 数据处理       
     ****************************/
     //赋值给全局变量
     AD_DMA_Result2 = result2 ;    //实际电压 = 检测电压 * 分压比
     //转换为小数显示实际电压值
//     AD_DMA_a2 = AD_DMA_Result2/10000;                //ab.cd
//     AD_DMA_b2 = (AD_DMA_Result2/1000)%10;
//     AD_DMA_c2 = (AD_DMA_Result2/100)%10;
//     AD_DMA_d2 = (AD_DMA_Result2/10)%10;
     /**************************
     *       result3 数据处理     
     **************************/
     /*把ACS712的电压转变为电流值,方便显示Vout = 2.5 + 0.1*I  */
    // AD_DMA_Result3 = result3;

//     if(AD_DMA_Result3 < 0)
//          AD_DMA_Result3 = 0;
//     //显示
//     AD_DMA_a3 = AD_DMA_Result3/1000;                //a.bcd
//     AD_DMA_b3 = (AD_DMA_Result3/100)%10;
//     AD_DMA_c3 = (AD_DMA_Result3/10)%10;
//     AD_DMA_d3 = AD_DMA_Result3%10;


    
}

