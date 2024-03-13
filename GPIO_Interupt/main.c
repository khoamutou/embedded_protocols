#include <stdint.h>
#include "stm32f10x.h"							// use library be built by dev

void delay(void);

#define GPIOC_EN						( 1 << 4)
#define GPIOA_EN						( 1 << 2)

uint32_t counter;
int main(void){
	RCC->APB2ENR |= GPIOC_EN | GPIOA_EN;							// enable clock for PORT C
	GPIOC->CRH &= ~(0b1111<<20);			// clear 1 nibble to set MODE of PIN
	
	GPIOC->CRH &= ~(0b11<<22);			  // set output with mode push pull
	GPIOC->CRH |= (0b11<<20);					// set output with bus_CLK 50MHz
	
	GPIOC->ODR |= 0b1<<13;						// HIGH to off led 
	GPIOC->ODR &= ~(0b1<<13);
	
	GPIOA->CRL &= ~(0b1111);
	GPIOA->CRL |= 0b1000; //set pa0 to input pull
	
	__disable_irq();
	AFIO->EXTICR[0] &= ~(0b1111); //config pA0 to ngat ngoai
	EXTI->IMR |= (1U << 0);       // kich hoat ngat ngoai
	EXTI->RTSR |= (1U << 0); 			//rising trigger
	NVIC_EnableIRQ(EXTI0_IRQn);		//
	__enable_irq();
	
	while(1)
	{

	}
}



void delay(void){
	uint32_t i = 0;												// for count
	
	while(i<1000000){
		i++;
	}
}

void EXTI0_IRQHandler(void)
{
	EXTI->PR |= 1;
	GPIOC->ODR ^= ( 1U << 13);
	counter++;
}
