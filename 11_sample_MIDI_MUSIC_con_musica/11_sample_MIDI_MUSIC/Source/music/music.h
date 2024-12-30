#ifndef MUSIC_H
#define MUSIC_H

#define TIMERSCALER 1
#define SECOND 0x17D7840 * TIMERSCALER
#define SPEEDUP 2
#define Tone_Reductor 0.3
#define per_freq 25000000

// num_bit_per_note = 45;

// k = 25Mhz / (note_frequency * num_bit_per_note)

typedef char BOOL;
#define TRUE 1
#define FALSE 0

typedef enum note_durations
{
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), // 1/128
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), // 1/64
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), // 1/32
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), // 1/16
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), // 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), // 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), // 1
} NOTE_DURATION;

typedef enum frequencies
{
  Fa = (unsigned int) (per_freq / (349 * Tone_Reductor * 45)),
  Fa_diesis = (unsigned int) (per_freq / (370 * Tone_Reductor * 45)),
  Sol = (unsigned int) (per_freq / (392 * Tone_Reductor * 45)),
  La = (unsigned int) (per_freq / (440 * Tone_Reductor * 45)), 
  La_bemolle = (unsigned int) (per_freq / (415 * Tone_Reductor * 45)),
  Si = (unsigned int) (per_freq / (247 * Tone_Reductor * 45)),
  Si_alto = (unsigned int) (per_freq / (494 * Tone_Reductor * 45)),
  Mi = (unsigned int) (per_freq / (330 * Tone_Reductor * 45)),
  Mi_bemolle = (unsigned int) (per_freq / (311 * Tone_Reductor * 45)),
  Do = (unsigned int) (per_freq / (262 * Tone_Reductor * 45)), 
  Do_alto = (unsigned int) (per_freq / (523 * Tone_Reductor * 45)),
  Re = (unsigned int) (per_freq / (294 * Tone_Reductor * 45)), 
  
  Wa = (unsigned int) (per_freq / (200 * 45)),
  Ka = (unsigned int) (per_freq / (600 * 45)),
  silence = 0,
} FREQUENCY;


typedef struct 
{
	FREQUENCY freq;
	NOTE_DURATION duration;
	unsigned int intensity;
} NOTE;

void playNote(NOTE note);
BOOL isNotePlaying(void);

#endif
/* EOF */




