#include "stm32f10x.h"                  // Device header

//CS	 PB12 
//SCK2 PB13 AFPP
//MISO PB14 Input floating
//MOSI PB15 Alternate function push-pull

#define GPIOB_CLK_EN										(1U << 3)
#define SPI2_CLK_EN											(1U << 14)
#define SR_TXE													(1U << 1)
#define SR_RXNE													(1U << 0)
#define SR_BSY													(1U << 7)


uint8_t buffer[4];
void GPIOB_Init(void);
void SPI2_Init(void);
void SPI2_Send(void);

void delay(uint32_t millis)
{
	SysTick->LOAD = 72000;
	
	SysTick->VAL = 0;
	
	SysTick->CTRL = (1U << 0) | ( 1U << 2);
	
	while ( millis )
	{
		while ( !(SysTick->CTRL & (1U << 16)) ) {}
		millis--;
	}
	
	SysTick->CTRL = 0;
	
}

int main(void) 
{
	GPIOB_Init();
	SPI2_Init();
	SPI2_Send();
	while(1)
	{
		
	}
	
}

void GPIOB_Init(void)
{
//	RCC->APB2ENR |= GPIOB_CLK_EN;
//	
//	GPIOB->CRH &= ~(0xffff0000);
//	
//	GPIOB->CRH |= 0x3 << 16;
//	
//	GPIOB->CRH |= 0xB << 20;
//	
//	GPIOB->CRH |= 0x4 << 24;
//	
//	GPIOB->CRH |= 0xB << 28;
//	
//	GPIOB->ODR |= (1U << 12);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	GPIO_InitTypeDef SPI2_SCK_MOSI_Init = {0};
	SPI2_SCK_MOSI_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	SPI2_SCK_MOSI_Init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	SPI2_SCK_MOSI_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &SPI2_SCK_MOSI_Init);
	
	GPIO_InitTypeDef SPI2_MISO_Init = {0};
	SPI2_MISO_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	SPI2_MISO_Init.GPIO_Pin = GPIO_Pin_14;
	SPI2_MISO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &SPI2_MISO_Init);
	//GPIO_SetBits(GPIOA, GPIO_Pin_3);
	
	
	GPIO_InitTypeDef SPI2_CS_Init = {0};
	SPI2_CS_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	SPI2_CS_Init.GPIO_Pin = GPIO_Pin_12;
	SPI2_CS_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &SPI2_CS_Init);
}

void SPI2_Init(void)
{
	RCC->APB1ENR |= SPI2_CLK_EN;
	
	SPI2->CR1 |= 0x32F;
	SPI2->CR2 = 0;
	//SPI2->CRCPR |= 0xFFFF;
	
	SPI2->CR1 |= 0x40;
	
}

void SPI2_Send(void)
{
//	while ( !(SPI2->SR & SR_TXE) ) {}
//		
//	GPIOB->BSRR = 0x10000000;
//	while ( (GPIOB->ODR & (1 << 12)) ) {}
//		
//	SPI2->DR = 0x9F;
//	while ( SPI2->SR & 0x80 ) {}	
//	buffer[0] = SPI2->DR;
//	
//	

//	SPI2->DR = 0x00;
//	while ( SPI2->SR & 0x80 ) {}
//	buffer[1] = SPI2->DR;

//	SPI2->DR = 0x00;
//	while ( SPI2->SR & 0x80 ) {}
//	buffer[2] = SPI2->DR;

//	SPI2->DR = 0x00;
//	while ( SPI2->SR & 0x80 ) {}
//	buffer[3] = SPI2->DR;
//		
//	GPIOB->BSRR = 0x00001000;
	while ( (SPI2->SR & SR_BSY) ) {}
		
	GPIOB->BSRR = (1U << 28);
		
	while ( (GPIOB->ODR & (1 << 12)) ) {} //wait until cs low
		
	SPI2->DR = 0x9F;
	
	while ( !(SPI2->SR & SR_TXE) ) {}	//wait 
	SPI2->DR = 0x00;
	while ( !(SPI2->SR & SR_RXNE) ) {}	//receive first byte 	
	buffer[0] = SPI2->DR;
	
	while ( !(SPI2->SR & SR_TXE) ) {}	//wait 
	SPI2->DR = 0x00;
	while ( !(SPI2->SR & SR_RXNE) ) {}	//receive second byte 	
	buffer[1] = SPI2->DR;
		
	while ( !(SPI2->SR & SR_TXE) ) {}	//wait 
	SPI2->DR = 0x00;
	while ( !(SPI2->SR & SR_RXNE) ) {}	//receive second byte 	
	buffer[2] = SPI2->DR;
		
	while ( !(SPI2->SR & SR_RXNE) ) {}	//receive final byte 	
	buffer[3] = SPI2->DR;
	while ( !(SPI2->SR & SR_TXE) ) {}
	while ( (SPI2->SR & SR_BSY) ) {}
	
	GPIOB->BSRR = (1 << 12);
}


