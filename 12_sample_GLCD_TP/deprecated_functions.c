//#include "pacman.h"
//#include "../GLCD/GLCD.h"
//#include "stdio.h"
//#include "math.h"

// DEFINING THE SCORES AND NUMBER OF PILLS 
//#define STDSCORE 10							// Score associated to a standard pill
//#define POWERSCORE 50						// Score associated to a power pill
//#define N_STDPILLS 240					// Tot number of standard pills
//#define INITLIVES 1							// Initial number of lives
//#define WALL 1									// Wall in the maze
//#define EMPTY 0									// Empty cell in the maze
//#define INITSCORE 0							// Initial score
//#define NEWLIFE 50							// Threshold for the genaration of a new life
//#define PACMANRADIUS 16
//#define POWERRADIUS 9
//#define STDRADIUS 1

// DEFINING THE GRID FOR THE MAZE
//#define COLS 24									// Number of colums of the matrix
//#define ROWS 24									// Number of rows of the matrix
//#define OFFSET 4
//#define BOXSIZE 10						// Size of each pixel block
//#define LEFTTUNNEL 8						// Encoding of the left tunnel
//#define RIGHTTUNNEL 9						// Encoding for the right tunnel
//#define UPPERMENU 50						// Padding for the upper interface
//#define	LIFEPOS 25							// Y coordinate for printing lives

// DECLARING VARIABLES
//extern player p;
//extern int direction;
//extern int countdown;
//volatile int life_increment_threshold = NEWLIFE;

///*
//MOVEMENT LEGENDA
//*/
//volatile int dirselection = 1;
//volatile int dirup = 2;
//volatile int dirdown = 3;
//volatile int dirleft = 4;
//volatile int dirright = 5;

//int boardMatrix[ROWS][COLS] = {
//    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//		{1,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,10,10,10,10,10,1},
//		{1,10,1,1,10,1,1,1,1,1,10,1,1,10,1,1,1,1,1,10,1,1,10,1},
//		{1,10,1,1,10,1,1,1,1,1,10,1,1,10,1,1,1,1,1,10,1,1,10,1},
//		{1,10,10,50,10,10,10,10,10,10,10,10,10,10,10,50,10,10,10,10,10,10,10,1},
//		{1,10,1,1,10,1,1,10,1,1,1,1,1,1,1,1,10,1,1,10,1,1,10,1},
//		{1,10,1,1,10,1,1,10,10,10,10,1,1,10,10,10,10,1,1,10,1,1,10,1},
//		{1,10,1,1,10,1,1,1,1,1,10,1,1,10,1,1,1,1,1,10,1,1,10,1},
//		{1,10,10,10,10,10,1,1,1,1,10,1,1,10,1,1,1,1,10,10,10,10,10,1},
//		{1,10,1,1,1,10,1,1,10,10,10,10,10,10,50,10,1,1,10,1,1,1,10,1},
//		{1,10,1,1,1,10,1,1,10,1,1,0,0,1,1,10,1,1,10,1,1,1,10,1},
//		{LEFTTUNNEL,10,50,10,10,10,10,10,10,1,0,0,0,0,1,10,10,10,10,10,10,10,10,RIGHTTUNNEL},		// Tunnel
//		{1,10,1,1,1,10,1,1,10,1,0,0,0,0,1,10,1,1,10,1,1,1,10,1},			
//		{1,10,1,1,1,10,1,1,10,1,1,1,1,1,1,10,1,1,10,1,1,1,10,1},
//		{1,10,10,10,10,10,1,10,10,10,10,10,10,10,10,50,10,1,10,10,10,10,10,1},
//		{1,10,1,1,1,1,1,10,1,1,1,1,1,1,1,1,10,1,1,1,1,1,10,1},
//		{1,10,10,10,10,10,10,10,10,10,10,1,1,10,10,10,10,10,10,10,10,10,10,1},
//		{1,10,1,1,1,10,1,1,1,1,10,1,1,10,1,1,1,1,10,1,1,1,10,1},
//		{1,10,10,10,1,10,10,10,10,10,10,10,10,10,10,10,10,10,10,1,10,10,10,1},
//		{1,1,1,10,1,10,1,10,1,1,1,1,1,1,1,1,10,1,10,1,10,1,1,1},
//		{1,10,10,10,10,10,1,10,10,10,10,1,1,10,10,10,10,1,10,10,10,10,10,1},
//		{1,10,1,1,1,1,1,1,1,1,10,1,1,10,1,1,1,1,1,1,1,1,10,1},
//		{1,10,10,10,10,50,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,0,1},
//		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
//};

///* REMINDER
//#define  MAX_X  240
//#define  MAX_Y  320 
//*/

///*
//------------- DRAW FUNCTIONS -------------
//*/

//void draw_STDpill(uint16_t x, uint16_t y, uint32_t color){
//	int i, j;
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	
//	for (i=0;i<2;i++){
//		for (j=0;j<2;j++){
//			LCD_SetPoint(x_start+i+4, y_start+j+4, color); 
//		}
//	}
//}

//void draw_POWERpill(uint16_t x, uint16_t y, uint32_t color){
//	int i, j;
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	
//	for (i=0;i<4;i++){
//		for (j=0;j<4;j++){
//			LCD_SetPoint(x_start+i+3, y_start+j+3, color); 
//		}
//	}
//}

//void draw_WallFull(uint16_t x, uint16_t y, uint32_t color){
//	int i,j;
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	
//	for (i=0;i<10;i++){
//		for (j=0;j<10;j++){
//			LCD_SetPoint(x_start+i, y_start+j, color); 
//		}
//	}
//}

//void draw_WallEmpty(uint16_t x, uint16_t y, uint32_t color){
//	int i,j;
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	
//	int x1 = x_start, y1 = y_start;  					// Top-left corner
//  int x2 = x_start + BOXSIZE, y2 = y_start;  						// Top-right corner
//  int x3 = x_start + BOXSIZE, y3 = y_start + BOXSIZE;  	// Bottom-right corner
//  int x4 = x_start, y4 = y_start + BOXSIZE; 						// Bottom-left corner

//  // Draw the four edges of the square
//  LCD_DrawLine(x1, y1, x2, y2, Blue);  // Top edge
//  LCD_DrawLine(x2, y2, x3, y3, Blue);  // Right edge
//  LCD_DrawLine(x3, y3, x4, y4, Blue);  // Bottom edge
//  LCD_DrawLine(x4, y4, x1, y1, Blue);  // Left edge
//}

//void draw_Circle(int x, int y, int radius_2, int color){

//	int i,j;
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	float distance_2;
//	
//	int x_center = BOXSIZE/2;
//	int y_center = BOXSIZE/2;
//	for(i=0;i<BOXSIZE;i++){
//		for(j=0;j<BOXSIZE;j++){
//			distance_2 = pow(i-x_center,2)+pow(j-y_center,2);
//			if(distance_2<=radius_2){
//					LCD_SetPoint(x_start+i,y_start+j,color);
//			}
//		}
//	}
//	
//}

//void draw_Pacman_test(int x, int y, int color){
//	
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	
//	x_start += BOXSIZE/2;
//	y_start += BOXSIZE/2;
//	
//	int radius = 5;
//	int start_angle = 30;
//	int end_angle = 330;
//	int angle;
//	int xf, yf, r;
//	float pi = 3.14;
//	
//	// Conversion from angle to radiants
//	float startrad = start_angle * pi / 180.0;
//	float endrad = end_angle * pi / 180.0;
//	
//	for(angle=start_angle;angle<=end_angle;angle++){
//		float rad = angle * pi / 180.0;
//		for (r=0;r<=radius;r++){
//			xf = x_start+r*cos(rad);
//			yf = y_start+r*sin(rad);
//			LCD_SetPoint(xf,yf,color);			
//		}	
//	}

//	int i,j;
//	uint16_t x_start = x*BOXSIZE;
//	uint16_t y_start = y*BOXSIZE+UPPERMENU;
//	float distance_2;
//	
//	int x_center = BOXSIZE/2;
//	int y_center = BOXSIZE/2;
//	float radius_2 = 25;
//	for(i=0;i<BOXSIZE;i++){
//		for(j=0;j<BOXSIZE;j++){
//			distance_2 = pow(i-x_center,2)+pow(j-y_center,2);
//			if(distance_2<=radius_2){
//				if (
//						(j == 4 && i == 3) ||
//						(j == 5 && i <= 2 && i <= 4) ||
//						(j == 6 && i <= 1 && i <= 5) ||
//						(j == 7 && i <= 0 && i <= 6) ||
//						(j == 8 && i <= 0 && i <= 8)
//					 ){
//				} else {
//					LCD_SetPoint(x_start+i,y_start+j,color);
//				}
//			}
//		}
//	}
//}

///*
//------------- INITIALIZATION FUNCTIONS -------------
//*/

///* FUNCTION TO INITIALIZE THE INTERFACE */
//void init_GameSpace(){
//	
//	GUI_Text(0,0, (unsigned char*) "Game over over in",0xffff,Black); 
//	//sprintf(str,"%d", timer_value);
//	//GUI_Text(0,20, (unsigned char*) str,0xffff,Black); 
//	GUI_Text(190,0, (unsigned char*) "SCORE",0xffff,Black); 
//	GUI_Text(190,20, (unsigned char*) "20",0xffff,Black);

//}

///* FUNCTION TO INITIALIZE THE INTERFACE */
//void init_Header(){
//	GUI_Text(0,0,(unsigned char*) "Game Over in",0xffff,Black);
//	GUI_Text(0,20,(unsigned char*) "60s" ,0xffff,Black);
//	
//	GUI_Text((MAX_X/3)*2,0,(unsigned char*) "Score",0xffff,Black);
//	GUI_Text((MAX_X/3)*2,MAX_Y/16,(unsigned char*) "0" ,0xffff,Black);
//}

//void update_TimerHeader(int countdown){
//	char str_timer[10];
//	
//	if(countdown>=10){
//		sprintf(str_timer, "%ds", countdown); 
//	} else {
//		sprintf(str_timer, "0%ds", countdown); 
//	}
//	
//	GUI_Text(0,0, (unsigned char*) "Game Over in",0xffff,Black);
//	GUI_Text(0,MAX_Y/16, (unsigned char*) str_timer ,0xffff,Black);
//}

//void update_ScoreHeader(int score){
//	char str_score[10];
//	sprintf(str_score, "%d", score); 
//	GUI_Text((MAX_X/3)*2,0,(unsigned char*) "Score",0xffff,Black);
//	GUI_Text((MAX_X/3)*2,MAX_Y/16,(unsigned char*) str_score ,0xffff,Black);
//}

///* FUNCTION TO INITIALIZE THE MAZE*/
//void setup_GameSpace(){
//	uint16_t x,y,i,j;
//	uint32_t color;

//  for(y = 0; y < ROWS; y++){
//		for( x = 0; x < COLS; x++){
//					
//			switch(boardMatrix[y][x]){	
//				// Blue wall
//				case WALL: 
//					color = Blue; 					
//					draw_WallEmpty(x,y,color);
//					break; 
//				// Black path
//				case 0: 
//					color = Black; 				
//					break; 		
//				// Red standard pill 
//				case STDSCORE: 
//					color= Red; 						// Standard pills
//					draw_Circle(x,y,STDRADIUS,color);
//					//draw_STDpill(x,y,color);
//					break; 
//				// Green power pill
//				case POWERSCORE: 
//					color = Green; 				// Power pills
//					//draw_POWERpill(x,y,color);
//					draw_Circle(x,y,POWERRADIUS,color);
//					break;		
//				// Pacman
//				default:
//					draw_Circle(COLS-2,ROWS-2,PACMANRADIUS,Yellow);
//					break;
//			}
//    } 		
//	}
//	
//	// Printing the first life
//	draw_Circle(1,LIFEPOS,PACMANRADIUS,Yellow);
//}

// PLAYER INITIALIZATION
//void init_Player(player *p){
// // Function to initialize the player structure
// p->x_pos = ROWS-2;
// p->y_pos = COLS-2;
// p->nlives = INITLIVES;
// p->score = INITSCORE;
//}

// LIFE INCREMENT FUNCTION
//void check_NewLife(player *p){

//	if(p->score >= life_increment_threshold){
//		p->nlives++;
//		life_increment_threshold += NEWLIFE;
//		draw_Circle(p->nlives,LIFEPOS,PACMANRADIUS,Yellow);
//		
//	}
//}

// FUNCTION TO COMPUTE NEXT POSITION OF PLAYER
//void controller_Player(int direction, player *p){
//	
//	if(direction == dirup){
//		p->next_x = p->x_pos;
//		p->next_y = p->y_pos-1; 
//	} 
//	else if (direction == 3){
//		p->next_x = p->x_pos;
//		p->next_y = p->y_pos+1;  
//	} else if (direction == 4 && boardMatrix[p->y_pos][p->x_pos] != LEFTTUNNEL){
//		p->next_x = p->x_pos-1;
//		p->next_y = p->y_pos; 
//	}	else if (direction == 4 && boardMatrix[p->y_pos][p->x_pos] == LEFTTUNNEL){
//		p->next_x = COLS-1;
//		p->next_y = p->y_pos;  
//	} else if (direction == 5 && boardMatrix[p->y_pos][p->x_pos] != RIGHTTUNNEL){
//		p->next_x = p->x_pos+1;
//		p->next_y = p->y_pos; 
//	}	else if (direction == 5 && boardMatrix[p->y_pos][p->x_pos] == RIGHTTUNNEL){
//		p->next_x = 0;
//		p->next_y = p->y_pos;  
//	}

//}

// FUNCTION TO PERFORM NEXT STEP OF THE PLAYER
//void move_Player(player *p){
//	
//	int current_x = p->x_pos;
//	int current_y = p->y_pos;
//	
//	if(boardMatrix[p->next_y][p->next_x] != WALL && direction != 0){
//		// Updating player position
//		p->x_pos = p->next_x;
//		p->y_pos = p->next_y;
//		
//		// Redrawing
//		//draw_((uint16_t) current_x,(uint16_t) current_y,Black);
//		draw_Circle(current_x,current_y,PACMANRADIUS,Black);
//		draw_Circle(p->next_x,p->next_y,PACMANRADIUS,Yellow);
//		
//		// Updating the scores and the matrix
//		switch(boardMatrix[p->next_y][p->next_x]){
//			case STDSCORE:
//				p->score += STDSCORE;
//				update_ScoreHeader(p->score);
//				boardMatrix[p->next_y][p->next_x] = EMPTY;
//				// Check for incrementing lives
//				check_NewLife(p);
//				break;
//			case POWERSCORE:
//				p->score += POWERSCORE;
//				update_ScoreHeader(p->score);
//				boardMatrix[p->next_y][p->next_x] = EMPTY;
//				// Check for incrementing lives
//				check_NewLife(p);
//				break;
//		}
//	} else {
//		direction = 0;
//		p->next_x = p->x_pos;
//		p->next_y = p->y_pos;
//	}
//	
//}