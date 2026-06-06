#include "stm32f4xx.h"
// 测试，是否同步
// 测试2
//  LED1  PE5
//  LED2  PB0
//  LED3  PB1

void cpu_delay1(uint32_t us); // 声明了延时函数

int main(void)
{
	//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/**/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  从实例里复制过来的，和上面两行的功能没有什么区别
	//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	while (1)
	{
		for (int i = 0; i < 3; i++)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6);
			cpu_delay1(200 * 1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_5);
			cpu_delay1(200 * 1000);
		}
		for (int i = 0; i < 3; i++)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6);
			cpu_delay1(600 * 1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_5);
			cpu_delay1(600 * 1000);
		}
		for (int i = 0; i < 3; i++)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6);
			cpu_delay1(200 * 1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_5);
			cpu_delay1(200 * 1000);
		}
		cpu_delay1(2000 * 1000);
	}
}

void cpu_delay1(uint32_t us)
{
	while (us >= 1000)
	{
		SysTick->LOAD = SystemCoreClock / 1000;
		SysTick->VAL = 0;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
			;
		SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;
		us -= 1000;
	}

	if (us > 0)
	{
		SysTick->LOAD = us * SystemCoreClock / 1000 / 1000;
		SysTick->VAL = 0;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
			;
		SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;
	}
}

// void delay(unsigned int count)
//{
// int i;
// for(i=0;i<count;i++);
// }
