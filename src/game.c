#include "game.h"

void drawInvader(int x_pos, int y_pos){
	//dimensions = 10x8
	PCD8544_DrawPixel(x_pos+2, y_pos, 1);
	PCD8544_DrawPixel(x_pos+7, y_pos, 1);
	PCD8544_DrawPixel(x_pos+3, y_pos+1, 1);
	PCD8544_DrawPixel(x_pos+6, y_pos+1, 1);
	PCD8544_DrawLine(x_pos+2, y_pos+2, x_pos+7, y_pos+2, 1);
	PCD8544_DrawLine(x_pos+1, y_pos+3, x_pos+2, y_pos+3, 1);
	PCD8544_DrawLine(x_pos+4, y_pos+3, x_pos+5, y_pos+3, 1);
	PCD8544_DrawLine(x_pos+7, y_pos+3, x_pos+8, y_pos+3, 1);
	PCD8544_DrawLine(x_pos, y_pos+4, x_pos+9, y_pos+4, 1);
	PCD8544_DrawPixel(x_pos+0, y_pos+5, 1);
	PCD8544_DrawLine(x_pos+2, y_pos+5, x_pos+7, y_pos+5, 1);
	PCD8544_DrawPixel(x_pos+9, y_pos+5, 1);
	PCD8544_DrawPixel(x_pos, y_pos+6, 1);
	PCD8544_DrawPixel(x_pos+2, y_pos+6, 1);
	PCD8544_DrawPixel(x_pos+7, y_pos+6, 1);
	PCD8544_DrawPixel(x_pos+9, y_pos+6, 1);
	PCD8544_DrawPixel(x_pos+3, y_pos+7, 1);
	PCD8544_DrawPixel(x_pos+6, y_pos+7, 1);
}

void drawPlayer(int x_pos, int y_pos){
	//dimensions = 8x4
	PCD8544_DrawPixel(x_pos+3, y_pos, 1);
	PCD8544_DrawPixel(x_pos+4, y_pos, 1);
	PCD8544_DrawLine(x_pos, y_pos+1, x_pos+7, y_pos+1, 1);
	PCD8544_DrawLine(x_pos, y_pos+2, x_pos+7, y_pos+2, 1);
	PCD8544_DrawLine(x_pos, y_pos+3, x_pos+7, y_pos+3, 1);
}

void gameLoop(){
	//var initialization
	TM_LIS302DL_LIS3DSH_t axes;
	int x_axe, y_axe, z_axe;
	int player_x_pos = 37;
	int player_y_pos = 43;
	int swarm_n_row = 3;
	int swarm_n_column = 6;
	int swarm_x_pos = 1;
	int swarm_x_max = 83 - swarm_n_column * 10 - (swarm_n_column - 1) * 2;
	int swarm_y_pos = 1;
	int swarm_y_max = 44 - swarm_n_row * 8 - (swarm_n_row - 1) * 2;
	int swarm_movement_speed = 10;
	int swarm_movement_counter = 0;
	direction swarm_dir = RIGHT;
	int (*swarm)[swarm_n_column] = malloc(sizeof(int[swarm_n_row][swarm_n_column])); //invader data
	for(int i_row = 0; i_row < swarm_n_row; ++i_row){
		for(int i_col = 0; i_col < swarm_n_column; ++i_col){
			swarm[i_row][i_col] = 1;
		}
	}
	bool movement = false;
	bool game_over = false;
	//initial draw
	drawPlayer(player_x_pos, player_y_pos);
	for(int i_row = 0; i_row < swarm_n_row; ++i_row){
		for(int i_col = 0; i_col < swarm_n_column; ++i_col){
			drawInvader(swarm_x_pos + i_col * 2 + i_col * 10, swarm_y_pos + i_row * 2 + i_row * 8);
		}
	}
	PCD8544_Refresh();
	//the loop
	for(;;){
		if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)){
			//accelerometer data
			TM_LIS302DL_LIS3DSH_ReadAxes(&axes);
			x_axe = axes.X * 0.06;
			y_axe = axes.Y * 0.06;
			z_axe = axes.Z * 0.06;
			//player movement
			if(x_axe > 400){
				movement = true;
				player_x_pos -= 4;
			}else if(x_axe < -400){
				movement = true;
				player_x_pos += 4;
			}else if(x_axe > 150){
				movement = true;
				player_x_pos -= 2;
			}else if(x_axe < -150){
				movement = true;
				player_x_pos += 2;
			}
			if(movement){
				movement = false;
				if(player_x_pos < 1){
					player_x_pos = 1;
				}
				if(player_x_pos > 73){
					player_x_pos = 73;
				}
			}
			//invaders action
			++swarm_movement_counter;
			if(swarm_movement_speed == swarm_movement_counter){
				swarm_movement_counter = 0;
				if(swarm_dir == RIGHT){
					++swarm_x_pos;
					if(swarm_x_pos > swarm_x_max){
						swarm_x_pos = swarm_x_max;
						++swarm_y_pos;
						swarm_dir = LEFT;
					}
				}else{
					--swarm_x_pos;
					if(swarm_x_pos < 1){
						swarm_x_pos = 1;
						++swarm_y_pos;
						swarm_dir = RIGHT;
					}
				}
				if(swarm_y_pos == swarm_y_max){
					game_over = true;
				}
			}
			//game draw
			PCD8544_Clear();
			if(game_over){
				PCD8544_GotoXY(16, 13);
				PCD8544_Puts("GAME OVER", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
				PCD8544_Refresh();
				break;
			}else{
				drawPlayer(player_x_pos, player_y_pos);
				for(int i_row = 0; i_row < swarm_n_row; ++i_row){
					for(int i_col = 0; i_col < swarm_n_column; ++i_col){
						drawInvader(swarm_x_pos + i_col * 2 + i_col * 10, swarm_y_pos + i_row * 2 + i_row * 8);
					}
				}
				PCD8544_Refresh();
			}
			TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		}
	}
	free(swarm);
}
