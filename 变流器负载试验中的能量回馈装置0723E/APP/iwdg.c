#include "iwdg.h"

/*******************************************************************************
* �� �� ��         : iwdg_init
* ��������		   : �������Ź�ģʽ���ó�ʼ��	   
* ��    ��         : ��
* ��    ��         : ��     
*******************************************************************************/
void iwdg_init()	//�������Ź���ʼ������
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ�ܼĴ�����д����
	IWDG_SetPrescaler(IWDG_Prescaler_64);//����IWDGԤ��Ƶֵ//40K/64=1.6ms
	IWDG_SetReload(800);//����IWDG��װ��ֵ  12λ�ģ�0~4095��//800*1.6ms=1.28s
	IWDG_ReloadCounter();//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
	IWDG_Enable();//ʹ��IWDG
}
