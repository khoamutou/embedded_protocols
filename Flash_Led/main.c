//#include "stm32f10x.h"                  // Device header

//volatile uint8_t a;
//volatile uint8_t j;

//void delay(void);
//void toggle_led(void);
//int main(void)
//{
//	RCC->APB2ENR |= 1 << 4; //enable gpio c
//	
//	GPIOC->CRH &= 0x0f0fffff;
//	
//	GPIOC->CRH |= 0x3 << 20;

//	GPIOC->CRH |= 1U << 31;
//	
//	while(1)
//	{
//		toggle_led();
//		/*
//		//toggle_led();
//		if ( GPIOC->IDR & 1 << 15 )
//		{
//			//GPIOC->BSRR = (1U << 29);//led on
//			toggle_led();
//		}
//		else
//		{
//			GPIOC->BSRR = (1U << 13); //led off
//		}
//		*/
//	}
//}

//void delay()
//{	
//	int i;
//	for ( i = 0; i < 1000000 ; i++)
//	{}
//}			


//void toggle_led()
//{
//	delay();
//	//GPIOC->ODR |= 1 << 13; 
//	GPIOC->BSRR = (1U << 29); 
//	delay();
//	GPIOC->BSRR = (1U << 13); 
//	//GPIOC->ODR &= 0 << 13; 
//}

#include <stdint.h>
#include "stm32f10x.h"							// use library be built by dev

static void delay(void);



int main(void){
	RCC->APB2ENR |= 1<<4;							// enable clock for PORT C
	GPIOC->CRH &= ~(0b1111<<20);			// clear 1 nibble to set MODE of PIN
	
	GPIOC->CRH &= ~(0b11<<22);			  // set output with mode push pull
	GPIOC->CRH |= (0b11<<20);					// set output with bus_CLK 50MHz
	
	GPIOC->ODR |= 0b1<<13;						// HIGH to off led 
	GPIOC->ODR &= ~(0b1<<13);
	
	while(1)
	{
		delay();
		GPIOC->ODR &= ~(1<<13); 			// LOW to on led
		delay();
		GPIOC->ODR |= (1<<13);				// HIGH to off led 	
	}
	//return 0;
}



static void delay(void){
	uint32_t i = 0;												// for count
	
	while(i<1000000){
		i++;
	}
}
