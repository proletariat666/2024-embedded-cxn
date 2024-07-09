#include "stm32f4xx.h"
#include "head.h"
#include "ID.h"

extern const unsigned char bkrc[][128];
extern const unsigned char Test_Name[][128];

unsigned char tmp_buf[16] = {0x55,0xaa,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //发送的数据
unsigned char keyvalue;

uint8_t display_flag = 0;

int main(void)
{
	Key_Hardware_Init();
	LED_Hardware_Init();
	Delay_Init();
	LCD_Configure();

	GUI_DrawFont32(32, 0,BLACK, WHITE, 0,7, 0,bkrc);
	GUI_DrawFont32(24, 32,BLACK, WHITE, 8,15, 0,bkrc);
	GUI_DrawFont32(56, 70,BLACK, WHITE, 0,5, 0,Test_Name);
	LCD_ShowString(32,140,24*3,24,24,"ID:");
	
	GPIO_ID_Init();
	SYN7318_Init();

	while(1)
	{
		keyvalue = Key_Scan(0);
		
		if(keyvalue == 1)
		{
			SYN7318_flag = 1 ;
		}

		SYN7318Test();			//语音

	} 
}


