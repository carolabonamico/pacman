#ifndef __Pacman_H
#define __Pacman_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>							// To perform A* algorithm

/*
---------- DEFINE SECTION ----------
*/

// DEFINING THE SCORES AND NUMBER OF PILLS 
#define STDSCORE 10							// Score associated to a standard pill
#define POWERSCORE 50						// Score associated to a power pill
#define BLINKYSCORE 100					// Score associated to Blinky
#define NMAXPOWERPILLS 6				// Maximum number of power pills
#define N_STDPILLS 240					// Tot number of standard pills
#define INITLIVES 1							// Initial number of lives
#define INITSCORE 0							// Initial score
#define NEWLIFE 1000						// Threshold for the genaration of a new life

// DEFINING THE GRID FOR THE MAZE
#define COLS 24									// Number of colums of the matrix
#define ROWS 24									// Number of rows of the matrix
#define WALL 1									// Wall in the maze
#define EMPTY 0									// Empty cell in the maze
#define LEFTTUNNEL 8						// Encoding of the left tunnel
#define RIGHTTUNNEL 9						// Encoding for the right tunnel
#define PACMANPOS 2							// Starting position of player
#define NOSPAWN 3								// Empty space in the grid where power pills cannot spawn
#define DOOR 4									// Door for the ghost spawn
#define GHOSTPOS 5							// Starting position of ghost

#define CONTINUE 0							// Continue flag
#define PAUSE 1									// Pause flag
#define GAMETIME 60							// Match duration
#define END 2;									// End match flag

// DEFINE FOR THE PRINTING FUNCTIONS
#define OFFSET 4
#define BOXSIZE 10							// Size of each pixel block
#define UPPERMENU 50						// Padding for the upper interface
#define PACMANRADIUS 16					// Radius^2 for drawing Pacman
#define POWERRADIUS 9						// Radius^2 for drawing power pills
#define STDRADIUS 1							// Radius^2 for drawing standard pills
#define	LIFEPOS 25							// Y coordinate for printing lives

// DEFINE FOR THE DIRECTIONS OF THE MOVEMENT OF THE PLAYER
#define DIRSELECTION 1
#define DIRUP 2
#define DIRDOWN 3
#define DIRLEFT 4
#define DIRRIGHT 5

//#define ROW 9
//#define COL 10

#define M_PI 3.14159265358979323846

/*
---------- STRUCTURE SECTION ----------
*/

typedef struct{
	int x;								
	int y;							
} node;

typedef struct{
	node pos;
	node next_pos;
} coord;

typedef struct{
	coord player_coord;
	int nlives;										// Number of current lives
	int score;										// Current score
	int game_state;								// If game is in pause or not
	int last_direction; 					// Last coded direction taken before a wall
	bool life_incremented;				// Set to true as the life is incremented
} player;

typedef struct{
	int n_stdpills;								// Number of standard pills on the grid
	int n_powerpills;							// Number of power pills on the grid
} grid;


/*
---------- FUNCTIONS DECLARATION ----------
*/

/* pacman.c */
// Drawing functions
void draw_WallFull(uint16_t x, uint16_t y, uint32_t color, int size);
void draw_Pacman(int x, int y, int radius_2, int color);
void draw_WallEmpty(uint16_t x, uint16_t y, uint32_t color);
void draw_Circle(int x, int y, int radius_2, int color);
void draw_Pacman(int x, int y, int radius_2, int color);
void draw_Pacman_new(uint16_t x, uint16_t y, int color);

// Update functions
void update_TimerHeader(int countdown);
void update_ScoreHeader(int score);
void update_NewLife(player *p);
// Move layer functions
void controller_Player(int direction, coord *c);
void move_Player(player *p, grid *gr, int direction);

// Power pills spawn functions
int rand_Range(int min, int max);
void rand_PowerPill(grid *gr, player *p);

void draw_Character(uint16_t x, uint16_t y, int matrix[BOXSIZE][BOXSIZE], int color);
void redraw_Pacman(int current_x, int current_y, int next_x, int next_y, int direction);

/* layout.c */
void draw_LastDir(int x, int y);
// Pop-up functions
void clear_Section(int i, int j, int direction, player *p);
void menu_Pause(player *p, int direction);
void display_GameOver(void);
void display_Win(void);

/* initialization.c */
void init_Header(void);
void init_GameSpace(grid *gr);
void init_Player(player *p);
void init_Grid(grid *gr);
int sub_Counter(int elapsed_time,int *sub_second_count, int current_interval, int ticks_per_second);

#endif