/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_timer, funct_timer, IRQ_timer .c files
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include "../music/music.h"
#include "LPC17xx.h"

// K VALUES
// k = 25MHz * time desired

#define ONESEC 0x17D7840
#define SEVENTYMS	0x6ACFC0
#define FIFTYMS 0x4C4B40
#define TWENTYMS 0x2DC6C0
#define TENMS 0x16E360
#define SYSFREQ 25000000

/* init_timer.c */
//extern unsigned int init_timer( char timer_num, unsigned int timerInterval );
extern uint32_t init_timer( uint8_t timer_num, uint32_t Prescaler, uint8_t MatchReg, uint8_t SRImatchReg, uint32_t TimerInterval );
extern void enable_timer( uint8_t  timer_num );
extern void disable_timer( uint8_t timer_num );
extern void reset_timer( uint8_t  timer_num );

/* IRQ_timer.c */
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);
extern void TIMER2_IRQHandler (void);
extern void TIMER3_IRQHandler (void);

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
