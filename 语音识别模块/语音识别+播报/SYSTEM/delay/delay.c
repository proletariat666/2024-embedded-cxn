#include "stm32f4xx.h"
#include "delay.h"

static uint32_t fam_nus = 0;
static uint32_t fam_nms = 0;

/**********************************************************
��  �ܣ���ʱ��ʼ��
��  ������
����ֵ����
**********************************************************/
void Delay_Init(void)
{
    //�رռ�ʱ���������жϡ�����ʱ��ΪAHB/8
    SysTick->CTRL = 0x0;
    //΢�뼶��ֵ
    fam_nus = SystemCoreClock/8000000;
    //���뼶��ֵ
    fam_nms = fam_nus * 1000;
    //����������
    SysTick->VAL = 0;
}

/**********************************************************
��  �ܣ�΢�뼶��ʱ
��  ����ctr ΢�뱶�������� ctr = 1 �� ��ʱ1us
����ֵ����
**********************************************************/
void Delay_us(uint16_t ctr)
{
    uint32_t tick_flag = 0;
    
    //��ʱ����ֵ
    SysTick->LOAD = ctr*fam_nus;   
    //����������
    SysTick->VAL = 0;
    //������ʱ��
    SysTick->CTRL |= 0x01;
    do
    {
        //��ȡ��ʱ��״̬
        tick_flag = SysTick->CTRL;
    }
    //״̬�ж�
    while(!(tick_flag&(1<<16)) && (tick_flag&0x01));
    //�رռ�ʱ
    SysTick->CTRL = 0x00;
}

/**********************************************************
��  �ܣ����뼶��ʱ
��  ����ctr ΢�뱶�������� ctr = 1 �� ��ʱ1ms
����ֵ����
**********************************************************/
void Delay_ms(uint16_t ctr)
{
    uint32_t tick_flag = 0;
    
    SysTick->LOAD = ctr * fam_nms;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x01;
    do
    {
        tick_flag = SysTick->CTRL;
    }
    while(!(tick_flag & (1<<16)) && (tick_flag & 0x01));
    SysTick->CTRL = 0x00;
}

/**********************************************************
��  �ܣ���ʱnms
��  ����ctr ΢�뱶�������� ctr = 1 �� ��ʱ1ms
����ֵ����
**********************************************************/
void Delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fam_nms;						//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           						//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  //�رռ�����
	SysTick->VAL =0X00;     		  						//��ռ�����	  	    
} 

