#ifndef MUSIC_H
#define MUSIC_H

#include <stdbool.h>

#define TIMERSCALER 1
#define SECOND 0x17D7840 * TIMERSCALER
#define SPEEDUP 2
#define Tone_Reductor 0.3
#define SYSFREQ 25000000

// num_bit_per_note = 45;
// k = 25Mhz / (note_frequency * num_bit_per_note)

typedef enum note_durations {
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), 		// 1/128
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), 				// 1/64
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), 				// 1/32
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), 						// 1/16
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), 				// 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), 						// 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), 								// 1
} note_duration;

typedef enum frequencies {
  Fa = (unsigned int) (SYSFREQ / (349 * Tone_Reductor * 45)),
  Fa_diesis = (unsigned int) (SYSFREQ / (370 * Tone_Reductor * 45)),
  Sol = (unsigned int) (SYSFREQ / (392 * Tone_Reductor * 45)),
  La = (unsigned int) (SYSFREQ / (440 * Tone_Reductor * 45)), 
  La_bemolle = (unsigned int) (SYSFREQ / (415 * Tone_Reductor * 45)),
  Si = (unsigned int) (SYSFREQ / (247 * Tone_Reductor * 45)),
  Si_alto = (unsigned int) (SYSFREQ / (494 * Tone_Reductor * 45)),
  Mi = (unsigned int) (SYSFREQ / (330 * Tone_Reductor * 45)),
  Mi_bemolle = (unsigned int) (SYSFREQ / (311 * Tone_Reductor * 45)),
  Do = (unsigned int) (SYSFREQ / (262 * Tone_Reductor * 45)), 
  Do_alto = (unsigned int) (SYSFREQ / (523 * Tone_Reductor * 45)),
  Re = (unsigned int) (SYSFREQ / (294 * Tone_Reductor * 45)), 
  
  Wa = (unsigned int) (SYSFREQ / (200 * 45)),
  Ka = (unsigned int) (SYSFREQ / (600 * 45)),
  silence = 0,
} frequency;

typedef struct {
	frequency freq;
	note_duration duration;
	unsigned int intensity;
} note;

/* music.c */
void playNote(note note);
bool isNotePlaying(void);
void playSound(note *sound, int soundlength);

#endif
/* EOF */
