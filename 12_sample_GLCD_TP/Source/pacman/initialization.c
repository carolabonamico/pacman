#include "../pacman/pacman.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern int boardMatrix[ROWS][COLS];
extern pacmanMatrix_RIGHT[BOXSIZE][BOXSIZE];
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern pacmanMatrix_LEFT[BOXSIZE][BOXSIZE];

/* -------------------- FUNCTIONS DEFINITION -------------------- */

// HEADER
void init_Header(){
	GUI_Text(0,0,(unsigned char*) "GAME OVER IN",White,Black);
	GUI_Text(0,20,(unsigned char*) "60s" ,White,Black);
	
	GUI_Text((MAX_X/3)*2,0,(unsigned char*) "SCORE",White,Black);
	GUI_Text((MAX_X/3)*2,MAX_Y/16,(unsigned char*) "0",White,Black);
}


// MAZE
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

// PLAYER 
void init_Player(player *p){
	
	// Function to initialize the player structure
	p->player_coord.pos.x = ROWS-2;
	p->player_coord.pos.y = COLS-2;
	p->nlives = INITLIVES;
	p->score = INITSCORE;
	p->game_state = CONTINUE;
	
}

// GHOST
void init_Ghost(ghost *g){
	g->ghost_coord.pos.x = 11;
	g->ghost_coord.pos.y = 11;
	g->vulnerable = false;
	g->eaten = false;
	g->reset_counter = false;
}

// GRID
void init_Grid(grid *gr){
	gr->n_powerpills=0;
	gr->n_stdpills=0;
}

// ROUTE
void init_Route(route *r){
	
	int i;
  for(i = 0; i < ROWS * COLS; i++){
		r->path[i].x = 0;
    r->path[i].y = 0;
  }
  r->path_length = 0;
		
}

// CLOSED LIST
void init_ClosedList(int closedList[ROWS][COLS]){
	
	int i, j;
	for(i=0;i<ROWS;i++){
		for(j=0;j<COLS;j++){
			closedList[i][j] = 0;	
		}
	}

}