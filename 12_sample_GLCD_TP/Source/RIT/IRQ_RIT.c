/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../pacman/pacman.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

// DECLARATION OF VARIABLES
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

extern player p;
extern int direction;
extern grid gr;
extern int pause_flag;

// RIT HANDLER
void RIT_IRQHandler (void)
{					
	static int jselect=0;
	static int jup=0;
	static int jdown=0;
	static int jleft=0;
	static int jright=0;
	
if(p.game_state == 0){
	
	// JOYSTICK SELECTION
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joytick SELECT pressed */
		jselect++;
		switch(jselect){
			case 1:
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				break;
			default:
				break;
		}
	}
	else{
			jselect=0;
	}
	
	// JOYSTICK UP
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		jup++;
		switch(jup){
			case 1:
				direction = DIRUP;
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				break;
			default:
				break;
		}
	}
	else{
			jup=0;
	}
	
	// JOYSTICK DOWN
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick DOWN pressed */
		jdown++;
		switch(jdown){
			case 1:
				direction = DIRDOWN;
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				break;
			default:
				break;
		}
	}
	else{
			jdown=0;
	}

	// JOYSTICK LEFT
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick LEFT pressed */
		jleft++;
		switch(jleft){
			case 1:
				direction = DIRLEFT;
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				break;
			default:
				break;
		}
	}
	else{
			jleft=0;
	}

	// JOYSTICK RIGHT
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick RIGHT pressed */
		jright++;
		switch(jright){
			case 1:
				direction = DIRRIGHT;
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				break;
			default:
				break;
		}
	}
	else{
			jright=0;
	}

}
/*************************INT0***************************/
if(down_0 !=0){
	down_0++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		switch(down_0){
			case 2:	
				
				menu_Pause(&p,direction);
			
				break;
			default:
				break;
		}

	}
	else {	/* button released */
		down_0=0;
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
	}
} // end INT0

/*************************KEY1***************************/
if(down_1 !=0){
	down_1++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
		switch(down_1){
			case 2:
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_1=0;			
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
	}
} // end KEY1

/*************************KEY2***************************/
if(down_2 !=0){
	down_2++;
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
		switch(down_2){
			case 2:
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_2=0;
		NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	}
} // end KEY2
	

	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
                            End Of File
******************************************************************************/
