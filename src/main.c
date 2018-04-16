#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_syscfg.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
#include "tm_stm32f4_pcd8544.h"

typedef enum{
	false, true
} bool;

int x_pos;
int y_pos;

int main(void){
	//pcd8544
	SystemInit();
	PCD8544_Init(0x38);
	//diodes
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_diodes;
	GPIO_diodes.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_diodes.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_diodes.GPIO_OType = GPIO_OType_PP;
	GPIO_diodes.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_diodes.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_diodes);
	//accelerometer
	TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	TM_LIS302DL_LIS3DSH_t axes;

	int X, Y, Z;
	int width = 10;
	int height = 10;
	x_pos = 84/2 - width/2;
	y_pos = 48 - height;
	bool movement = false;
	PCD8544_DrawFilledRectangle(x_pos, y_pos, x_pos+width, y_pos+height, 1);
	PCD8544_Refresh();
	for(;;){
		TM_LIS302DL_LIS3DSH_ReadAxes(&axes);
		X = axes.X * 0.06;
		Y = axes.Y * 0.06;
		Z = axes.Z * 0.06;
		if(X > 250){
			movement = true;
			--x_pos;
		}else if(X < -250){
			movement = true;
			++x_pos;
		}
		if(movement){
			movement = false;
			if(x_pos < 3){
				x_pos = 3;
			}
			if(x_pos + width > 80){
				x_pos = 80 - width;
			}
			PCD8544_Clear();
			PCD8544_DrawFilledRectangle(x_pos, y_pos, x_pos+width, y_pos+height, 1);
			PCD8544_Refresh();
		}
	}
	//180MHz

}
