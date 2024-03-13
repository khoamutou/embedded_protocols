#include "stm32f10x.h"                  // Device header

#define UART2_ENABLE 					(1U << 17)
#define GPIOA_ENABLE 					(1U << 2)
#define AF_PP				 					(0xB)
#define INPUT_FLOATING 			 	(0x4)

#define CR1_TE 								(1U << 3)
#define CR1_RE 								(1U << 2)
#define CR1_UE 								(1U << 13)
void UART2_Init(void);
void UART2_Transmit( char chat);
char UART2_Reveive(void);

volatile char buffer;

int main(void)
{
	UART2_Init();
	
	while(1)
	{
		buffer = UART2_Reveive();
		UART2_Transmit(buffer);
	}
}

void UART2_Init(void)
{
	//enable clock port A
	RCC->APB2ENR |= GPIOA_ENABLE;
	//config PA2 as afpp
	GPIOA->CRL &= ~(0xf << 8);
	GPIOA->CRL |= AF_PP << 8;
	
	//config PA2 as ipft
	GPIOA->CRL &= ~(0xf << 12);
	GPIOA->CRL |= INPUT_FLOATING << 12;
	
	//enable uart2
	RCC->APB1ENR |= UART2_ENABLE;
	
	//set baud 9600
	USART2->BRR = 0xEA6;
	
	//set PA2 as TX2
	USART2->CR1 |= CR1_TE;
	
	//set PA3 as RX2
	USART2->CR1 |= CR1_RE;
	
	//Start UART2
	USART2->CR1 |= CR1_UE;

}

void UART2_Transmit( char chat)
{
		while ( !(USART2->SR & (1<<7)) ) {}
			
		USART2->DR = chat & 0xff;
}

char UART2_Reveive(void)
{
	while ( !(USART2->SR & (1<<5)) ) {};
		
	return USART2->DR;		
}