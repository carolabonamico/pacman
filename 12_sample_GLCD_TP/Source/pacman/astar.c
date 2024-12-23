#include "pacman.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern route r;

/* -------------------- FUNCTIONS DEFINITION -------------------- */

// Function to check if a cell is valid (within grid boundaries)
int is_Valid(int row, int col) {
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Function to check if a cell is unblocked (1 means unblocked, 0 means blocked)
int is_Unblocked(int boardMatrix[ROWS][COLS], int row, int col) {
    return (boardMatrix[row][col] != WALL);
}

// Function to check if the current cell is the destination
int is_Destination(int row, int col, node dest) {
    return (row == dest.y && col == dest.x); // Align with row=y and col=x
}

// A* Search Algorithm
void a_Star(int boardMatrix[ROWS][COLS], node start, node dest, route *r) {
	
    // Internal declarations
		int i, j;
    static cell cellDetails[ROWS][COLS];
    static int closedList[ROWS][COLS] = {0};
    static node openList[ROWS * COLS];
    static int openListSize = 0;
		
		
    // Reset the path (clear previous path if any)
    r->path_length = 0;  // Reset path length before recomputing
    for (i = 0; i < ROWS * COLS; i++) {
        r->path[i] = (node){-1, -1};  // Optional: clear any previous path
    }

    // Reset the closed list to 0 (not visited)
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            closedList[i][j] = 0;
        }
    }

    // Reset the open list (mark all cells as unused)
    for (i = 0; i < ROWS * COLS; i++) {
        openList[i].x = 0;
        openList[i].y = 0;
    }

    // Initialize cell details
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            cellDetails[i][j].f = INT_MAX;
            cellDetails[i][j].g = INT_MAX;
            cellDetails[i][j].h = INT_MAX;
            cellDetails[i][j].parent_x = -1;
            cellDetails[i][j].parent_y = -1;
        }
    }

    // Initialize the start node
    int startX = start.x, startY = start.y;
    cellDetails[startY][startX].f = 0;
    cellDetails[startY][startX].g = 0;
    cellDetails[startY][startX].h = 0;
    cellDetails[startY][startX].parent_x = startX;
    cellDetails[startY][startX].parent_y = startY;

    // Add the start node to the open list
    openList[openListSize++] = start;

    // 4 possible directions: up, down, left, right
    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    while (openListSize > 0) {
        // Find the node with the lowest f-value
        int lowestIndex = 0;
        for (i = 1; i < openListSize; i++) {
            int x = openList[i].x, y = openList[i].y;
            if (cellDetails[y][x].f < cellDetails[openList[lowestIndex].y][openList[lowestIndex].x].f) {
                lowestIndex = i;
            }
        }

        // Remove the current node from the open list
        node current = openList[lowestIndex];
        openList[lowestIndex] = openList[--openListSize];

        int currentRow = current.y, currentCol = current.x;
        closedList[currentRow][currentCol] = 1;

        // Check if we reached the destination
        if (is_Destination(currentRow, currentCol, dest)) {
            int pathRow = dest.y, pathCol = dest.x;
            while (!(pathRow == startY && pathCol == startX)) {
                r->path[r->path_length++] = (node){pathCol, pathRow};
                int tempRow = cellDetails[pathRow][pathCol].parent_y;
                int tempCol = cellDetails[pathRow][pathCol].parent_x;
                pathRow = tempRow;
                pathCol = tempCol;
            }
            return; // Path found
        }

        // Explore neighboring cells
        for (i = 0; i < 4; i++) {
            int newRow = currentRow + dRow[i];
            int newCol = currentCol + dCol[i];

            if (is_Valid(newRow, newCol) && is_Unblocked(boardMatrix, newRow, newCol) && !closedList[newRow][newCol]) {
                int gNew = cellDetails[currentRow][currentCol].g + 1;
                int hNew = abs(newRow - dest.y) + abs(newCol - dest.x); // Manhattan Distance
                int fNew = gNew + hNew;

                if (fNew < cellDetails[newRow][newCol].f) {
                    openList[openListSize++] = (node){newCol, newRow};
                    cellDetails[newRow][newCol].f = fNew;
                    cellDetails[newRow][newCol].g = gNew;
                    cellDetails[newRow][newCol].h = hNew;
                    cellDetails[newRow][newCol].parent_x = currentCol;
                    cellDetails[newRow][newCol].parent_y = currentRow;
                }
            }
        }
    }
}