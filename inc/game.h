#ifndef GAME_H_
#define GAME_H_

#include "tm_stm32f4_lis302dl_lis3dsh.h"
#include "tm_stm32f4_pcd8544.h"

#include "utility.h"

typedef enum{
	RIGHT, LEFT
} direction;

void drawInvader();
void drawPlayer();
void gameLoop();

#endif /* GAME_H_ */
