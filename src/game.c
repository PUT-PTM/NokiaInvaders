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

void drawShot(int x_pos, int y_pos){
	PCD8544_DrawPixel(x_pos, y_pos, 1);
	PCD8544_DrawPixel(x_pos, y_pos+1, 1);
	PCD8544_DrawPixel(x_pos, y_pos+2, 1);
}

void gameLoop(){
	//var initialization
	TM_LIS302DL_LIS3DSH_t axes;
	int x_axe, y_axe, z_axe;
	int player_x_pos = 37;
	int player_y_pos = 43;
	int swarm_n_row = 4;
	int swarm_n_column = 6;
	int swarm_init_count = swarm_n_row * swarm_n_column;
	int swarm_count = swarm_init_count;
	int swarm_x_pos = 1;
	int swarm_x_max = 83 - swarm_n_column * 10 - (swarm_n_column - 1) * 2;
	int swarm_y_pos = 1;
	//int swarm_y_max = 44 - swarm_n_row * 8 - (swarm_n_row - 1) * 2;
	int swarm_movement_counter = 0;
	direction swarm_dir = RIGHT;
	int (*swarm)[swarm_n_column] = malloc(sizeof(int[swarm_n_row][swarm_n_column])); //invader data
	for(int i_row = 0; i_row < swarm_n_row; ++i_row){
		for(int i_col = 0; i_col < swarm_n_column; ++i_col){
			swarm[i_row][i_col] = 1;
		}
	}
	int shot_x_pos, shot_y_pos;
	bool game_over = false;
	bool victory = false;
	bool movement = false;
	bool desperate_movement = false;
	bool shot = false;
	bool user_button_pressed = false;
	//initial draw
	drawPlayer(player_x_pos, player_y_pos);
	for(int i_row = 0; i_row < swarm_n_row; ++i_row){
		for(int i_col = 0; i_col < swarm_n_column; ++i_col){
			if(swarm[i_row][i_col]){
				drawInvader(swarm_x_pos + i_col * 12, swarm_y_pos + i_row * 10);
			}
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
			//shot mechanics
			if(shot){
				if(shot_y_pos < 1){
					shot = false;
				}else{
					bool hit = false;
					int relative_shot_y = shot_y_pos - swarm_y_pos;
					int relative_shot_x = shot_x_pos - swarm_x_pos;
					int shot_swarm_y = relative_shot_y / 10;
					int shot_swarm_x = relative_shot_x / 12;
					if(shot_swarm_y >= 0 && shot_swarm_x >= 0 && shot_swarm_y < swarm_n_row && shot_swarm_x < swarm_n_column){
						if(swarm[shot_swarm_y][shot_swarm_x]){
							if(relative_shot_x % 12 < 10){
								swarm[shot_swarm_y][shot_swarm_x] = 0;
								--swarm_count;
								shot = false;
							}
						}
					}
					if(!hit){
						--shot_y_pos;
					}
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
				if(!user_button_pressed){
					user_button_pressed = true;
					if(!shot){
						shot = true;
						shot_x_pos = player_x_pos + 2;
						shot_y_pos = player_y_pos - 3;
					}
				}
			}
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
				if(user_button_pressed){
					user_button_pressed = false;
				}
			}
			//invaders speed mechanics
			int swarm_movement_speed = 10;
			if(swarm_count < 4){
				desperate_movement = true;
			}
			while(true){
				if(swarm_movement_speed == 0){
					break;
				}
				if(swarm_count * 10 / swarm_movement_speed < swarm_init_count){
					--swarm_movement_speed;
				}else{
					break;
				}
			}
			//invaders action
			int swarm_rows_alive = 0;
			for(int i_row = swarm_n_row - 1; i_row >= 0; --i_row){
				bool row_alive = false;
				for(int i_col = 0; i_col < swarm_n_column; ++i_col){
					if(swarm[i_row][i_col]){
						row_alive = true;
						break;
					}
				}
				if(row_alive){
					swarm_rows_alive = i_row + 1;
					break;
				}
			}
			++swarm_movement_counter;
			if(swarm_movement_speed <= swarm_movement_counter){
				swarm_movement_counter = 0;
				if(swarm_dir == RIGHT){
					++swarm_x_pos;
					if(desperate_movement){
						++swarm_x_pos;
					}
					if(swarm_x_pos > swarm_x_max){
						swarm_x_pos = swarm_x_max;
						++swarm_y_pos;
						swarm_dir = LEFT;
					}
				}else{
					--swarm_x_pos;
					if(desperate_movement){
						--swarm_x_pos;
					}
					if(swarm_x_pos < 1){
						swarm_x_pos = 1;
						++swarm_y_pos;
						swarm_dir = RIGHT;
					}
				}
				if(swarm_y_pos >= (44 - swarm_rows_alive * 10)){
					game_over = true;
				}
			}
			//victory condition
			if(swarm_count == 0){
				victory = true;
			}
			//game draw
			PCD8544_Clear();
			if(victory){
				PCD8544_GotoXY(20, 13);
				PCD8544_Puts("VICTORY", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
				PCD8544_Refresh();
				break;
			}else if(game_over){
				PCD8544_GotoXY(16, 13);
				PCD8544_Puts("GAME OVER", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
				PCD8544_Refresh();
				break;
			}else{
				drawPlayer(player_x_pos, player_y_pos);
				if(shot){
					drawShot(shot_x_pos, shot_y_pos);
				}
				for(int i_row = 0; i_row < swarm_n_row; ++i_row){
					for(int i_col = 0; i_col < swarm_n_column; ++i_col){
						if(swarm[i_row][i_col]){
							drawInvader(swarm_x_pos + i_col * 12, swarm_y_pos + i_row * 10);
						}
					}
				}
				PCD8544_Refresh();
			}
			TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		}
	}
	free(swarm);
}
