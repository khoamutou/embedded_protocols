/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "systick_delay.h"
#include "stm32f10x_can.h"
#include "misc.h"

void Init_RxMes(CanRxMsg *RxMessage);
void NVIC_Config(void);

int main(void)
{
	//InitSysTick();
	//NVIC_Config();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
	CAN_InitTypeDef        CAN_InitStruct;
  CAN_FilterInitTypeDef  CAN_FilterInitStruct;
	
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStruct.CAN_BS2 = CAN_BS2_4tq;
	CAN_InitStruct.CAN_Mode= CAN_Mode_Normal;
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_Prescaler = 9;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_TXFP = DISABLE;
	CAN_Init(CAN1, &CAN_InitStruct);
	
//
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;  
	CAN_FilterInit(&CAN_FilterInitStruct);
//	
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x456;
  TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
	
	for (uint8_t i = 0; i < 8;i++)
  {
    TxMessage.Data[i] = i;
  }
	//CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
  CAN_Transmit(CAN1, &TxMessage);
	/*
	txHeader.StdId = 0x030;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.IDE = CAN_ID_STD;
	txHeader.DLC = 8;
	*/
	
  while (1)
  {	
		
  }
}
//

void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

