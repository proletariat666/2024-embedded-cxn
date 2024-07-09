#include "stm32f4xx.h"
#include "usart.h"	
 	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

uint16_t USART_RX_STA=0;       //接收状态标记	

/*********************************************************
*功  能：串口1初始化
*参  数: baudrate  波特率
*返回值: 无
**********************************************************/
void uart1_init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

/**************************************************************
*功  能：串口中断服务函数
*参  数: 无
*返回值: 无
**************************************************************/
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		USART_SendData(USART1,Res);
	} 
	USART_ClearFlag(USART1,USART_IT_RXNE);

} 

/**************************************************************
*功  能：串口1以字节为单位发送
*参  数: data 发送的字节
*返回值: 无
**************************************************************/
void USART1_Send_Byte(uint8_t data)
{
	//串口1发送数据
	USART_SendData(USART1,data);
	//等待发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

/**************************************************************
*功  能：串口1指定长度发送数据
*参  数: str 数据地址指针  length_num 数据长度
*返回值: 无
**************************************************************/
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num)
{
	uint8_t Tx_num = 0;
	do
	{	
		USART1_Send_Byte(*(str+Tx_num));	//指针加一
		Tx_num++;
	}
	while(Tx_num < length_num);				//指针小于数据长度
}

/**************************************************************
*功  能：串口1发送字符串
*参  数: str 数据地址指针
*返回值: 无
**************************************************************/
void USART1_Send_String(uint8_t *str)
{
	uint8_t Tx_num = 0;
	do
	{
		USART1_Send_Byte(*(str+Tx_num));	//指针加一
		Tx_num++;
	}
	while(*(str+Tx_num) != '\0');			//判断该指针对应字符是否为结束符
}
