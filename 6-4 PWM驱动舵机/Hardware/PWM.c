#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出，引脚控制权交给片上外设
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2); //选择内部时钟
	
	//初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//PSC预分频器用于分频输入时钟，ARR计数器用于记录计数值，而自动重装器用于设置计数器的上限值
	TIM_TimeBaseInitStructure.TIM_Period = 20000-1; //ARR自动重装器 10000/10k = 1s
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;//PSC预分频器  72M/7200 = 10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器（高级定时器）
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//PWM频率：Freq = CK_PSC/(PSC+1)/(ARR+1)
	//PWm占空比：Duty = CCR/(ARR+1)
	//PWM分辨率：Reso = 1/(ARR+1)
	//频率：1KHz 占空比：50% 
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); //TIM_OCInitStructure里面的变量赋初始值 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;  //CCR值
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); //OC2通道
	

	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare2(uint16_t Compare) //单独更改通道1的CCR值
{
	TIM_SetCompare2(TIM2, Compare);
}