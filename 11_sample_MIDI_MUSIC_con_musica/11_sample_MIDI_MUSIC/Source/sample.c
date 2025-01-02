/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control leds and loudpeaker through the potentiometer
 *        	- led11 is on when the potentiometer is at minimum value
 *					- other leds light on progressively when operating the potentiometer
 *					- all leds are on when potentiometer is at maximum value
 *					- tones emitted by the loudspeaker range from central A to A2. 	
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.h"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "adc/adc.h"

#include "GLCD/GLCD.h" 
#include "pacman/pacman.h"
#include "joystick/joystick.h"
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

volatile int direction = 0;
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_LEFT[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_RIGTH[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_UP[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_DOWN[BOXSIZE][BOXSIZE];
extern int boardMatrix[ROWS][COLS];

volatile int TimerInterval3 = 0x2DC6C0;
volatile int freq = 0x17D7840;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
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

//	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
//	init_RIT(0x0016E360);									/* RIT Initialization 60 msec       	*/
	init_RIT(0x006ACFC0);										/* RIT Initialization 70 msec       	*/
	enable_RIT();														/* RIT enabled												*/
	ADC_init();
	
	LPC_SC->PCON |= 0x1;										/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1) {                           	/* Loop forever                       */	
		__ASM("wfi");
  }

}
