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

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern unsigned char led_value;							/* defined in funct_led */
unsigned char ledval = 0xA5;

extern player p;
extern grid gr;
extern ghost g;

extern route r;
volatile float astar_interval = 10;					// Starting interval after which the a* is computed
volatile float aggressive_threshold = 10;
volatile float factor = 2.0/3;

extern int direction;
volatile int countdown = GAMETIME;
volatile double spawn_prob;
volatile int seed;
extern int boardMatrix[ROWS][COLS];
extern int ghostMatrix[BOXSIZE][BOXSIZE];

extern int TimerInterval3;
extern int freq;

// Increment of speed variables
volatile int elapsed_time_speed = 0;  				// Tracks elapsed time in real seconds
int ticks_per_second = 25000000;   						// Initial timer interval (1 second for 25 MHz frequency)
int current_interval_speed = 0x2DC6C0;  			// Current timer interval
volatile int ghost_movement_increase;

// Ghost movement interval
volatile int elapsed_time_ghost = 0;					
int current_interval_ghost = 0x2DC6C0;
int ticks_per_second_ghost = 25000000;

// Blue ghost interval
volatile int elapsed_time_blue = 0;					
int current_interval_blue = 0x2DC6C0;
node blue_dest;

// 3s respawn
volatile int elapsed_time_respawn = 0; 						
int current_interval_respawn = 0x2DC6C0;

// 1s countdown
volatile int elapsed_time_countdown = 0;  						
volatile int current_interval_countdown = 0x2DC6C0;

void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM0->IR & 2){ // MR1
		// your code	
		LPC_TIM0->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 4){ // MR2
//		disable_timer(0);
		
		// Generating the seed for the random spawn of power pills in space
		seed = LPC_TIM1->TC;
		if(gr.n_powerpills<NMAXPOWERPILLS){
		rand_PowerPill(&gr,&p);
		}
		
//		enable_timer(0);

		
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
//			countdown --;
//		
//			if (countdown<0){
//				display_GameOver();
//			}else{
//			update_TimerHeader(countdown);
////			enable_timer(1);
//			}
			
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
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1

		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 4){ // MR2
		
		LPC_TIM2->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 8){ // MR3
		// your code	
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
		static int sub_second_count_speed = 0;
		static int sub_second_count_blue = 0;
		static int sub_second_count_respawn = 0;
		static int sub_second_count_ghost = 0;
		
		if(p.game_state == CONTINUE){
			
				if(gr.n_powerpills == 0 && gr.n_stdpills == 0){
						display_Win();
				} else {
						controller_Player(direction,&p.player_coord);
						move_Player(&p,&gr,direction,&g);
				}
				
			if(gr.n_powerpills != 0 || gr.n_stdpills != 0){

				// Every 15 seconds, reduce the timer interval
				elapsed_time_speed = sub_Counter(elapsed_time_speed,&sub_second_count_speed,current_interval_speed,ticks_per_second);
        if(elapsed_time_speed >= 15 && countdown >= 0){
           elapsed_time_speed = 0;
					
           // Decrease the interval to make Blinky faster
					ticks_per_second_ghost -= 0x2DC6C0;
        }
				
				elapsed_time_ghost = sub_Counter(elapsed_time_ghost,&sub_second_count_ghost,current_interval_ghost,ticks_per_second_ghost);
        if(elapsed_time_ghost >= 1){
           elapsed_time_ghost = 0;
					
						if(!g.eaten){
							if(p.player_coord.pos.x != p.player_coord.next_pos.x ||
								p.player_coord.pos.x != p.player_coord.next_pos.y ||
								g.path_length == 0){
									
								// Performing a* algorithm
								init_Route(&r);
									
								if(g.vulnerable != true){
									a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r);
								}else{
									find_NewDest(boardMatrix,p.player_coord.pos.x,p.player_coord.pos.y,&blue_dest);
									a_Star(boardMatrix,g.ghost_coord.pos,blue_dest,&r);
								}
									
							}
								
							// Section to move ghost
							if(r.path_length>0){
								move_Ghost(&g,&r,&p);
							}
							
							// Section to make the ghost blue
							if(g.vulnerable == true && g.reset_counter == true){
								elapsed_time_blue = 0;
								g.reset_counter = false;
							}
							if(g.vulnerable == true && g.reset_counter == false){
								elapsed_time_blue = sub_Counter(elapsed_time_blue,&sub_second_count_blue,ticks_per_second_ghost,ticks_per_second);
								if(elapsed_time_blue >= 10){
									elapsed_time_blue = 0;
									g.vulnerable = false;
								}
							}
						} else {
							
							// Section for blinky to respawn after 3 s after being eaten
							elapsed_time_respawn = sub_Counter(elapsed_time_respawn,&sub_second_count_respawn,ticks_per_second_ghost,ticks_per_second);
							if(elapsed_time_respawn >= 3){
								elapsed_time_respawn = 0;
								init_Ghost(&g);
								draw_Character(11,11,ghostMatrix,Red);
							}
							
						}
					
        }	
				
				// Countdown section
				elapsed_time_countdown = sub_Counter(elapsed_time_countdown,&sub_second_count,current_interval_countdown,ticks_per_second);
				if(elapsed_time_countdown >= 1){
					elapsed_time_countdown = 0;
					countdown --;
					if (countdown<0){
						display_GameOver();
					}else{
					update_TimerHeader(countdown);
					}		
				}
				
			}
			
		}
		
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2){ // MR1
		
//		disable_timer(3);
		
		if(p.game_state == CONTINUE){
			if(gr.n_powerpills != 0 || gr.n_stdpills != 0){
				
//				// A* implementation
//				init_Route(&r);
//				a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r,cellDetails,openList,&current,closedList);

				}
		}
		
//		enable_timer(3);

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