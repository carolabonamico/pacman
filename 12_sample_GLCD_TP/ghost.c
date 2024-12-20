#include "../pacman/pacman.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */
volatile player ghost;

extern int boardMatrix[ROWS][COLS];
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_RIGHT[BOXSIZE][BOXSIZE];
extern int direction;

int ghostMatrix[BOXSIZE][BOXSIZE] = {
		{0,0,0,2,2,2,2,0,0,0},
		{0,0,2,2,2,2,2,2,0,0},
		{0,2,2,2,2,2,2,2,2,0},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{22,0,2,2,2,2,0,2,2},
		{2,2,0,0,2,2,0,0,2,2},
};

/* -------------------- FUNCTIONS DEFINITION -------------------- */

void move_Ghost(player *ghost, player *p, grid *gr, int direction){
	
	int current_x = ghost->x_pos;
	int current_y = ghost->y_pos;
	
		if(boardMatrix[ghost->next_y][ghost->next_x] != WALL &&
			direction != 0){
			
			if (ghost->next_x == p->x_pos && ghost->next_y == p->y_pos){
				decrement_Life(p);
				ghost->next_x = ghost->x_pos;
				ghost->next_y = ghost->y_pos;
				redraw_Pacman(p->x_pos,p->y_pos,p->x_pos,p->y_pos,direction);
				redraw_Pacman(p->x_pos,p->y_pos,p->x_pos,p->y_pos,direction);
			} else {
					
				if(boardMatrix[current_y][current_x] == GHOSTPOS){
					boardMatrix[current_y][current_x] = NOSPAWN;
				}
				
				// Updating the scores and the matrix
				draw_WallFull(current_x,current_y,Black,BOXSIZE);
				switch(boardMatrix[current_y][current_x]){
					case STDSCORE:
						draw_Circle(current_x,current_y,STDRADIUS,Red);
						break;
					case POWERSCORE:
						draw_Circle(current_x,current_y,POWERRADIUS,Green);
						break;
					default:
						break;
				} 
				
				// Updating ghost	position
				ghost->x_pos = ghost->next_x;
				ghost->y_pos = ghost->next_y;
				draw_Character(ghost->next_x,ghost->next_y,ghostMatrix,Red);
			}
				
		} else {
			ghost->next_x = ghost->x_pos;
			ghost->next_y = ghost->y_pos;
		}	
		
		if(p->nlives <= 0){
			// Pacman disappears as it died
			draw_WallFull(p->x_pos,p->y_pos,Black,BOXSIZE);
			display_GameOver();
		}
			
}

void decrement_Life(player *p){
	
	// Erasing a life
	draw_Character(p->nlives,LIFEPOS,pacmanMatrix_RIGHT,Black);
	// Decrementing life counter
	p->nlives--;
	// Feedback to the loss of a life
	redraw_Pacman(p->x_pos,p->y_pos,p->x_pos,p->y_pos,direction);
	redraw_Pacman(p->x_pos,p->y_pos,p->x_pos,p->y_pos,direction);
	
}