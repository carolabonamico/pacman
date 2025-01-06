/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../pacman/pacman.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"

extern unsigned char led_value;							/* defined in funct_led */
unsigned char ledval = 0xA5;

extern player p;
extern grid gr;

extern int direction;
volatile int countdown = GAMETIME;
volatile int seed;
extern int boardMatrix[ROWS][COLS];

extern int TimerInterval2;
extern int TimerInterval3;
extern int TimerInterval2_1;

// Tracks elapsed time in real seconds
volatile int elapsed_time_countdown = 0;
volatile int elapsed_time_powerpill = 0;
volatile int elapsed_time_life = 0; 

// Current timer interval
/* NOTE: these values have to match with the related value in the match registers! */
volatile int current_interval_tim2 = TWENTYMS;
volatile int current_interval_tim3 = TENMS;

extern int rand_time;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		
		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM0->IR & 2){ // MR1

		LPC_TIM0->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 4){ // MR2

		LPC_TIM0->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 8){ // MR3
		// your code	
		LPC_TIM0->IR = 8;			// clear interrupt flag 
	}
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER1_IRQHandler (void)
{
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM1->IR & 2){ // MR1

		LPC_TIM1->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 4){ // MR2
		// your code	
		LPC_TIM1->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 8){ // MR3
		// your code	
		LPC_TIM1->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0
	{ 
		
		static int sub_second_count_powerpill = 0;
		
		if(p.game_state == CONTINUE){
			
			if(gr.n_powerpills == 0 && gr.n_stdpills == 0){
						display_Win();
				} else {
						controller_Player(direction,&p.player_coord);
						move_Player(&p,&gr,direction);
			}
				
			elapsed_time_powerpill = sub_Counter(elapsed_time_powerpill,&sub_second_count_powerpill,current_interval_tim2,rand_time);
			if(elapsed_time_powerpill >= 1){
				elapsed_time_powerpill = 0;
				
				// Generating the seed for the random spawn of power pills in space
				seed = LPC_TIM1->TC;
				if(gr.n_powerpills<NMAXPOWERPILLS){
					rand_PowerPill(&gr,&p);
				}		
			}
			
		}
		
		TimerInterval2_1 = 0x186A0;
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1
		
		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 4){ // MR2
		
		LPC_TIM2->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 8){ // MR3
	
		LPC_TIM2->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1) // MR0
	{ 
		
		static int sub_second_count = 0;
		static int sub_second_count_life = 0;
		
		if(p.game_state == CONTINUE){
				
			if(gr.n_powerpills != 0 || gr.n_stdpills != 0){
				
				// Countdown section
				elapsed_time_countdown = sub_Counter(elapsed_time_countdown,&sub_second_count,current_interval_tim3,SYSFREQ);
				if(elapsed_time_countdown >= 1){
					elapsed_time_countdown = 0;
					countdown --;
					update_TimerHeader(countdown);
					if (countdown<=0){
						display_GameOver();
					}		
				}
				
			}
			
		}
		
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2){ // MR1
		
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 4){ // MR2
		
		LPC_TIM3->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 8){ // MR3
		// your code	
		LPC_TIM3->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

/*
void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		// your code
		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM0->IR & 2){ // MR1
		// your code	
		LPC_TIM0->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 4){ // MR2
		// your code	
		LPC_TIM0->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 8){ // MR3
		// your code	
		LPC_TIM0->IR = 8;			// clear interrupt flag 
	}
  return;
}*/