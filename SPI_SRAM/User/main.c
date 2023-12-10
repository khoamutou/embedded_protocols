/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "systick_delay.h"
#include "stm32f10x_spi.h"

volatile uint8_t u8Buff[256];
volatile uint8_t u8ReadBuff[256];

uint16_t SPI_Send_And_Receive(uint16_t data);
void Read_ID(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	SPI_Send_And_Receive(0x9f);
	SPI_Send_And_Receive(0x00);
	SPI_Send_And_Receive(0x00);
	SPI_Send_And_Receive(0x00);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
void Sector_Erase(uint32_t sector_address)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	SPI_Send_And_Receive(0x06);
	SPI_Send_And_Receive(0x20);
	SPI_Send_And_Receive(sector_address >> 16);
	SPI_Send_And_Receive(sector_address >> 8);
	SPI_Send_And_Receive(sector_address);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	Delay_SysTick(400);
}

void Page_Write(uint32_t sector_address)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	SPI_Send_And_Receive(0x06);
	SPI_Send_And_Receive(0x02);
	SPI_Send_And_Receive(sector_address >> 16);
	SPI_Send_And_Receive(sector_address >> 8);
	SPI_Send_And_Receive(sector_address);
	for (uint16_t i = 0; i <= 255; i++)
	{
		u8Buff[i] = i;
		SPI_Send_And_Receive(u8Buff[i]);
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

void Page_Read(uint32_t sector_address)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	SPI_Send_And_Receive(0x03);
	SPI_Send_And_Receive(sector_address >> 16);
	SPI_Send_And_Receive(sector_address >> 8);
	SPI_Send_And_Receive(sector_address);
	for (uint16_t i = 0; i <= 255; i++)
	{
		u8ReadBuff[i] = SPI_Send_And_Receive(0x00);
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
int main(void)
{
	InitSysTick();

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	/*
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	*/
	GPIO_InitTypeDef SPI2_SCK_MOSI_Init = {0};
	SPI2_SCK_MOSI_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	SPI2_SCK_MOSI_Init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	SPI2_SCK_MOSI_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &SPI2_SCK_MOSI_Init);
	//GPIO_SetBits(GPIOA, GPIO_Pin_2);
	
	//
	
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
	

	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_InitTypeDef SPI2_Init = {0};
	SPI2_Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI2_Init.SPI_CPHA = SPI_CPHA_2Edge;
	SPI2_Init.SPI_CPOL = SPI_CPOL_High;
	SPI2_Init.SPI_CRCPolynomial = SPI_CRCPR_CRCPOLY;
	SPI2_Init.SPI_DataSize = SPI_DataSize_8b;
	SPI2_Init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI2_Init.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI2_Init.SPI_Mode = SPI_Mode_Master;
	SPI2_Init.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI2_Init);
	SPI_Cmd(SPI2, ENABLE);
//
	Read_ID();
	Delay_SysTick(1);
	Sector_Erase(0U);
	Delay_SysTick(1);
	Page_Write(0U);
	Delay_SysTick(1);
	Page_Read(0U);
  while (1)
  {	
		
  }
}

uint16_t SPI_Send_And_Receive(uint16_t data)
{
	SPI_I2S_SendData(SPI2, data);
	while ( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET)
	{};
	return SPI_I2S_ReceiveData(SPI2);
}


