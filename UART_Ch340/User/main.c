/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "systick_delay.h"
#include "stm32f10x_usart.h"

int main(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	//
	GPIO_InitTypeDef UART2_TX_Init = {0};
	UART2_TX_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	UART2_TX_Init.GPIO_Pin = GPIO_Pin_2;
	UART2_TX_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &UART2_TX_Init);
	//GPIO_SetBits(GPIOA, GPIO_Pin_2);
	
	//
	GPIO_InitTypeDef UART2_RX_Init = {0};
	UART2_RX_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	UART2_RX_Init.GPIO_Pin = GPIO_Pin_3;
	UART2_RX_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &UART2_RX_Init);
	//GPIO_SetBits(GPIOA, GPIO_Pin_3);
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
	USART_InitTypeDef UART1_Init = {0};
	UART1_Init.USART_BaudRate = 9600;
	UART1_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART1_Init.USART_Mode = USART_Mode_Tx  | USART_Mode_Rx;
	UART1_Init.USART_Parity = USART_Parity_No;
	UART1_Init.USART_StopBits = USART_StopBits_1;
	UART1_Init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &UART1_Init);
	USART_Cmd(USART2, ENABLE);
	//
	

	USART_SendData(USART2, 0x20);
	while ( USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}; 
	USART_SendData(USART2, 0x35);
	while ( USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}; 

  while (1)
  {	
				while ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
				{
					uint8_t u8Buffer = USART_ReceiveData(USART2);
				};
  }
}
