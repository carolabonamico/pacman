/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "pacman/pacman.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "led/led.h"
#include "button_EXINT/button.h"

#include <stdint.h>

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

// VARIABLE DECLARATION
player p;
ghost g;
grid gr;
route r;

extern cell cellDetails[ROWS][COLS];
extern node openList[ROWS * COLS];
extern node current;
//extern int grid_test[ROW][COL];

volatile int direction = 0;
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_LEFT[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_RIGTH[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_UP[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_DOWN[BOXSIZE][BOXSIZE];
extern int boardMatrix[ROWS][COLS];

node start = {0, 0}; // Start at (0, 0)
node dest = {8, 9};  // Destination at (8, 9)


int main(void)
{
	
  SystemInit();  
	LED_init();	
	BUTTON_init();
	LCD_Initialization();	
	LCD_Clear(Black);
	
	init_Header();
	init_Grid(&gr);
	init_GameSpace(&gr);
	init_Player(&p);
	init_Ghost(&g);
//	a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r,cellDetails,openList,&current);
	
	joystick_init();
	
//  init_RIT(0x004C4B40);								// 50ms RIT
//	init_RIT(0x14FB18);									// 55ms RIT
	init_RIT(0x006ACFC0);								// 70ms RIT	
	init_timer(0,0,2,3,0x7F2815);				// Timer for generating random power pills
	init_timer(1,0,0,3,0x17D7840);			// 1s timer for the game time
	init_timer(2,0,1,3,0x2DC6C0);				// 20ms timer for the Pacman controller
//	init_timer(3,0,1,3,0xEE6B280);			// 10s timer for the a* algorithm
	init_timer(3,0,0,3,0x17D7840);			// 1s timer for the ghost movement

	enable_RIT();	
  enable_timer(0);
	enable_timer(1);
	enable_timer(2);
	enable_timer(3);

	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
