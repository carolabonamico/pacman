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
#include "../timer/timer.h"
#include "../pacman/pacman.h"

// DECLARATION OF VARIABLES

volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;
volatile int begin = 0;
volatile int TimerInterval2 = TWENTYMS;
volatile int TimerInterval3 = TENMS;
volatile int TimerInterval2_1 = 0x186A0;				// 4ms

extern player p;
extern ghost g;
extern int direction;
extern grid gr;
extern int pause_flag;
extern note song[];
extern int songlength;
extern note ghost_vulnerable[];
extern int ghostvulnerablelength;
extern note pacman_death[];
extern int deathlength;
extern note victory[];
extern int victorylength;
extern note game_over[];
extern int gameoverlength;
extern int countdown;
extern note power_up[];
extern int poweruplength;
extern note eating_sound[];
extern int eatlength;

// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{
	static int jselect=0;
	static int jup=0;
	static int jdown=0;
	static int jleft=0;
	static int jright=0;
	
	// Initial soundtrack
	if(begin == 0){
    playSound(song,songlength);
	}
	
    if (begin == 1) {
			
        // Initialize timers after soundtrack playback
        init_timer(2, 0, 0, 3, TimerInterval2);  						// Timer for random power pills + pacman movement
        init_timer(3, 0, 0, 3, TimerInterval3);  					// Timer for ghost movement + ghost speedup + countdown implementation
				init_timer(2, 0, 1, 5, TimerInterval2_1);						// Waka sound implementation

        enable_timer(2);  // Enable Timer 2
        enable_timer(3);  // Enable Timer 3

        reset_RIT();      // Reset RIT for normal operation
        enable_RIT();     // Re-enable RIT
        begin++;          // Move to the next stage
    }
	
	if(begin > 1){
		if(p.game_state == CONTINUE){
			
			/*************************SOUND EFFECTS***************************/
			
			if(g.play_vulnerable){
				gr.sound_effect_triggered = true;
				playSound(ghost_vulnerable,ghostvulnerablelength);
			} else if(p.nlives == 0){
				playSound(pacman_death,deathlength);
				gr.sound_effect_triggered = true;
			} else if(gr.n_powerpills == 0 && gr.n_stdpills == 0){
				gr.sound_effect_triggered = true;
				playSound(victory,victorylength);
			} else if(countdown < 0){
				gr.sound_effect_triggered = true;
				playSound(game_over,gameoverlength);
			} else if(p.life_incremented){
				gr.sound_effect_triggered = true;
				playSound(power_up,poweruplength);
			} else if(g.play_eaten){ 
				gr.sound_effect_triggered = true;
				playSound(eating_sound,eatlength);
			}
			/*************************JOYSTICK SELECTION***************************/
			
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
			
			/*************************JOYSTICK UP***************************/
			
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
			
			/*************************JOYSTICK DOWN***************************/
			
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

			/*************************JOYSTICK LEFT***************************/
			
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

			/*************************JOYSTICK RIGHT***************************/
			
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
	
	}
		
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
		
}
/******************************************************************************
**                            End Of File
******************************************************************************/
