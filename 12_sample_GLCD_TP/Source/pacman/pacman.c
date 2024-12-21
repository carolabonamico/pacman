#include "pacman.h"
#include "../GLCD/GLCD.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "../timer/timer.h"

// DECLARING VARIABLES
extern player p;
extern ghost g;
extern grid gr;
extern int direction;
extern int flag;
volatile int life_increment_threshold = NEWLIFE;
extern int seed;

/* REMINDER
#define  MAX_X  240
#define  MAX_Y  320 
*/

// Board matrix
volatile int boardMatrix[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,10,10,10,10,10,1},
		{1,10,1,1,10,1,1,1,1,1,10,1,1,10,1,1,1,1,1,10,1,1,10,1},
		{1,10,1,1,10,1,1,1,1,1,10,1,1,10,1,1,1,1,1,10,1,1,10,1},
		{1,10,10,10,10,10,10,10,10,10,10,10,10,10,10,50,10,10,10,10,10,10,10,1},
		{1,10,1,1,10,1,1,10,1,1,1,1,1,1,1,1,10,1,1,10,1,1,10,1},
		{1,10,1,1,10,1,1,10,10,10,10,1,1,10,10,10,10,1,1,10,1,1,10,1},
		{1,10,1,1,10,1,1,1,50,1,10,1,1,10,1,10,1,1,1,10,1,1,10,1},
		{1,10,10,10,10,10,10,10,10,1,10,1,1,10,1,10,10,10,10,10,10,10,10,1},
		{1,1,1,1,10,1,1,1,10,10,10,10,10,10,50,10,1,1,1,10,1,1,1,1},
		{1,1,1,1,10,1,1,1,10,1,1,DOOR,DOOR,1,1,10,1,1,1,10,1,1,1,1},
		{LEFTTUNNEL,NOSPAWN,NOSPAWN,NOSPAWN,10,10,10,10,10,1,NOSPAWN,GHOSTPOS,NOSPAWN,NOSPAWN,1,10,10,10,10,10,NOSPAWN,NOSPAWN,NOSPAWN,RIGHTTUNNEL},		// Tunnel
		{1,1,1,1,10,1,1,1,10,1,NOSPAWN,NOSPAWN,NOSPAWN,NOSPAWN,1,10,1,1,1,10,1,1,1,1},			
		{1,1,1,1,10,1,1,1,10,1,1,1,1,1,1,10,1,1,1,10,1,1,1,1},
		{1,10,10,10,10,10,10,10,10,10,10,10,10,10,10,50,10,10,10,10,10,10,10,1},
		{1,10,1,1,1,1,1,10,1,1,1,1,1,1,1,1,10,1,1,1,1,1,10,1},
		{1,10,10,10,1,10,10,10,10,10,10,1,1,10,10,10,10,10,10,10,10,10,10,1},
		{1,1,1,10,1,10,1,1,1,1,10,1,1,10,1,1,1,1,10,1,10,1,1,1},
		{1,1,1,10,1,10,10,10,10,10,10,10,10,10,50,10,10,10,10,1,10,1,1,1},
		{1,1,1,10,1,10,1,10,1,1,1,1,1,1,1,1,10,1,10,1,10,1,1,1},
		{1,10,10,10,10,10,1,10,10,10,10,1,1,10,10,10,10,1,10,10,10,10,10,1},
		{1,10,1,1,1,1,1,1,1,1,10,1,1,10,1,1,1,1,1,1,1,1,10,1},
		{1,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,50,10,PACMANPOS,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

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
			

/*
------------- DRAW FUNCTIONS -------------
*/

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

/*
------------- INITIALIZATION FUNCTIONS -------------
*/

/* FUNCTION TO INITIALIZE THE INTERFACE */
void init_Header(){
	GUI_Text(0,0,(unsigned char*) "GAME OVER IN",White,Black);
	GUI_Text(0,20,(unsigned char*) "60s" ,White,Black);
	
	GUI_Text((MAX_X/3)*2,0,(unsigned char*) "SCORE",White,Black);
	GUI_Text((MAX_X/3)*2,MAX_Y/16,(unsigned char*) "0",White,Black);
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

/* FUNCTION TO INITIALIZE THE MAZE*/
void init_GameSpace(grid *gr){
	uint16_t x,y;
	uint32_t color;

  for(y=0;y<ROWS;y++){
		for(x=0;x<COLS;x++){
					
			switch(boardMatrix[y][x]){	
				// Blue wall
				case WALL: 
					color = Blue; 					
					draw_WallEmpty(x,y,color);
					break; 
				// Black path
				case 0: 
					color = Black; 				
					break; 		
				// Red standard pill 
				case STDSCORE: 
					color= Red; 					
					draw_Circle(x,y,STDRADIUS,color);
					//draw_STDpill(x,y,color);
					gr->n_stdpills++;
					break; 
				// Green power pill
				case POWERSCORE: 
					color = Green; 				// Power pills
					//draw_POWERpill(x,y,color);
					draw_Circle(x,y,POWERRADIUS,color);
					gr->n_powerpills++;
					break;		
				case PACMANPOS:
					draw_Character(COLS-2,ROWS-2,pacmanMatrix_LEFT,Yellow);
					break;
				case GHOSTPOS:
					draw_Character(x,y,ghostMatrix,Red);
					break;
				default:
					break;
			}
    } 		
	}
	
	// Printing the first life
	draw_Character(1,LIFEPOS,pacmanMatrix_RIGHT,Yellow);
}

// PLAYER INITIALIZATION
void init_Player(player *p){
	
	// Function to initialize the player structure
	p->player_coord.x_pos = ROWS-2;
	p->player_coord.y_pos = COLS-2;
	p->nlives = INITLIVES;
	p->score = INITSCORE;
	p->game_state = CONTINUE;
	
}

void init_Grid(grid *gr){
	gr->n_powerpills=0;
	gr->n_stdpills=0;
}

void init_Perc(percorso *perc){
	perc->path_length = 0;
}

// LIFE INCREMENT FUNCTION
void update_NewLife(player *p){

	if(p->score >= life_increment_threshold){
		p->nlives++;
		life_increment_threshold += NEWLIFE;
		draw_Character(p->nlives,LIFEPOS,pacmanMatrix_RIGHT,Yellow);
		
	}
}

// FUNCTION TO COMPUTE NEXT POSITION OF PLAYER
void controller_Player(int direction, coord *c){
	
	if(direction == DIRUP){
		c->next_x = c->x_pos;
		c->next_y = c->y_pos-1; 
	} 
	else if (direction == DIRDOWN){
		c->next_x = c->x_pos;
		c->next_y = c->y_pos+1;  
	} else if (direction == DIRLEFT && boardMatrix[c->y_pos][c->x_pos] != LEFTTUNNEL){
		c->next_x = c->x_pos-1;
		c->next_y = c->y_pos; 
	}	else if (direction == DIRLEFT && boardMatrix[c->y_pos][c->x_pos] == LEFTTUNNEL){
		c->next_x = COLS-1;
		c->next_y = c->y_pos;  
	} else if (direction == DIRRIGHT && boardMatrix[c->y_pos][c->x_pos] != RIGHTTUNNEL){
		c->next_x = c->x_pos+1;
		c->next_y = c->y_pos; 
	}	else if (direction == DIRRIGHT && boardMatrix[c->y_pos][c->x_pos] == RIGHTTUNNEL){
		c->next_x = 0;
		c->next_y = c->y_pos;  
	}

}

// FUNCTION TO PERFORM NEXT STEP OF THE PLAYER
void move_Player(player *p, grid *gr, int direction){
	
	int current_x = p->player_coord.x_pos;
	int current_y = p->player_coord.y_pos;
	
	if(boardMatrix[p->player_coord.next_y][p->player_coord.next_x] != WALL && 
		boardMatrix[p->player_coord.next_y][p->player_coord.next_x] != DOOR &&
		direction != 0){
		// Updating player position
		p->player_coord.x_pos = p->player_coord.next_x;
		p->player_coord.y_pos = p->player_coord.next_y;
			
		redraw_Pacman(current_x,current_y,p->player_coord.next_x,p->player_coord.next_y,direction);
		
		// Updating the scores and the matrix
		switch(boardMatrix[p->player_coord.next_y][p->player_coord.next_x]){
			case STDSCORE:
				p->score += STDSCORE;
				update_ScoreHeader(p->score);
				boardMatrix[p->player_coord.next_y][p->player_coord.next_x] = EMPTY;
				gr->n_stdpills--;
				// Check for incrementing lives
				update_NewLife(p);
				break;
			case POWERSCORE:
				p->score += POWERSCORE;
				update_ScoreHeader(p->score);
				boardMatrix[p->player_coord.next_y][p->player_coord.next_x] = EMPTY;
				gr->n_powerpills--;
				// Check for incrementing lives
				update_NewLife(p);
				break;
		}
	} else {
		direction = 0;
		p->player_coord.next_x = p->player_coord.x_pos;
		p->player_coord.next_y = p->player_coord.y_pos;
	}
	
}

void clear_Section(int i, int j, int direction){
	uint16_t x = 9;
	uint16_t y = 9;

  for(y=9;y<j+5;y++){
		for(x=9;x<i+7;x++){
			draw_WallFull(x,y,Black,BOXSIZE);
			switch(boardMatrix[y][x]){
				case WALL:
					draw_WallEmpty((uint16_t) x,(uint16_t) y, Blue);
					break;
				case STDSCORE:
					draw_Circle(x,y,STDRADIUS,Red);
					break;
				case POWERSCORE:
					draw_Circle(x,y,POWERRADIUS,Green);
					break;
				
				// CASE FOR THE GHOST
				
//				case PACMANPOS:
//					draw_WallFull(x,y,Black,BOXSIZE);
//					if(direction == DIRUP){
//						draw_Character(x,y,pacmanMatrix_UP,Yellow);
//					} else if (direction == DIRDOWN){
//						draw_Character(x,y,pacmanMatrix_DOWN,Yellow); 
//					} else if (direction == DIRLEFT){
//						draw_Character(x,y,pacmanMatrix_LEFT,Yellow);
//					}	else if (direction == DIRRIGHT){
//						draw_Character(x,y,pacmanMatrix_RIGHT,Yellow); 
//					}
					break;
				default:
					break;			
			}
		}
  } 		
}


void menu_Pause(player *p, int direction){
	if(p->game_state == CONTINUE){
		GUI_Text(93,160,(uint8_t*) " PAUSE ", Red, White);
		disable_timer(0);
		disable_timer(1);
		disable_timer(2);
		disable_timer(3);
		p->game_state = PAUSE;
	} else {
		clear_Section(9,9,direction);
		enable_timer(0);
		enable_timer(1);
		enable_timer(2);
		enable_timer(3);
		p->game_state = CONTINUE;
	}
}

void display_GameOver(){
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	disable_RIT();
	GUI_Text(78, 160,(uint8_t*) " GAME OVER ", Red, White);

}

void display_Win(){
	GUI_Text(83,160,(uint8_t*) " VICTORY ", Red, White);
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	disable_RIT();
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
					(x_rand == p->player_coord.x_pos &&
					y_rand == p->player_coord.y_pos)
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
		int rand_time = rand_Range(1,5) * 25000000;

		init_timer(0,0,2,7,rand_time);

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