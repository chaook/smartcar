#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "motor.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

void usart_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//�����ж���������
	NVIC_PriorityGroupConfig(2);
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//��ʼ��gpio
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//��ʼ������1
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	USART_Cmd(USART1, ENABLE);
	
}
/*
uchar i = 0, j = 0;
uchar rec[12];
uchar t;
void USART1_IRQHandler()
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		lcd_show_8X16char(i+'0', 0, 0);
		lcd_show_8X16string("display ok", 1, 0);
		rec[i] = USART_ReceiveData(USART1);	
		i++;
	//	if (i>=11)
		{
			rec[++i] = '\0';
			i = 0;
			lcd_show_8X16string(rec, 2, 0);
		}
		led_turn();
	
		if (USART_ReceiveData(USART1)=='u')
		{
			motor_up(1);
		}
	}
}
*/

u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
short int USART3_RX_STA=0; 
void USART1_IRQHandler()
{
	u8 res;	    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART1);		
		if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if((USART3_RX_STA<USART3_MAX_RECV_LEN)&&(res!='\n'))		//�����Խ�������
			{
			//	TIM_SetCounter(TIM7,0);//���������        				 
			//	if(USART3_RX_STA==0)		
			//		TIM_Cmd(TIM7, ENABLE);  //ʹ�ܶ�ʱ��7 
				USART3_RX_BUF[USART3_RX_STA++]=res;		//��¼���յ���ֵ	 
			}else 
			{
			//	USART3_RX_BUF[++USART3_RX_STA]='\0';
				USART3_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  	
 }										 
}  
