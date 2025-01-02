#include "music.h"
#include "../pacman/pacman.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"

#define UPTICKS 1

extern int begin;
extern player p;
extern ghost g;

volatile NOTE song[] = 
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

volatile NOTE waka_waka[] = {
    {Wa, 5*time_biscroma},  
    {Ka, 5*time_biscroma},   
    {silence, 5*time_biscroma},
};

volatile NOTE ghost_vulnerable[] = {
    {Sol, time_biscroma}, 
    {Mi, time_biscroma}, 
    {Do, time_biscroma}, 
    {Sol, time_biscroma},
    {silence, time_biscroma},
};

NOTE pacman_death[] = {
    {Si, time_croma}, 
    {La, time_croma},
    {La_bemolle, time_semiminima}, 
    {Sol, time_semiminima}, 
    {Fa, time_minima},
    {silence, time_minima},
};

NOTE victory[] = {
    {Do_alto, time_semicroma}, 
    {Mi, time_croma}, 
    {Sol, time_croma}, 
    {Do_alto, time_minima},
    {Mi, time_semiminima}, 
    {Re, time_semiminima}, 
    {Do, time_minima}, 
    {silence, time_minima},
};

NOTE game_over[] = {
    {Fa_diesis, time_croma}, 
    {Mi_bemolle, time_croma}, 
    {Do, time_minima},
    {La_bemolle, time_minima}, 
    {silence, time_semibreve},
};

NOTE power_up[] = {
    {Do, time_semicroma}, 
    {Re, time_semicroma}, 
    {Mi, time_croma},
    {Fa, time_croma},
    {Sol, time_minima},
    {silence, time_biscroma},
};

volatile int songlength = sizeof(song) / sizeof(song[0]);
volatile int wakalength = sizeof(waka_waka) / sizeof(waka_waka[0]);
volatile int ghostvulnerablelength = sizeof(ghost_vulnerable) / sizeof(ghost_vulnerable[0]);
volatile int deathlength = sizeof(pacman_death) / sizeof(pacman_death[0]);
volatile int victorylength = sizeof(victory) / sizeof(victory[0]);
volatile int gameoverlength = sizeof(game_over) / sizeof(game_over[0]);
volatile int poweruplength = sizeof(power_up) / sizeof(power_up[0]);

void playNote(NOTE note)
{
    if (note.freq != silence)
    {
        reset_timer(0);
        init_timer(0, 0, 0, 3, note.freq);
        enable_timer(0);
    }
    reset_timer(1);
    init_timer(1, 0, 0, 7, note.duration);
    enable_timer(1);
}

BOOL isNotePlaying(void)
{
    return ((LPC_TIM0->TCR != 0) || (LPC_TIM1->TCR != 0));
}

void playSound(NOTE *sound, int soundlength){
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
				p.game_state = END;
			} else if(sound == power_up){
				p.life_incremented = false;
			}
			
    }
}
