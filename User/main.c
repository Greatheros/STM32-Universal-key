#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "serial.h"

/*id为0按键的回调函数1*/
void Key0_callback1(KEY_STATE State)
{
	if(State==KEY_SINGLE)
	{
		printf("这是KEY0单击1\r\n");	
	}
	if(State==KEY_DOUBLE)
	{
		printf("这是KEY0双击1\r\n");	
	}
	if(State==KEY_LONG)
	{
		printf("这是KEY0长按1\r\n");	
	}
	if(State==KEY_REPEAT)
	{
		printf("这是KEY0重复1\r\n");	
	}
}

/*id为0按键的回调函数2*/
void Key0_callback2(KEY_STATE State)
{
	if(State==KEY_SINGLE)
	{
		printf("这是KEY0单击2\r\n");	
	}
	if(State==KEY_DOUBLE)
	{
		printf("这是KEY0双击2\r\n");	
	}
	if(State==KEY_LONG)
	{
		printf("这是KEY0长按2\r\n");	
	}
	if(State==KEY_REPEAT)
	{
		printf("这是KEY0重复2\r\n");	
	}
}

/*id为2按键的回调函数*/
void Key2_callback(KEY_STATE State)
{
	if(State==KEY_SINGLE)
	{
		printf("这是KEY2单击\r\n");	
	}
	if(State==KEY_DOUBLE)
	{
		printf("这是KEY2双击\r\n");	
	}
	if( State==KEY_LONG)
	{
		printf("这是KEY2长按\r\n");	
	}
	if(State==KEY_REPEAT)
	{
		printf("这是KEY2重复\r\n");	
	}
}

int main(void)
{
	Serial_Init();                
	Create_Key(0,GPIOA,GPIO_Pin_2);            //创建id为1的按键
	Create_Key(2,GPIOB,GPIO_Pin_1);            //创建id为2的按键 
	TIM2_ITInit();
	Button_Setrep(2,keyTRUE);                 //将id为2的按键设置为可以重复触发（长按）
	Key_Register(0,Key0_callback1);           //注册id为0按键的回调函数1
	Key_Register(0,Key0_callback2);			  //注册id为0按键的回调函数2
	Key_Register(2,Key2_callback);            //注册id为2按键的回调函数

	printf("Hello\r\n");
	while (1)
	{
		
	}
}



