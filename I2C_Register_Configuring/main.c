#include "stm32f10x.h"                  // Device header

//PB10 I2C SCL
//PB11 I2C SDA
#define DS1307_ADDRESS									(0x68)
#define GPIOB_CLK_EN										(1U <<3)
#define GPIO_AF_OD											(0xF)

#define I2C_BUSY												(1U << 1)
#define I2C_START												(1U << 8)

#define I2C_STOP												(1U << 9)
#define I2C_ACK													(1U << 10)
#define I2C_ADD_TRANSMITED							(1U << 1)
#define I2C_TXE													(1U << 7)
#define I2C_RXNE												(1U << 6)
#define I2C_START_COMPLTED							(1U << 0)
void I2C2_Init(void);
char I2C_Read_Byte(char saddress, char maddress);
void I2C_Read_Time(char saddress, char *Timeptr);
char data[3];

int main(void) {
	
	I2C2_Init();
//	I2C_Read_Byte(DS1307_ADDRESS, 0x00);
//	I2C_Read_Byte(DS1307_ADDRESS, 0x01);
//	I2C_Read_Byte(DS1307_ADDRESS, 0x02);
	
	
	while(1)
	{
		I2C_Read_Time(DS1307_ADDRESS ,&data);
	}
	
}

//function
void I2C2_Init(void)
{
	//enable port b clock
	RCC->APB2ENR |= GPIOB_CLK_EN;
	
	
	//config PB10 and PB11 as AFOD
	GPIOB->CRH |= ( GPIO_AF_OD << 8 ) | ( GPIO_AF_OD << 12 );
	//GPIOB->BSRR |= (1U << 10) | (1U << 11);
	
	RCC->APB1ENR |= (1U<<22);
	
	I2C2->CR1 |= (1U << 15);
	I2C2->CR1 &= ~(1U << 15);
	
	I2C2->CR2 |= 0x24;
	
	I2C2->CCR |= 180;
	
	I2C2->CR1 |= (1U << 0);
	
}

char I2C_Read_Byte(char saddress, char maddress)
{
	volatile int temp;
	
	while ( I2C1->SR2 & I2C_BUSY) {} // wait if bus busy
		
	//I2C start
	I2C2->CR1 |= I2C_START;
	while ( !(I2C2->SR1 & I2C_START_COMPLTED )) {}  
		
	I2C2->DR = saddress << 1;
	while ( !( I2C2->SR1 & I2C_ADD_TRANSMITED )) {}  
	temp = I2C2->SR2;
	while ( !( I2C2->SR1 & I2C_TXE )) {} 
		
	I2C2->DR = maddress;	
	while ( !( I2C2->SR1 & I2C_TXE )) {} 
	//
	I2C2->CR1 |= I2C_START;
	while ( !(I2C2->SR1 & I2C_START_COMPLTED )) {} //ev5
	
	I2C2->DR = (saddress << 1) | 1U ;	
	while ( !( I2C2->SR1 & I2C_ADD_TRANSMITED )) {}  
	
	I2C2->CR1 &= ~ (I2C_ACK);
	temp = I2C2->SR2;
	I2C2->CR1 |= I2C_STOP;
	while ( !( I2C2->SR1 & I2C_RXNE )) {} 
		
	return  I2C2->DR;
}

void I2C_Read_Time(char saddress, char *Timeptr )
{
	volatile int temp;
	
	while ( I2C1->SR2 & I2C_BUSY) {} // wait if bus busy
		
	//I2C start
	I2C2->CR1 |= I2C_START;
	while ( !(I2C2->SR1 & I2C_START_COMPLTED )) {}  
		
	I2C2->DR = saddress << 1;
	while ( !( I2C2->SR1 & I2C_ADD_TRANSMITED )) {}  
	temp = I2C2->SR2;
	while ( !( I2C2->SR1 & I2C_TXE )) {} 
		
	I2C2->DR = 0x00;	
	while ( !( I2C2->SR1 & I2C_TXE )) {} 
	//
	I2C2->CR1 |=  (I2C_ACK);	
	I2C2->CR1 |= I2C_START;
	while ( !(I2C2->SR1 & I2C_START_COMPLTED )) {} //ev5
	
	I2C2->DR = (saddress << 1) | 1U ;		
	while ( !( I2C2->SR1 & I2C_ADD_TRANSMITED )) {}  
	temp = I2C2->SR2;
		
	//while ( !( I2C2->SR1 & I2C_RXNE )) {} 
	//*Timeptr++ = I2C2->DR;
		
	while ( !( I2C2->SR1 & (1U << 2) )) {} 	
	I2C2->CR1 &= ~ (I2C_ACK);
	*Timeptr++ = I2C2->DR;
	I2C2->CR1 |= I2C_STOP;
	*Timeptr++ = I2C2->DR;	
	while ( !( I2C2->SR1 & I2C_RXNE )) {} 
	*Timeptr	= I2C2->DR;
	//return  I2C2->DR;
}