#include "pacman.h"

extern route r;
cell cellDetails[ROWS][COLS];
node openList[ROWS * COLS];
node current;

volatile int grid_test[ROW][COL] = {
        {1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 1},
        {1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
        {1, 0, 0, 1, 1, 0, 1, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

//volatile node start = {0, 0}; // Start at (0, 0)
//volatile node dest = {8, 9};  // Destination at (8, 9)

// Function to initialize the route
void init_Route(route *r){
	
    if (r != NULL){
        // Set all path elements to zero (node.x and node.y to 0)
        int i;
        for (i = 0; i < ROW * COL; i++) {
            r->path[i].x = 0;
            r->path[i].y = 0;
        }

        // Set the path_length to zero
        r->path_length = 0;
    }
		
}

// Function to check if a cell is valid (within grid boundaries)
int is_Valid(int row,int col){
    return (row >= 0 && row < ROW && col >= 0 && col < COL);
}

// Function to check if a cell is unblocked (1 means unblocked, 0 means blocked)
int is_Unblocked(int grid_test[ROW][COL],int row,int col){
    return (grid_test[row][col] == 1);
}

// Function to check if the current cell is the destination
int is_Destination(int row, int col, node dest){
    return (row == dest.x && col == dest.y);
}

// A* Search Algorithm (modified)
void a_Star(int grid_test[ROW][COL],node start,node dest,route *r,cell cellDetails[ROWS][COLS],node openList[ROWS * COLS],node *current){
		
		int i,j;

//		// Initializing the route to 0
//		initialize_Route(r);
		
    // Initializing closed and open lists
    int closedList[ROW][COL] = {0}; // 0 means cell is not closed, 1 means closed

    // Initializing the cell details grid
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            cellDetails[i][j].f = INT_MAX;
            cellDetails[i][j].g = INT_MAX;
            cellDetails[i][j].h = INT_MAX;
            cellDetails[i][j].parent_x = -1;
            cellDetails[i][j].parent_y = -1;
        }
    }

    int startX = start.x;
    int startY = start.y;

    cellDetails[startX][startY].f = 0;
    cellDetails[startX][startY].g = 0;
    cellDetails[startX][startY].h = 0;
    cellDetails[startX][startY].parent_x = start.x;
    cellDetails[startX][startY].parent_y = start.y;

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
            if (cellDetails[x][y].f < cellDetails[openList[lowestIndex].x][openList[lowestIndex].y].f) {
                lowestIndex = i;
            }
        }

        *current = openList[lowestIndex];
        openListSize--;
        openList[lowestIndex] = openList[openListSize];  // Remove the node from the open list

        int currentRow = current->x;
        int currentCol = current->y;
        closedList[currentRow][currentCol] = 1;  // Add to closed list

        // If we have reached the destination
        if (is_Destination(currentRow, currentCol, dest)) {

            // Reconstruct the path inside the a_star function
            int currentRow = dest.x;
            int currentCol = dest.y;

            // Reconstruct path from destination to start
            while (!(currentRow == start.x && currentCol == start.y)) {
                r->path[r->path_length++] = (node){currentRow, currentCol};
                int tempRow = cellDetails[currentRow][currentCol].parent_x;
                int tempCol = cellDetails[currentRow][currentCol].parent_y;
                currentRow = tempRow;
                currentCol = tempCol;
            }

            // Add the start node to the path
            r->path[r->path_length++] = (node){start.x, start.y};

        }

        // Check the 4 possible directions
        for (i = 0; i < 4; i++) {
            int newRow = currentRow + dRow[i];
            int newCol = currentCol + dCol[i];

            if (is_Valid(newRow, newCol) && is_Unblocked(grid_test, newRow, newCol) && !closedList[newRow][newCol]) {
                int gNew = cellDetails[currentRow][currentCol].g + 1;
                int hNew = abs(newRow - dest.x) + abs(newCol - dest.y);  // Heuristic (Manhattan Distance)
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