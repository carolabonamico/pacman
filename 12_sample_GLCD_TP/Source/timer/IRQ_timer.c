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

extern int TimerInterval3;
extern int freq;

// Increment of speed variables
volatile int elapsed_time_speed = 0;  				// Tracks elapsed time in real seconds
volatile int sub_second_count_speed = 0; 			// Sub-second timer counter
int ticks_per_second = 25000000;   						// Initial timer interval (1 second for 25 MHz frequency)
int current_interval_speed = 25000000;  			// Current timer interval

// Blue ghost interval
volatile int elapsed_time_blue = 0;  				
volatile int sub_second_count_blue = 0;						
int current_interval_blue = 25000000;  			

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
			countdown --;
			if (countdown<0){
				display_GameOver();
			}else{
			update_TimerHeader(countdown);
//			enable_timer(1);
			}
			
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
//		disable_timer(2);
		
		if(p.game_state == CONTINUE){
			if(gr.n_powerpills == 0 && gr.n_stdpills == 0){
					display_Win();
				} else {
					controller_Player(direction,&p.player_coord);
					move_Player(&p,&gr,direction,&g);
				}
		}
		
//		enable_timer(2);

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
		
		if(p.game_state == CONTINUE){
			if(gr.n_powerpills != 0 || gr.n_stdpills != 0){
				
//				if(astar_interval >= aggressive_threshold){
//					
//					astar_interval = 0;
//					if(aggressive_threshold > 4){
//						aggressive_threshold --;
//						TimerInterval3 = (int) (TimerInterval3 * factor);
//						init_timer(3,0,0,3,TimerInterval3);
//					}
//					
//					// A* implementation
//					init_Route(&r);
//					a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r);
//				}
//				astar_interval += TimerInterval3/(float) freq;
//				
//				if(r.path_length>0){
//					move_Ghost(&g,&r,&p);
//				}
//				
//				if(g.vulnerable_timer !=0){
//					g.vulnerable_timer -= (int) (TimerInterval3/freq);
//					if(g.vulnerable_timer == 0){
//						g.vulnerable = false;
//					}
//				}

//        // Increment the sub-second counter
//        sub_second_count += current_interval;

//        // When sub-second counter reaches 1 second worth of ticks
//        if (sub_second_count >= ticks_per_second) {
//						// Reset the sub-second counter
//            sub_second_count -= ticks_per_second;
//						// Increment elapsed time in seconds
//            elapsed_time++;
//        }

				elapsed_time_speed = sub_Counter(elapsed_time_speed,sub_second_count_speed,current_interval_speed);
				
        // Every 15 seconds, reduce the timer interval
        if(elapsed_time_speed >= 15){
           elapsed_time_speed = 0;
					
           // Decrease the interval to make Blinky faster
           if(current_interval_speed > 5000000){ 						// Set a minimum interval
							current_interval_speed -= 5000000; 							// Reduce interval by 0.2s
							init_timer(3,0,0,3,current_interval_speed);
           }
        }
						
				if(p.player_coord.pos.x != p.player_coord.next_pos.x ||
					p.player_coord.pos.x != p.player_coord.next_pos.y ||
					countdown == GAMETIME){
					// Performing a* algorithm
					init_Route(&r);
					a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r);
				}
					
				// Section to move ghost
				if(r.path_length>0){
					move_Ghost(&g,&r,&p);
				}
				
				// Section to make the ghost blue
				if(g.vulnerable == true){
					elapsed_time_blue = sub_Counter(elapsed_time_blue,sub_second_count_blue,current_interval_blue);
					if(elapsed_time_blue > 10){
						elapsed_time_blue = 0;
						g.vulnerable = false;
					}
				}
	
				
//				// Section to make the ghost blue
//				if(g.vulnerable_timer !=0){
//					g.vulnerable_timer --;
//					if(g.vulnerable_timer == 0){
//						g.vulnerable = false;
//					}
//				}

//				// Section to perform the search path algorithm
//				if(astar_interval == aggressive_threshold){
//					// Performing a* algorithm
//					init_Route(&r);
//					a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r);
//					astar_interval = 0;
//				}
//				// astar_interval is a counter that makes compiting the algorithm every 10s
//				astar_interval --;
//				
//				// Section to move ghost
//				if(r.path_length>0){
//					move_Ghost(&g,&r,&p);
//				}
//				
//				// Section to make the ghost blue
//				if(g.vulnerable_timer !=0){
//					g.vulnerable_timer --;
//					if(g.vulnerable_timer == 0){
//						g.vulnerable = false;
//					}
//				}
				
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