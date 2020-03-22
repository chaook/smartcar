#ifndef _USART_H
#define _USART_H

#include "stm32f10x.h"

#define USART3_MAX_RECV_LEN		100					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern short int USART3_RX_STA;   						//��������״̬

void usart_init(void);

#endif
