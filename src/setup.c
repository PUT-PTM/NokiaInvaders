#include "setup.h"

void setup(){
	//nokia screen
	SystemInit();
	PCD8544_Init(0x40);
	PCD8544_Invert(PCD8544_Invert_Yes);
	//accelerometer
	TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	//timer
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 839;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);
	//user button
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_UserButton;
	GPIO_UserButton.GPIO_Pin = GPIO_Pin_0;
	GPIO_UserButton.GPIO_Mode = GPIO_Mode_IN;
	GPIO_UserButton.GPIO_OType = GPIO_OType_PP;
	GPIO_UserButton.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_UserButton.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_UserButton);
	//diode LED
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_Diode;
	GPIO_Diode.GPIO_Pin = GPIO_Pin_13;
	GPIO_Diode.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Diode.GPIO_OType = GPIO_OType_PP;
	GPIO_Diode.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Diode.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_Diode);
}
