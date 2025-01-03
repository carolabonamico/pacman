#include "pacman.h"
#include "../GLCD/GLCD.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern player p;
extern ghost g;
extern grid gr;
extern int direction;
extern int flag;
volatile int life_increment_threshold = NEWLIFE;
extern int seed;
extern int boardMatrix[ROWS][COLS];
extern int ghostMatrix[BOXSIZE][BOXSIZE];
volatile int rand_time = 5*SYSFREQ;

/* REMINDER
#define  MAX_X  240
#define  MAX_Y  320 
*/

int pacmanMatrix_UP[BOXSIZE][BOXSIZE] = {
		{0,0,0,0,0,0,0,0,0,0},
		{0,2,0,0,0,0,0,0,2,0},
		{2,2,2,0,0,0,0,2,2,2},		
		{2,2,2,2,0,0,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},		
		{0,2,2,2,2,2,2,2,2,0},
		{0,0,2,2,2,2,2,2,0,0},
		{0,0,0,2,2,2,2,0,0,0},		
		{0,0,0,0,0,0,0,0,0,0}
};

int pacmanMatrix_DOWN[BOXSIZE][BOXSIZE] = {
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,2,2,2,2,0,0,0},
		{0,0,2,2,2,2,2,2,0,0},
		{0,2,2,2,2,2,2,2,2,0},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,0,0,2,2,2,2},
		{2,2,2,0,0,0,0,2,2,2},
		{0,2,0,0,0,0,0,0,2,0},			
		{0,0,0,0,0,0,0,0,0,0}
};

int pacmanMatrix_LEFT[BOXSIZE][BOXSIZE] = {
		{0,0,2,2,2,2,0,0,0,0},
		{0,2,2,2,2,2,2,0,0,0},
		{0,0,2,2,2,2,2,2,0,0},
		{0,0,0,2,2,2,2,2,2,0},
		{0,0,0,0,2,2,2,2,2,0},
		{0,0,0,0,2,2,2,2,2,0},
		{0,0,0,2,2,2,2,2,2,0},
		{0,0,2,2,2,2,2,2,0,0},
		{0,2,2,2,2,2,2,0,0,0},			
		{0,0,2,2,2,2,0,0,0,0}
};

int pacmanMatrix_RIGHT[BOXSIZE][BOXSIZE] = {
		{0,0,0,2,2,2,2,0,0,0},
		{0,0,2,2,2,2,2,2,0,0},
		{0,2,2,2,2,2,2,0,0,0},
		{2,2,2,2,2,2,0,0,0,0},
		{2,2,2,2,2,0,0,0,0,0},
		{2,2,2,2,2,0,0,0,0,0},
		{2,2,2,2,2,2,0,0,0,0},
		{0,2,2,2,2,2,2,0,0,0},			
		{0,0,2,2,2,2,2,2,0,0},
		{0,0,0,2,2,2,2,0,0,0},
};
			

/* -------------------- FUNCTIONS DEFINITION -------------------- */

void draw_WallFull(uint16_t x, uint16_t y, uint32_t color, int size){
	int i,j;
	uint16_t x_start = x*BOXSIZE;
	uint16_t y_start = y*BOXSIZE+UPPERMENU;
	
	for (i=0;i<size;i++){
		for (j=0;j<size;j++){
			LCD_SetPoint(x_start+i, y_start+j, color); 
		}
	}
}

void draw_WallEmpty(uint16_t x, uint16_t y, uint32_t color){
	int i,j;
	uint16_t x_start = x*BOXSIZE;
	uint16_t y_start = y*BOXSIZE+UPPERMENU;
	
	int x1 = x_start, y1 = y_start;  															// Top-left corner
  int x2 = x_start + BOXSIZE - 1, y2 = y_start;  								// Top-right corner
  int x3 = x_start + BOXSIZE - 1, y3 = y_start + BOXSIZE - 1;  	// Bottom-right corner
  int x4 = x_start, y4 = y_start + BOXSIZE - 1; 								// Bottom-left corner

  // Draw the four edges of the square
  LCD_DrawLine(x1, y1, x2, y2, Blue);  													// Top edge
  LCD_DrawLine(x2, y2, x3, y3, Blue);  													// Right edge
  LCD_DrawLine(x3, y3, x4, y4, Blue); 							 						// Bottom edge
  LCD_DrawLine(x4, y4, x1, y1, Blue);  													// Left edge
	LCD_DrawLine(x1, y1+1, x2, y2+1, Blue);  											// Top edge
  LCD_DrawLine(x2-1, y2, x3-1, y3, Blue);  											// Right edge
  LCD_DrawLine(x3, y3-1, x4, y4-1, Blue); 							 				// Bottom edge
  LCD_DrawLine(x4+1, y4, x1+1, y1, Blue);  											// Left edge
}

void draw_Circle(int x, int y, int radius_2, int color){

	int i,j;
	uint16_t x_start = x*BOXSIZE;
	uint16_t y_start = y*BOXSIZE+UPPERMENU;
	float distance_2;
	
	int x_center = BOXSIZE/2;
	int y_center = BOXSIZE/2;
	for(i=0;i<BOXSIZE;i++){
		for(j=0;j<BOXSIZE;j++){
			distance_2 = pow(i-x_center,2)+pow(j-y_center,2);
			if(distance_2<=radius_2){
					LCD_SetPoint(x_start+i,y_start+j,color);
			}
		}
	}
	
}

void update_TimerHeader(int countdown){
	char str_timer[5];
	
	if(countdown>=10){
		sprintf(str_timer, "%ds", countdown); 
	} else {
		sprintf(str_timer, "0%ds", countdown); 
	}
	
	GUI_Text(0,0, (unsigned char*) "GAME OVER IN",White,Black);
	GUI_Text(0,MAX_Y/16, (unsigned char*) str_timer ,White,Black);
}

void update_ScoreHeader(int score){
	char str_score[10];
	sprintf(str_score, "%d", score); 
	GUI_Text((MAX_X/3)*2,0,(unsigned char*) "SCORE",White,Black);
	GUI_Text((MAX_X/3)*2,MAX_Y/16,(unsigned char*) str_score ,White,Black);
}

void init_Perc(route *perc){
	perc->path_length = 0;
}

// LIFE INCREMENT FUNCTION
void update_NewLife(player *p){

	if(p->score >= life_increment_threshold){
		p->nlives++;
		p->life_incremented = true;
		life_increment_threshold += NEWLIFE;
		draw_Character(p->nlives,LIFEPOS,pacmanMatrix_RIGHT,Yellow);
		
	}
}

// FUNCTION TO COMPUTE NEXT POSITION OF PLAYER
void controller_Player(int direction, coord *c){
	
	if(direction == DIRUP){
		c->next_pos.x = c->pos.x;
		c->next_pos.y = c->pos.y-1; 
	} 
	else if (direction == DIRDOWN){
		c->next_pos.x = c->pos.x;
		c->next_pos.y = c->pos.y+1;  
	} else if (direction == DIRLEFT && boardMatrix[c->pos.y][c->pos.x] != LEFTTUNNEL){
		c->next_pos.x = c->pos.x-1;
		c->next_pos.y = c->pos.y; 
	}	else if (direction == DIRLEFT && boardMatrix[c->pos.y][c->pos.x] == LEFTTUNNEL){
		c->next_pos.x = COLS-1;
		c->next_pos.y = c->pos.y;  
	} else if (direction == DIRRIGHT && boardMatrix[c->pos.y][c->pos.x] != RIGHTTUNNEL){
		c->next_pos.x = c->pos.x+1;
		c->next_pos.y = c->pos.y; 
	}	else if (direction == DIRRIGHT && boardMatrix[c->pos.y][c->pos.x] == RIGHTTUNNEL){
		c->next_pos.x = 0;
		c->next_pos.y = c->pos.y;  
	}

}

// FUNCTION TO PERFORM NEXT STEP OF THE PLAYER
void move_Player(player *p, grid *gr, int direction, ghost *g) {
    int current_x = p->player_coord.pos.x;
    int current_y = p->player_coord.pos.y;

    if (boardMatrix[p->player_coord.next_pos.y][p->player_coord.next_pos.x] != WALL &&
        boardMatrix[p->player_coord.next_pos.y][p->player_coord.next_pos.x] != DOOR &&
        direction != 0) {

        // Check for collision with ghost
        if (p->player_coord.next_pos.x == g->ghost_coord.pos.x &&
            p->player_coord.next_pos.y == g->ghost_coord.pos.y && !g->eaten) {
            
            if (g->vulnerable) {
                // Pacman eats the ghost
                p->score += BLINKYSCORE;
                update_ScoreHeader(p->score);
                g->eaten = true;
								g->play_eaten = true;
                g->vulnerable = false;

                // Remove ghost from the maze
                boardMatrix[g->ghost_coord.pos.y][g->ghost_coord.pos.x] = EMPTY;
            } else {
                // If the ghost is not vulnerable, Pacman cannot eat it
                // Stop Pacman's movement
                p->player_coord.next_pos.x = current_x;
                p->player_coord.next_pos.y = current_y;
                direction = 0;

                return; // Exit as Pacman stays in place
            }
        }

        // Update Pacman's position
        p->player_coord.pos.x = p->player_coord.next_pos.x;
        p->player_coord.pos.y = p->player_coord.next_pos.y;

        redraw_Pacman(current_x, current_y, p->player_coord.pos.x, p->player_coord.pos.y, direction);
				p->last_direction = direction;

        // Update the scores and grid
        switch (boardMatrix[p->player_coord.next_pos.y][p->player_coord.pos.x]) {
            case STDSCORE:
                p->score += STDSCORE;
                update_ScoreHeader(p->score);
                boardMatrix[p->player_coord.next_pos.y][p->player_coord.pos.x] = EMPTY;
                gr->n_stdpills--;
                update_NewLife(p);
								p->waka_trigger = true;
                break;
            case POWERSCORE:
                p->score += POWERSCORE;
                update_ScoreHeader(p->score);
                boardMatrix[p->player_coord.next_pos.y][p->player_coord.pos.x] = EMPTY;
                gr->n_powerpills--;
                update_NewLife(p);
                g->vulnerable = true; // Make ghost vulnerable
								g->play_vulnerable = true;
								g->reset_counter = true;
                break;
        }
    } else {
        // Pacman stays in place if the move is invalid
        direction = 0;
        p->player_coord.next_pos.x = p->player_coord.pos.x;
        p->player_coord.next_pos.y = p->player_coord.pos.y;
    }
}


int rand_Range(int min, int max){
    return min + (rand() % (max - min + 1));
}

void rand_PowerPill(grid *gr, player *p){
	
		int x_rand = 0;
		int y_rand = 0;
		
		// Generate random column index
		srand(seed);
		
		do{
			x_rand = rand_Range(0,COLS-1);
			y_rand = rand_Range(0,ROWS-1);
		}

		while(boardMatrix[y_rand][x_rand] == WALL || 
					boardMatrix[y_rand][x_rand] == NOSPAWN ||
					boardMatrix[y_rand][x_rand] == LEFTTUNNEL ||
					boardMatrix[y_rand][x_rand] == RIGHTTUNNEL ||
					boardMatrix[y_rand][x_rand] == POWERSCORE ||
					(x_rand == p->player_coord.pos.x &&
					y_rand == p->player_coord.pos.y)
		);
		
		if(boardMatrix[y_rand][x_rand] == STDSCORE){
			gr->n_stdpills--;
		}
		gr->n_powerpills++;
		boardMatrix[y_rand][x_rand] = POWERSCORE;
		draw_Circle(x_rand,y_rand,POWERRADIUS,Green);
		
		// Implementing random spawn in time
		seed += p->score;
		srand(seed);
		if(rand_time < 0xFFFFFFFF) rand_time += rand_Range(10,15)*SYSFREQ;
		if(rand_time >= 0xFFFFFFFF) rand_time = 0xFFFFFFFF;

}

void draw_Pacman_new(uint16_t x, uint16_t y, int color){

	int i, j;
	uint32_t bgcolor;
  uint16_t x_start = x * BOXSIZE;
  uint16_t y_start = y * BOXSIZE + UPPERMENU;
	
	for(i=0;i<BOXSIZE;i++){
		for(j=0;j<BOXSIZE;j++){
			switch(pacmanMatrix_UP[j][i]){
				case 0:
					bgcolor = Black;
				break;
				case 2:
					bgcolor = color;
					break;
				default:
					break;
			}
			LCD_SetPoint(x_start+i,y_start+j,bgcolor);
		}
	}

}

void draw_Character(uint16_t x, uint16_t y, int matrix[BOXSIZE][BOXSIZE], int color){

	int i, j;
	uint32_t bgcolor;
  uint16_t x_start = x * BOXSIZE;
  uint16_t y_start = y * BOXSIZE + UPPERMENU;
	
	for(i=0;i<BOXSIZE;i++){
		for(j=0;j<BOXSIZE;j++){
			switch(matrix[j][i]){
				case 0:
					bgcolor = Black;
				break;
				case 2:
					bgcolor = color;
					break;
				default:
					break;
			}
			LCD_SetPoint(x_start+i,y_start+j,bgcolor);
		}
	}

}

void redraw_Pacman(int current_x, int current_y, int next_x, int next_y, int direction){
		// Redrawing
		draw_WallFull(current_x,current_y,Black,BOXSIZE);
		if(direction == DIRUP){
			draw_Character(next_x,next_y,pacmanMatrix_UP,Yellow);
		} else if (direction == DIRDOWN){
			draw_Character(next_x,next_y,pacmanMatrix_DOWN,Yellow); 
		} else if (direction == DIRLEFT){
			draw_Character(next_x,next_y,pacmanMatrix_LEFT,Yellow);
		}	else if (direction == DIRRIGHT){
			draw_Character(next_x,next_y,pacmanMatrix_RIGHT,Yellow); 
		}
}