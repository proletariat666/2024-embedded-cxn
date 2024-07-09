#include "stm32f4xx.h"
#include "usart.h"	
 	
uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

uint16_t USART_RX_STA=0;       //����״̬���	

/*********************************************************
*��  �ܣ�����1��ʼ��
*��  ��: baudrate  ������
*����ֵ: ��
**********************************************************/
void uart1_init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}

/**************************************************************
*��  �ܣ������жϷ�����
*��  ��: ��
*����ֵ: ��
**************************************************************/
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		USART_SendData(USART1,Res);
	} 
	USART_ClearFlag(USART1,USART_IT_RXNE);

} 

/**************************************************************
*��  �ܣ�����1���ֽ�Ϊ��λ����
*��  ��: data ���͵��ֽ�
*����ֵ: ��
**************************************************************/
void USART1_Send_Byte(uint8_t data)
{
	//����1��������
	USART_SendData(USART1,data);
	//�ȴ��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

/**************************************************************
*��  �ܣ�����1ָ�����ȷ�������
*��  ��: str ���ݵ�ַָ��  length_num ���ݳ���
*����ֵ: ��
**************************************************************/
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num)
{
	uint8_t Tx_num = 0;
	do
	{	
		USART1_Send_Byte(*(str+Tx_num));	//ָ���һ
		Tx_num++;
	}
	while(Tx_num < length_num);				//ָ��С�����ݳ���
}

/**************************************************************
*��  �ܣ�����1�����ַ���
*��  ��: str ���ݵ�ַָ��
*����ֵ: ��
**************************************************************/
void USART1_Send_String(uint8_t *str)
{
	uint8_t Tx_num = 0;
	do
	{
		USART1_Send_Byte(*(str+Tx_num));	//ָ���һ
		Tx_num++;
	}
	while(*(str+Tx_num) != '\0');			//�жϸ�ָ���Ӧ�ַ��Ƿ�Ϊ������
}
