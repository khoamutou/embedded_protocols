/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "systick_delay.h"
#include "stm32f10x_i2c.h"

void lcd_send_byte(char data)
{
//	I2C_GenerateSTART( I2C2, ENABLE);
//	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT ))
//	{};
//	I2C_Send7bitAddress(I2C2, 0x4E, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ))
//	{};
	I2C_SendData(I2C2, data);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
	{};	
//	I2C_GenerateSTOP( I2C2, ENABLE);	
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
		for (uint8_t i = 0; i < 4; ++i)
	{
		//lcd_send_byte(data_t[i]);
		I2C_SendData(I2C2, data_t[i]);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
		{};			
	}
}



void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	for (uint8_t i = 0; i < 4; ++i)
	{
		//lcd_send_byte(data_t[i]);
		I2C_SendData(I2C2, data_t[i]);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ))
		{};	
	}
}

void lcd_send_string (uint8_t *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
	Delay_SysTick(50);
}


void lcd_init (void)
{
	lcd_send_cmd (0x33); /* set 4-bits interface */
	Delay_SysTick(50);
	lcd_send_cmd (0x32);
	Delay_SysTick(50);
	lcd_send_cmd (0x28); /* start to set LCD function */
	Delay_SysTick(50);
	lcd_send_cmd (0x06); /* set entry mode */
	Delay_SysTick(50);
	lcd_send_cmd (0x0C); /* set display to on */	
	Delay_SysTick(50);
	lcd_send_cmd (0x02); /* move cursor to home and set data address to 0 */
	Delay_SysTick(50);
	lcd_send_cmd (0x80);
	Delay_SysTick(50);
}


int main(void)
{
	InitSysTick();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	//GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
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
	
	I2C_GenerateSTART( I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT ))
	{};
	
	I2C_Send7bitAddress(I2C2, 0x4E, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ))
	{};
	lcd_init();

	lcd_send_string("HCMUTE");
	I2C_GenerateSTOP( I2C2, ENABLE);
  while (1)
  {
		
  }
}

