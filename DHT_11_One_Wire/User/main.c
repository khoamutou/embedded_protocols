/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define DHT_GPIO_Port GPIOB
#define DHT_Pin GPIO_Pin_12

volatile float Temperture = 0;
volatile float Humid = 0;

void Delay1MS(void);
void DelayMicroSecond(uint16_t DelayMicroSecond);
void Delay_MS(uint32_t u32DelayInMs);
uint8_t DHT11_Start(void);
uint8_t DHT11_Read(void);
int main(void)
{
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_SetBits(DHT_GPIO_Port, DHT_Pin);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	//
	TIM_Cmd(TIM2, ENABLE);
	//

	while (1)
	{
		volatile uint8_t Respone = 0;
		Respone = DHT11_Start();

		if (Respone)
		{
			uint8_t data[5];
			data[0] = DHT11_Read();
			data[1] = DHT11_Read();
			data[2] = DHT11_Read();
			data[3] = DHT11_Read();
			data[4] = DHT11_Read();

			if (data[4] == data[0] + data[1] + data[2] + data[3])
			{
				
				Temperture = (float)data[0] + (float)data[1] / 10;
				Humid = (float)data[2] + (float)data[3] / 10;
			}
		}
		Delay_MS(500);
	}
}

//
void Delay1MS(void)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < 1000)
		;
}

void DelayMicroSecond(uint16_t DelayMicroSecond)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < DelayMicroSecond)
		;
}

void Delay_MS(uint32_t u32DelayInMs)
{
	while (u32DelayInMs)
	{
		Delay1MS();
		--u32DelayInMs;
	}
}

uint8_t DHT11_Start(void)
{
	uint8_t Respone = 0;
	GPIO_ResetBits(DHT_GPIO_Port, DHT_Pin);
	Delay_MS(18);
	GPIO_SetBits(DHT_GPIO_Port, DHT_Pin);
	DelayMicroSecond(30);
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
	{
		DelayMicroSecond(80);
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
	{
		TIM_SetCounter(TIM2, 0);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
		{
		};
		if (TIM_GetCounter(TIM2) < 70)
		{
			Respone = 1;
		}
	}
	return Respone;
}

uint8_t DHT11_Read(void)
{
	uint8_t a, b;
	for (a = 0; a < 8; a++)
	{
		while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
		{
		};
		DelayMicroSecond(40);
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)) // if the pin is low
			b &= ~(1 << (7 - a));
		else
			b |= (1 << (7 - a));
		//
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
		{};
	}
	return b;
}
