#include "relay.h"
#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);	
	GPIO_SetBits(GPIOB, GPIO_Pin_15);	
}

void LED_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void Fan_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
}

void Fan_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
}

void Fan_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	}
}

void REFRIG_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

void REFRIG_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
}

void REFRIG_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	}
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void Buzzer_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	}
}
