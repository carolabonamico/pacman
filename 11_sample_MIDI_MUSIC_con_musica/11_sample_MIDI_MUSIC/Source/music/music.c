#include "music.h"
#include "../pacman/pacman.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"

#define UPTICKS 1

extern int begin;
extern player p;
extern ghost g;
extern grid gr;

volatile note song[] = {
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
 {Si_alto, time_croma}
};

volatile note waka_waka[] = {
	{Do, time_biscroma/4}, 				// First "waka"
  {Re, time_biscroma/4}, 				// Second "waka"
  {silence, time_biscroma / 2} 	// Short pause between eventual cycles
};

volatile note ghost_vulnerable[] = {
  {Sol, time_biscroma}, 
  {Mi, time_biscroma}, 
  {Do, time_biscroma}, 
  {Sol, time_biscroma},
  {silence, time_biscroma}
};

volatile note pacman_death[] = {
  {Si, time_croma}, 
  {La, time_croma},
  {La_bemolle, time_semiminima}, 
  {Sol, time_semiminima}, 
  {Fa, time_minima},
  {silence, time_minima}
};

volatile note victory[] = {
  {Do_alto, time_semicroma}, 
  {Mi, time_croma}, 
  {Sol, time_croma}, 
  {Do_alto, time_minima},
  {Mi, time_semiminima}, 
  {Re, time_semiminima}, 
  {Do, time_minima}, 
  {silence, time_minima}
};

volatile note game_over[] = {
  {Fa_diesis, time_croma}, 
  {Mi_bemolle, time_croma}, 
  {Do, time_minima},
  {La_bemolle, time_minima}, 
  {silence, time_semibreve}
};

volatile note power_up[] = {
	{Do, time_semicroma},      
  {Mi, time_semicroma},
  {Sol, time_semicroma},
  {Do_alto, time_croma},  
  {Re, time_croma},       
  {Mi, time_minima}
};

volatile note eating_sound[] = {
	{Do, time_semicroma},       
  {Re, time_semicroma},       
  {Mi, time_semicroma},       
  {Fa, time_semicroma},       
  {Sol, time_semicroma}
};

volatile int songlength = sizeof(song) / sizeof(song[0]);
volatile int wakalength = sizeof(waka_waka) / sizeof(waka_waka[0]);
volatile int ghostvulnerablelength = sizeof(ghost_vulnerable) / sizeof(ghost_vulnerable[0]);
volatile int deathlength = sizeof(pacman_death) / sizeof(pacman_death[0]);
volatile int victorylength = sizeof(victory) / sizeof(victory[0]);
volatile int gameoverlength = sizeof(game_over) / sizeof(game_over[0]);
volatile int poweruplength = sizeof(power_up) / sizeof(power_up[0]);
volatile int eatlength = sizeof(eating_sound) / sizeof(eating_sound[0]);

void playNote(note musical_note)
{
    if (musical_note.freq != silence)
    {
        reset_timer(0);
        init_timer(0, 0, 0, 3, musical_note.freq);
        enable_timer(0);
    }
    reset_timer(1);
    init_timer(1, 0, 0, 7, musical_note.duration);
    enable_timer(1);
}

bool isNotePlaying(void)
{
    return ((LPC_TIM0->TCR != 0) || (LPC_TIM1->TCR != 0));
}

void playSound(note *sound, int soundlength){
    static int currentNote = 0;
    static int ticks = 0;

    if (!isNotePlaying()){
        ++ticks;
        if (ticks == UPTICKS){
            ticks = 0;
            playNote(sound[currentNote++]);
        }
    }
    if (currentNote == soundlength){
			currentNote = 0;
			gr.sound_effect_triggered = false;
			if(sound == song){
        disable_RIT();
        begin++;
			} else if(sound == waka_waka){
				p.waka_trigger = false;
			} else if(sound == ghost_vulnerable){
				g.play_vulnerable = false;
			} else if(sound == pacman_death || sound == victory || sound == game_over){
				disable_timer(0);
				disable_timer(1);
				disable_RIT();
			} else if(sound == power_up){
				p.life_incremented = false;
			} else if(sound == eating_sound){
				g.play_eaten = false;
			}
			
    }
}
