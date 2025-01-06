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
extern ghost g;

extern route r;
volatile float aggressive_threshold = 10;
volatile float factor = 2.0/3;
node blue_dest;

extern int direction;
volatile int countdown = GAMETIME;
volatile int seed;
extern int boardMatrix[ROWS][COLS];
extern int ghostMatrix[BOXSIZE][BOXSIZE];

extern int TimerInterval2;
extern int TimerInterval3;
extern int TimerInterval2_1;

extern note waka_waka[];
extern int wakalength;

// Tracks elapsed time in real seconds
volatile int elapsed_time_speed = 0;  					
volatile int elapsed_time_ghost = 0;
volatile int elapsed_time_blue = 0;		
volatile int elapsed_time_respawn = 0; 
volatile int elapsed_time_countdown = 0;
volatile int elapsed_time_powerpill = 0;
volatile int elapsed_time_life = 0; 

// Current timer interval
/* NOTE: these values have to match with the related value in the match registers! */
volatile int current_interval_tim2 = TWENTYMS;
volatile int current_interval_tim3 = TENMS;

volatile int ghost_movement_increase;
volatile int ticks_per_second_ghost = SYSFREQ;
extern int rand_time;

uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

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
		
		static int sineticks=0;
		/* DAC management */	
		static int currentValue; 
		currentValue = SinTable[sineticks];
		currentValue -= 410;
		currentValue /= 1;
		currentValue += 410;
		LPC_DAC->DACR = currentValue <<6;
		sineticks++;
		if(sineticks==45) sineticks=0;
		
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
		
		disable_timer(0);
		
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
						move_Player(&p,&gr,direction,&g);
			}
				
			elapsed_time_powerpill = sub_Counter(elapsed_time_powerpill,&sub_second_count_powerpill,current_interval_tim2,rand_time);
			if(elapsed_time_powerpill >= 1){
				elapsed_time_powerpill = 0;
				
				// Generating the seed for the random spawn of power pills in space
				seed = LPC_TIM2->TC;
				if(gr.n_powerpills<NMAXPOWERPILLS){
					rand_PowerPill(&gr,&p);
				}		
			}
			
		}
		
		TimerInterval2_1 = 0x186A0;
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1
		
		if(p.game_state == CONTINUE){
			
			if(gr.n_powerpills != 0 || gr.n_stdpills != 0){
					
				if(p.waka_trigger && (!gr.sound_effect_triggered)){
					playSound(waka_waka,wakalength);
				}
				
				TimerInterval2_1 += 0x186A0;
				init_timer(2, 0, 1, 5, TimerInterval2_1);
				enable_timer(2);

			}
		}
		
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
		static int sub_second_count_speed = 0;
		static int sub_second_count_blue = 0;
		static int sub_second_count_respawn = 0;
		static int sub_second_count_ghost = 0;
		static int sub_second_count_life = 0;
		static int ghost_moves = ASTARINTERVAL;
		
		if(p.game_state == CONTINUE){
				
			if(gr.n_powerpills != 0 || gr.n_stdpills != 0){

				// Every 15 seconds, reduce the timer interval
				elapsed_time_speed = sub_Counter(elapsed_time_speed,&sub_second_count_speed,current_interval_tim3,SYSFREQ);
        if(elapsed_time_speed >= 10 && countdown > 0 && ticks_per_second_ghost >= 2*TWENTYMS){
           elapsed_time_speed = 0;
					
           // Decrease the interval to make Blinky faster
					ticks_per_second_ghost -= TWENTYMS;
        }
				
				if(p.life_decremented){
					elapsed_time_life = sub_Counter(elapsed_time_life,&sub_second_count_life,current_interval_tim3,SYSFREQ);
					if(elapsed_time_life >= ASTARINTERVAL){
						elapsed_time_life = 0;
						p.life_decremented = false;
					}
				}
				
				elapsed_time_ghost = sub_Counter(elapsed_time_ghost,&sub_second_count_ghost,current_interval_tim3,ticks_per_second_ghost);
        if(elapsed_time_ghost >= 1){
           elapsed_time_ghost = 0;
					
						if(!g.eaten){
							if(p.player_coord.pos.x != p.player_coord.next_pos.x ||
								p.player_coord.pos.x != p.player_coord.next_pos.y ||
								g.path_length == 0){
								

								if(!p.life_decremented){
									// Performing a* algorithm
									init_Route(&r);
										
									if(g.vulnerable != true){
										a_Star(boardMatrix,g.ghost_coord.pos,p.player_coord.pos,&r);
									}else{
										find_NewDest(boardMatrix,p.player_coord.pos.x,p.player_coord.pos.y,&blue_dest);
										a_Star(boardMatrix,g.ghost_coord.pos,blue_dest,&r);
									}
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
								elapsed_time_blue = sub_Counter(elapsed_time_blue,&sub_second_count_blue,ticks_per_second_ghost,SYSFREQ);
								if(elapsed_time_blue >= 10){
									elapsed_time_blue = 0;
									g.vulnerable = false;
								}
							}
						} else {
							
							// Section for blinky to respawn after 3 s after being eaten
							elapsed_time_respawn = sub_Counter(elapsed_time_respawn,&sub_second_count_respawn,ticks_per_second_ghost,SYSFREQ);
							if(elapsed_time_respawn >= 3){
								elapsed_time_respawn = 0;
								init_Ghost(&g);
								draw_Character(11,11,ghostMatrix,Red);
							}
							
						}
					
        }	
				
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