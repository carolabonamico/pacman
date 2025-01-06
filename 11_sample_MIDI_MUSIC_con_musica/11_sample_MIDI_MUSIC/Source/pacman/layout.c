#include "../pacman/pacman.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern int pacmanMatrix_UP[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_DOWN[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_LEFT[BOXSIZE][BOXSIZE];
extern int pacmanMatrix_RIGHT[BOXSIZE][BOXSIZE];
extern int ghostMatrix[BOXSIZE][BOXSIZE];
extern int direction;		
extern int ticks_per_second;
extern ghost g;
extern player p;
extern int countdown;

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

/* -------------------- FUNCTIONS DEFINITION -------------------- */

void decrement_Life(player *p){
	
	// Erasing a life
	draw_Character(p->nlives,LIFEPOS,pacmanMatrix_RIGHT,Black);
	// Decrementing life counter
	p->nlives--;
	// Feedback to the loss of a life
	redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
	redraw_Pacman(p->player_coord.pos.x,p->player_coord.pos.y,p->player_coord.pos.x,p->player_coord.pos.y,direction);
	
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
		clear_Section(9,9,direction,p,&g);
		enable_timer(0);
		enable_timer(1);
		enable_timer(2);
		enable_timer(3);
		p->game_state = CONTINUE;
	}
}

void display_GameOver(){
	p.game_state = END;
	disable_timer(2);
	disable_timer(3);
	GUI_Text(78, 160,(uint8_t*) " GAME OVER ", Red, White);
}

void display_Win(){
	p.game_state = END;
	disable_timer(2);
	disable_timer(3);
	GUI_Text(83,160,(uint8_t*) " VICTORY ", Red, White);
}

void clear_Section(int i, int j, int direction, player *p, ghost *g){
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
				default:
					break;
			}
				
				if(x == p->player_coord.pos.x && y == p->player_coord.pos.y){
					draw_WallFull(x,y,Black,BOXSIZE);
					draw_LastDir(x,y);
				} else if(x == g->ghost_coord.pos.x && y == g->ghost_coord.pos.y){
					draw_WallFull(x,y,Black,BOXSIZE);
					int color = (g->vulnerable) ? Cyan : Red;
					draw_Character(g->ghost_coord.pos.x, g->ghost_coord.pos.y, ghostMatrix, color);
				}

		}
  } 		
}

void draw_LastDir(int x, int y){
	
	if(p.last_direction == DIRUP){
		draw_Character(x,y,pacmanMatrix_UP,Yellow);
	} else if (p.last_direction == DIRDOWN){
		draw_Character(x,y,pacmanMatrix_DOWN,Yellow); 
	} else if (p.last_direction == DIRLEFT){
		draw_Character(x,y,pacmanMatrix_LEFT,Yellow);
	}	else if (p.last_direction == DIRRIGHT){
		draw_Character(x,y,pacmanMatrix_RIGHT,Yellow); 
	}

}

int sub_Counter(int elapsed_time,int *sub_second_count, int current_interval, int ticks_per_second){
	
	// Increment the sub-second counter
  *sub_second_count += current_interval;

  // When sub-second counter reaches 1 second worth of ticks
  if(*sub_second_count >= ticks_per_second){
		// Reset the sub-second counter
		*sub_second_count -= ticks_per_second;
		// Increment elapsed time in seconds
    elapsed_time++;
  }
	
	return elapsed_time;
}

void check_PreviousCell(player *p, grid *gr, ghost *g){

		// Updating the scores and the matrix
		switch(boardMatrix[p->player_coord.next_pos.y][p->player_coord.next_pos.x]){
			case STDSCORE:
				p->score += STDSCORE;
				update_ScoreHeader(p->score);
				boardMatrix[p->player_coord.next_pos.y][p->player_coord.next_pos.x] = EMPTY;
				gr->n_stdpills--;
				// Check for incrementing lives
				update_NewLife(p);
				break;
			case POWERSCORE:
				p->score += POWERSCORE;
				update_ScoreHeader(p->score);
				boardMatrix[p->player_coord.next_pos.y][p->player_coord.next_pos.x] = EMPTY;
				gr->n_powerpills--;
				// Check for incrementing lives
				update_NewLife(p);
				// Bool for vulnerable ghost
				g->vulnerable = true;
				break;
		}


}