#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_syscfg.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"

typedef enum{
	false, true
} bool;

int16_t X;
int16_t Y;
int16_t Z;

int main(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_diodes;
	GPIO_diodes.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_diodes.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_diodes.GPIO_OType = GPIO_OType_PP;
	GPIO_diodes.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_diodes.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_diodes);

	//accelerometer control
	TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	TM_LIS302DL_LIS3DSH_t axes;
	for(;;){
		TM_LIS302DL_LIS3DSH_ReadAxes(&axes);
		X = axes.X * 0.06;
		Y = axes.Y * 0.06;
		Z = axes.Z * 0.06;
		if(X > 250){
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
		}else if(X < -250){
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			GPIO_SetBits(GPIOD, GPIO_Pin_12);
		}else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		}
	}
}
