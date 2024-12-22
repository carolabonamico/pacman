#include "pacman.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern route r;

cell cellDetails[ROWS][COLS];
node openList[ROWS * COLS];
node current;
int closedList[ROWS][COLS] = {0};

/* -------------------- FUNCTIONS DEFINITION -------------------- */

// Function to check if a cell is valid (within grid boundaries)
int is_Valid(int row,int col){
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Function to check if a cell is unblocked
int is_Unblocked(int boardMatrix[ROWS][COLS],int row,int col){
    return (boardMatrix[row][col] != WALL);
}

// Function to check if the current cell is the destination
int is_Destination(int row, int col, node dest){
    return (row == dest.x && col == dest.y);
}

// A* Search Algorithm (modified)
void a_Star(int boardMatrix[ROWS][COLS],node start,node dest,route *r,
						cell cellDetails[ROWS][COLS],node openList[ROWS * COLS],
						node *current, int closedList[ROWS][COLS]){
		
		int i,j;
		
//    // Initializing closed and open lists
//    int closedList[ROWS][COLS] = {0}; // 0 means cell is not closed, 1 means closed
		for(i=0;i<ROWS;i++){
			for(j=0;j<COLS;j++){
				closedList[i][j] = 0;	
			}
		}

    // Initializing the cell details grid
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            cellDetails[j][i].f = INT_MAX;
            cellDetails[j][i].g = INT_MAX;
            cellDetails[j][i].h = INT_MAX;
            cellDetails[j][i].parent_x = -1;
            cellDetails[j][i].parent_y = -1;
        }
    }

    int startX = start.x;
    int startY = start.y;

    cellDetails[startY][startX].f = 0;
    cellDetails[startY][startX].g = 0;
    cellDetails[startY][startX].h = 0;
    cellDetails[startY][startX].parent_x = start.x;
    cellDetails[startY][startX].parent_y = start.y;

    // Open list is implemented as a priority queue
    int openListSize = 0;
    openList[openListSize++] = start;

    // 4 possible directions: up, down, left, right
    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    while (openListSize > 0) {
        // Find the node with the lowest f-value (priority queue)
        int lowestIndex = 0;
        for (i = 1; i < openListSize; i++) {
            int x = openList[i].x;
            int y = openList[i].y;
            if (cellDetails[y][x].f < cellDetails[openList[lowestIndex].y][openList[lowestIndex].x].f) {
                lowestIndex = i;
            }
        }

        *current = openList[lowestIndex];
        openListSize--;
        openList[lowestIndex] = openList[openListSize];  // Remove the node from the open list

        int currentRow = current->y;
        int currentCol = current->x;
        closedList[currentRow][currentCol] = 1;  // Add to closed list

        // If we have reached the destination
        if (is_Destination(currentRow, currentCol, dest)) {

            // Reconstruct the path inside the a_star function
            int currentRow = dest.y;
            int currentCol = dest.x;

            // Reconstruct path from destination to start
            while (!(currentRow == start.y && currentCol == start.x)) {
                r->path[r->path_length++] = (node){currentCol, currentRow};
                int tempRow = cellDetails[currentRow][currentCol].parent_y;
                int tempCol = cellDetails[currentRow][currentCol].parent_x;
                currentRow = tempRow;
                currentCol = tempCol;
            }

            // Add the start node to the path
//            r->path[r->path_length++] = (node){start.x, start.y};

        }

        // Check the 4 possible directions
        for (i = 0; i < 4; i++) {
            int newRow = currentRow + dRow[i];
            int newCol = currentCol + dCol[i];

            if (is_Valid(newRow, newCol) && is_Unblocked(boardMatrix, newRow, newCol) && !closedList[newRow][newCol]) {
                int gNew = cellDetails[currentRow][currentCol].g + 1;
                int hNew = abs(newRow - dest.x) + abs(newCol - dest.y);  // Heuristic (Manhattan Distance)
                int fNew = gNew + hNew;

                // If a better path is found
                if (fNew < cellDetails[newRow][newCol].f) {
                    openList[openListSize++] = (node){newCol, newRow};
                    cellDetails[newRow][newCol].f = fNew;
                    cellDetails[newRow][newCol].g = gNew;
                    cellDetails[newRow][newCol].h = hNew;
                    cellDetails[newRow][newCol].parent_y = currentRow;
                    cellDetails[newRow][newCol].parent_x = currentCol;
                }
            }
        }
    }

}