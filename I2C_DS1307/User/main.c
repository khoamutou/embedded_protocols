/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "systick_delay.h"
#include "stm32f10x_i2c.h"

#define DS1307_ADDRESS					0xD0

uint8_t sec, min, hour;

uint8_t bcd_to_bin(uint8_t bcd)
{
	return (bcd >> 4)*10 + (bcd & 0x0f);
}

uint8_t bin_to_bcd(uint8_t bin)
{
	return ((bin / 10) << 4) | (bin % 10);
}


void DS1307_Init()
{
	I2C_GenerateSTART( I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT ))
	{};
	
	I2C_Send7bitAddress(I2C2, DS1307_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ))
	{};
		
	I2C_SendData(I2C2, 0x07);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};	

	I2C_SendData(I2C2, 0x00);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
	{};	
	I2C_GenerateSTOP( I2C2, ENABLE);
}

void DS1307_Write()
{
	I2C_GenerateSTART( I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT ))
	{};
		
	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ))
	{};
	//
	I2C_SendData(I2C2, 0x00);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};	
	//
	I2C_SendData(I2C2, bin_to_bcd(30));
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};	
	I2C_SendData(I2C2, bin_to_bcd(27));
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};	
	I2C_SendData(I2C2, bin_to_bcd(23) );
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};
	I2C_SendData(I2C2, bin_to_bcd(2));
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};
	I2C_SendData(I2C2, bin_to_bcd(20));
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};
	I2C_SendData(I2C2, bin_to_bcd(23));
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING ))
	{};
	I2C_SendData(I2C2, bin_to_bcd(00));
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
	{};	
	I2C_GenerateSTOP( I2C2, ENABLE);
}

void DS1307_Read_Time()
{
	I2C_AcknowledgeConfig (I2C2, ENABLE);
	I2C_GenerateSTART( I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT ))
	{};
	//	
	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ))
	{};
	//	
	I2C_SendData(I2C2, 0x00);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
	{};
	//I2C_GenerateSTOP( I2C2, ENABLE);
	//
	I2C_GenerateSTART( I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT ))
	{};
	/*
		byte ke cuoi thu 3 se khong doc ngay ma se ma se cho thanh ghi btf (data tranfer finish)
	*/
	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED  ))
	{};
	while (!I2C_GetFlagStatus(I2C2, I2C_FLAG_BTF))
	{};
	//3
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED  ))
	{};
	I2C_AcknowledgeConfig (I2C2, DISABLE);
	sec = I2C_ReceiveData(I2C2);
	I2C_GenerateSTOP( I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED  ))
	{};
	min = I2C_ReceiveData(I2C2);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED  ))
	{};
	hour = I2C_ReceiveData(I2C2);
	//
}


int main(void)
			{
	InitSysTick();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2C_InitTypeDef I2C2_InitStruct;
	I2C2_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C2_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C2_InitStruct.I2C_ClockSpeed = 100000;
	I2C2_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C2_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C2_InitStruct.I2C_OwnAddress1 = 0;
	I2C_Init(I2C2, &I2C2_InitStruct);
	I2C_Cmd(I2C2, ENABLE);
	
	DS1307_Init();
	DS1307_Write();
	//Delay_SysTick(2100);
	DS1307_Read_Time();
  while (1)
  {
  }
}

