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
	g->ghost_coord.x_pos = 11;
	g->ghost_coord.y_pos = 11;
	g->path_length = 0;
}

void move_Ghost(ghost *ghost, player *p, grid *gr, int direction){
	
	int current_x = ghost->ghost_coord.x_pos;
	int current_y = ghost->ghost_coord.y_pos;
	
		if(boardMatrix[ghost->ghost_coord.next_y][ghost->ghost_coord.next_x] != WALL &&
			direction != 0){
			
			if (ghost->ghost_coord.next_x == p->player_coord.x_pos && 
					ghost->ghost_coord.next_y == p->player_coord.y_pos){
				decrement_Life(p);
				ghost->ghost_coord.next_x = ghost->ghost_coord.x_pos;
				ghost->ghost_coord.next_y = ghost->ghost_coord.y_pos;
				redraw_Pacman(p->player_coord.x_pos,p->player_coord.y_pos,p->player_coord.x_pos,p->player_coord.y_pos,direction);
				redraw_Pacman(p->player_coord.x_pos,p->player_coord.y_pos,p->player_coord.x_pos,p->player_coord.y_pos,direction);
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
				ghost->ghost_coord.x_pos = ghost->ghost_coord.next_x;
				ghost->ghost_coord.y_pos = ghost->ghost_coord.next_y;
				draw_Character(ghost->ghost_coord.next_x,ghost->ghost_coord.next_y,ghostMatrix,Red);
			}
				
		} else {
			ghost->ghost_coord.next_x = ghost->ghost_coord.x_pos;
			ghost->ghost_coord.next_y = ghost->ghost_coord.y_pos;
		}	
		
		if(p->nlives <= 0){
			// Pacman disappears as it died
			draw_WallFull(p->player_coord.x_pos,p->player_coord.y_pos,Black,BOXSIZE);
			display_GameOver();
		}
			
}

void decrement_Life(player *p){
	
	// Erasing a life
	draw_Character(p->nlives,LIFEPOS,pacmanMatrix_RIGHT,Black);
	// Decrementing life counter
	p->nlives--;
	// Feedback to the loss of a life
	redraw_Pacman(p->player_coord.x_pos,p->player_coord.y_pos,p->player_coord.x_pos,p->player_coord.y_pos,direction);
	redraw_Pacman(p->player_coord.x_pos,p->player_coord.y_pos,p->player_coord.x_pos,p->player_coord.y_pos,direction);
	
}
int is_Unblocked(int boardMatrix[ROWS][COLS], int row, int col){
    return (boardMatrix[row][col] != WALL); 						// 1 means the cell is not blocked
}

int is_Destination(int row, int col, node dest){
    return (row == dest.x && col == dest.y);
}

int calculate_Heuristic(int row, int col, node dest){
    return abs(row - dest.x) + abs(col - dest.y);  			// Using Manhattan Distance
}

int is_Valid(int row, int col){
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// A* Search Algorithm
void a_Star(ghost *ghost, player *p){
		
		int i = 0;
		int j = 0;
		int q = 0;
		int e = 0;
		int counter = 0;
		int closedList[ROWS][COLS];
	
	  node start = {ghost->ghost_coord.x_pos,ghost->ghost_coord.y_pos}; 
    node dest = {p->player_coord.x_pos, p->player_coord.y_pos};  
	
    // Initializing closed and open list
		 for (q=0;q<ROWS;q++) {
        for (e=0;e<COLS;e++) {
            closedList[q][e] = 0;  // Set each element to 0
        }
    }

    cell cellDetails[ROWS][COLS]; 											// Details of each cell

    // Initializing the cell details grid
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            cellDetails[i][j].f = INT_MAX;
            cellDetails[i][j].g = INT_MAX;
            cellDetails[i][j].h = INT_MAX;
            cellDetails[i][j].parent_x = -1;
            cellDetails[i][j].parent_y = -1;
        }
    }

    int startX = ghost->ghost_coord.x_pos;
    int startY = ghost->ghost_coord.y_pos;

    cellDetails[startX][startY].f = 0;
    cellDetails[startX][startY].g = 0;
    cellDetails[startX][startY].h = 0;
    cellDetails[startX][startY].parent_x = startX;
    cellDetails[startX][startY].parent_y = startY;

    // Open list is implemented as a priority queue
    // Start with the source node in the open list
    node openList[ROWS * COLS];
    int openListSize = 0;
    openList[openListSize++] = start;

    // 4 possible directions: up, down, left, right
    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    while (openListSize > 0) {
        // Find the node with the lowest f-value (priority queue)
        int lowestIndex = 0;
				int w = 0;
        for (w=1;w<openListSize;w++) {
            int x = openList[w].x;
            int y = openList[w].y;
            if (cellDetails[x][y].f < cellDetails[openList[lowestIndex].x][openList[lowestIndex].y].f) {
                lowestIndex = w;
            }
        }

        node current = openList[lowestIndex];
        openListSize--;
        openList[lowestIndex] = openList[openListSize];  						// Remove the node from the open list

        int currentRow = current.x;
        int currentCol = current.y;
        closedList[currentRow][currentCol] = 1;  // Add to closed list

        // If we have reached the destination
        if (is_Destination(currentRow, currentCol, dest)){
						int index = 0;

						reconstruct_path(cellDetails, start, dest, ghost);
						
            return;
        }

        // Check the 4 possible directions
				int k=0;
        for (k=0;k<4;k++) {
            int newRow = currentRow + dRow[k];
            int newCol = currentCol + dCol[k];

            if (is_Valid(newRow, newCol) && is_Unblocked(boardMatrix, newRow, newCol) && !closedList[newRow][newCol]) {
                int gNew = cellDetails[currentRow][currentCol].g + 1;
                int hNew = calculate_Heuristic(newRow, newCol, dest);
                int fNew = gNew + hNew;

                // If a better path is found
                if (fNew < cellDetails[newRow][newCol].f) {
                    openList[openListSize++] = (node){newRow, newCol};
                    cellDetails[newRow][newCol].f = fNew;
                    cellDetails[newRow][newCol].g = gNew;
                    cellDetails[newRow][newCol].h = hNew;
                    cellDetails[newRow][newCol].parent_x = currentRow;
                    cellDetails[newRow][newCol].parent_y = currentCol;
                }
            }
        }
    }
		
}

void reconstruct_path(cell cellDetails[ROWS][COLS], node start, node dest, ghost *ghost){
    int currentRow = dest.x;
    int currentCol = dest.y;
    ghost->path_length = 0;

    while (!(currentRow == start.x && currentCol == start.y)) {
        ghost->path[ghost->path_length++] = (node){currentRow, currentCol};
        int tempRow = cellDetails[currentRow][currentCol].parent_x;
        int tempCol = cellDetails[currentRow][currentCol].parent_y;
        currentRow = tempRow;
        currentCol = tempCol;
    }

    // Add the start node
    ghost->path[ghost->path_length++] = (node){start.x, start.y};
}