#include "../pacman/pacman.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern int boardMatrix[ROWS][COLS];
extern int direction;
extern ghost g;

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

void move_Ghost_test(ghost *ghost, route *r, player *p){
	
	int current_x = ghost->ghost_coord.pos.x;
	int current_y = ghost->ghost_coord.pos.y;
	
	r->path_length --;
	ghost->ghost_coord.next_pos.x = r->path[r->path_length].x;
	ghost->ghost_coord.next_pos.y = r->path[r->path_length].y;
	
			if(ghost->ghost_coord.next_pos.x == p->player_coord.pos.x && 
				ghost->ghost_coord.next_pos.y == p->player_coord.pos.y){
					
				decrement_Life(p);				
				ghost->ghost_coord.next_pos.x = ghost->ghost_coord.pos.x;
				ghost->ghost_coord.next_pos.y = ghost->ghost_coord.pos.y;
				redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
				redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
				init_Route(r);
						
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
		
		if(p->nlives <= 0){
			// Pacman disappears as it died
			draw_WallFull(p->player_coord.pos.x,p->player_coord.pos.y,Black,BOXSIZE);
			display_GameOver();
		}
			
}