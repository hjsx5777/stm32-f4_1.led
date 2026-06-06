#include "stm32f4xx.h"

//  LED1  PE5
//  LED2  PB0
//  LED3  PB1

void cpu_delay1(uint32_t us); // 声明了延时函数

int main(void)
{
	/*GPIO的初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;				   // 定义了一个GPIO_InitTypeDef类型的变量GPIO_InitStructure，用于存储GPIO的初始化配置参数
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  // 使能GPIOE的时钟，为GPIOE的使用提供时钟信号
	GPIO_StructInit(&GPIO_InitStructure);				   // 使用GPIO_StructInit函数将GPIO_InitStructure结构体变量的成员初始化为默认值，确保在配置GPIO之前，结构体中的所有成员都有一个已知的初始状态
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; // 配置GPIO引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   // 设置GPIO引脚的模式为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 设置GPIO引脚的输出类型为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;	   // 设置GPIO引脚的速度为高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	   // 设置GPIO引脚的上下拉电阻为无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				   // 调用GPIO_Init函数，使用配置好的GPIO_InitStructure结构体变量来初始化GPIOE的引脚5和引脚6，使其成为输出模式，并且具有推挽输出、高速和无上下拉的特性

	while (1)
	{
		for (int i = 0; i < 3; i++)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6); // led负极引脚连接到GPIOE的引脚5和引脚6，调用GPIO_ResetBits函数将这两个引脚的电平设置为低电平，点亮LED灯
			cpu_delay1(200 * 1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_5); // 调用GPIO_SetBits函数将GPIOE的引脚5和引脚6的电平设置为高电平，熄灭LED灯
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

/*延时函数*/
void cpu_delay1(uint32_t us)
{
	while (us >= 1000)// 当需要延时的时间大于或等于1000微秒（1毫秒）时，进入循环
	{
		SysTick->LOAD = SystemCoreClock / 1000;								  // systemcoreclock=168mhz,设置SysTick定时器的重载值，使其在1000微秒（1毫秒）后产生一个中断
		SysTick->VAL = 0;													  // 将SysTick定时器的当前值寄存器清零，确保定时器从0开始计数
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // 启动SysTick定时器，使用处理器时钟作为时钟源，并使能定时器
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
			; // 轮询标志位SysTick_CTRL_COUNTFLAG_Msk等待SysTick定时器的计数完成，即等待1000微秒（1毫秒）过去
		SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;// 关闭SysTick定时器，停止计数
		us -= 1000;// 从需要延时的时间中减去1000微秒（1毫秒），继续循环，直到剩余的延时时间小于1000微秒（1毫秒）
	}

	if (us > 0)// 当剩余的延时时间小于1000微秒（1毫秒）且大于0时，进入循环
	{
		SysTick->LOAD = us * SystemCoreClock / 1000 / 1000;// 设置SysTick定时器的重载值，使其在剩余的延时时间（以微秒为单位）后产生一个中断
		SysTick->VAL = 0;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
			;
		SysTick->CTRL = ~SysTick_CTRL_ENABLE_Msk;
	}
}
