#ifndef __Packman_H
#define __Packman_H

#include <stdbool.h>

// DEFINING THE STRUCTURES

typedef struct{
	int x_pos;						// x coord. of the player
	int y_pos;						// y coord. of the player
	bool status;					// status of the player (passed or not on a pixel)
} player;

typedef struct{
	int current_score;	
	int best_score;
	int n_lives;
} match;

typedef struct{
	int y_min;
	int y_max;
} tunnel;

typedef struct{
	int x_pos;
	int y_pos;
	bool status;
	int score;
} stdpill;

typedef struct{
	int x_pos;
	int y_pos;
	bool status;
	int score;
} powerpill;

#endif