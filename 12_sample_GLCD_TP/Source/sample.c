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
#include "LPC17xx.h"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "ADC/adc.h"
#include "TouchPanel/TouchPanel.h"
#include "GLCD/GLCD.h" 
#include "pacman/pacman.h"
#include "joystick/joystick.h"
#include "button_EXINT/button.h"
#include "CAN/CAN.h"

#include <stdint.h>
#include <stdio.h>

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

volatile int direction = 0;
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_LEFT[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_RIGTH[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_UP[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_DOWN[BOXSIZE][BOXSIZE];
extern int boardMatrix[ROWS][COLS];

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
int main(void)
{
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	CAN_Init();
  LED_init();                           /* LED Initialization                 */
	BUTTON_init();
	LCD_Initialization();	
	LCD_Clear(Black);
	
	init_Header();
	init_Grid(&gr);
	init_GameSpace(&gr);
	init_Player(&p);
	init_Ghost(&g);
		
	joystick_init();

	init_RIT(SEVENTYMS);										/* RIT Initialization 70 msec       	*/
	enable_RIT();														/* RIT enabled												*/
	ADC_init();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);				

	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
