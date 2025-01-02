#include "music.h"

#include "../timer/timer.h"

#define UPTICKS 1

extern int begin;

NOTE song[] = 
{
 {Si, time_semicroma}, 
 {Si_alto, time_semicroma}, 
 {Fa_diesis, time_semicroma}, 
 {Mi_bemolle, time_semicroma}, 
 {Si_alto, time_biscroma}, 
 {Fa, time_semicroma + time_biscroma}, 
 {Mi, time_croma}, 
 {Do, time_semicroma}, 
 {Do_alto, time_semicroma}, 
 {Sol, time_semicroma}, 
 {Mi, time_semicroma}, 
 {Do_alto, time_biscroma}, 
 {Sol, time_semicroma + time_biscroma}, 
 {Mi, time_croma}, 
 {Si, time_semicroma}, 
 {Si_alto, time_semicroma}, 
 {Fa_diesis, time_semicroma}, 
 {Mi_bemolle, time_semicroma}, 
 {Si_alto, time_biscroma}, 
 {Fa, time_semicroma + time_biscroma}, 
 {Mi, time_croma}, 
 {Mi, time_biscroma}, 
 {Mi, time_semicroma},
 {Fa, time_semicroma},
 {Fa, time_biscroma}, 
 {Fa_diesis, time_semicroma}, 
 {Sol, time_semicroma},
 {Sol, time_biscroma}, 
 {La_bemolle, time_semicroma}, 
 {La, time_semicroma},
 {Si_alto, time_croma}, 
};

void playNote(NOTE note)
{
	if(note.freq != silence)
	{
		reset_timer(0);
		init_timer(0,0,0,3,note.freq);
		enable_timer(0);
	}
	reset_timer(1);
	init_timer(1,0,0,7,note.duration);
	enable_timer(1);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM0->TCR != 0) || (LPC_TIM1->TCR != 0));
}

void playSound(){
	
		static int currentNote = 0;
		static int ticks = 0;
	
		if(!isNotePlaying()){
			++ticks;
			if(ticks == UPTICKS)
			{
				ticks = 0;
				playNote(song[currentNote++]);
			}
		}
		
		if(currentNote == (sizeof(song) / sizeof(song[0]))){
			disable_RIT();
			begin ++;
		}

}