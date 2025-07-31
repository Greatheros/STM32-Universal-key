#include "stm32f10x.h"                  // Device header
#include <stdio.H>
#include "stdarg.h"


struct __FILE 
{ 
	int handle; 
 
}; 
 
FILE __stdout;       

void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
    USART1->DR = (uint8_t) ch;      
	return ch;
}

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructrue);
	
	GPIO_InitStructrue.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructrue);
	
	USART_InitTypeDef USART_InitStructrue;
	USART_InitStructrue.USART_BaudRate=115200;        
	USART_InitStructrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;          //硬件流控制
	USART_InitStructrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; ;                  
	USART_InitStructrue.USART_Parity=USART_Parity_No;               //是否奇偶检验  
	USART_InitStructrue.USART_StopBits=USART_StopBits_1;           //停止位字节长度
	USART_InitStructrue.USART_WordLength=USART_WordLength_8b;       //发送|接受数据长度
	USART_Init(USART1,&USART_InitStructrue);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);       //中断条件配置,接收数据寄存器不空中断
	
	//*******NVIC优先级设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_Initstructrue;
	NVIC_Initstructrue.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Initstructrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initstructrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initstructrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_Initstructrue);

	USART_Cmd(USART1,ENABLE);
}

