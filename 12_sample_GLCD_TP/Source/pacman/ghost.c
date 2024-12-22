#include "../pacman/pacman.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern int boardMatrix[ROWS][COLS];
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_RIGHT[BOXSIZE][BOXSIZE];
extern int direction;
extern ghost g;

/* -------------------- FUNCTIONS DEFINITION -------------------- */
void init_Ghost(ghost *g){
	g->ghost_coord.pos.x = 11;
		g->ghost_coord.pos.y = 11;
	g->path_length = 0;
}

void move_Ghost(ghost *ghost, player *p, grid *gr, int direction){
	
	int current_x = ghost->ghost_coord.pos.x;
	int current_y = ghost->ghost_coord.pos.y;
	
		if(boardMatrix[ghost->ghost_coord.next_pos.y][ghost->ghost_coord.next_pos.x] != WALL &&
			direction != 0){
			
			if (ghost->ghost_coord.next_pos.x == p->player_coord.next_pos.x && 
					ghost->ghost_coord.next_pos.y == p->player_coord.next_pos.y){
				decrement_Life(p);
				ghost->ghost_coord.next_pos.x = ghost->ghost_coord.pos.x;
				ghost->ghost_coord.next_pos.y = ghost->ghost_coord.pos.y;
				redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
				redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
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
				ghost->ghost_coord.pos.x = ghost->ghost_coord.next_pos.x;
				ghost->ghost_coord.pos.y = ghost->ghost_coord.next_pos.y;
				draw_Character(ghost->ghost_coord.next_pos.x,ghost->ghost_coord.next_pos.y,ghostMatrix,Red);
			}
				
		} else {
			ghost->ghost_coord.next_pos.x = ghost->ghost_coord.pos.x;
			ghost->ghost_coord.next_pos.y = ghost->ghost_coord.pos.y;
		}	
		
		if(p->nlives <= 0){
			// Pacman disappears as it died
			draw_WallFull(p->player_coord.pos.x,p->player_coord.pos.y,Black,BOXSIZE);
			display_GameOver();
		}
			
}

void decrement_Life(player *p){
	
	// Erasing a life
	draw_Character(p->nlives,LIFEPOS,pacmanMatrix_RIGHT,Black);
	// Decrementing life counter
	p->nlives--;
	// Feedback to the loss of a life
	redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
	redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
	
}
