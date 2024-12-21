#include "pacman.h"

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

//volatile Node start = {0, 0}; // Start at (0, 0)
//volatile Node dest = {8, 9};  // Destination at (8, 9)

// Function to check if a cell is valid (within grid boundaries)
int is_valid(int row, int col) {
    return (row >= 0 && row < ROW && col >= 0 && col < COL);
}

// Function to check if a cell is unblocked (1 means unblocked, 0 means blocked)
int is_unblocked(int grid_test[ROW][COL], int row, int col) {
    return (grid_test[row][col] == 1);
}

// Function to check if the current cell is the destination
int is_destination(int row, int col, Node dest) {
    return (row == dest.x && col == dest.y);
}

// Reconstruct the path from the cell details
percorso reconstruct_path_test(Cell cellDetails[ROW][COL], Node start, Node dest, percorso *perc) {
    int currentRow = dest.x;
    int currentCol = dest.y;
    perc->path_length = 0;

    // Reconstruct path from destination to start
    while (!(currentRow == start.x && currentCol == start.y)) {
        perc->path[perc->path_length++] = (Node){currentRow, currentCol};
        int tempRow = cellDetails[currentRow][currentCol].parent_x;
        int tempCol = cellDetails[currentRow][currentCol].parent_y;
        currentRow = tempRow;
        currentCol = tempCol;
    }

    // Add the start node to the path
    perc->path[perc->path_length++] = (Node){start.x, start.y};
		return *perc;
}

// A* Search Algorithm (modified)
percorso a_star(int grid_test[ROW][COL], Node start, Node dest, percorso *perc) {
		int i,j;
    // Initializing closed and open lists
    int closedList[ROW][COL] = {0}; // 0 means cell is not closed, 1 means closed
    Cell cellDetails[ROW][COL]; // Details of each cell

    // Initializing the cell details grid
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
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
    Node openList[ROW * COL];
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

        Node current = openList[lowestIndex];
        openListSize--;
        openList[lowestIndex] = openList[openListSize];  // Remove the node from the open list

        int currentRow = current.x;
        int currentCol = current.y;
        closedList[currentRow][currentCol] = 1;  // Add to closed list

        // If we have reached the destination
        if (is_destination(currentRow, currentCol, dest)) {

            // Reconstruct the path inside the a_star function
            volatile percorso perc_2 = {0};
            int currentRow = dest.x;
            int currentCol = dest.y;

            // Reconstruct path from destination to start
            while (!(currentRow == start.x && currentCol == start.y)) {
                perc_2.path[perc_2.path_length++] = (Node){currentRow, currentCol};
                int tempRow = cellDetails[currentRow][currentCol].parent_x;
                int tempCol = cellDetails[currentRow][currentCol].parent_y;
                currentRow = tempRow;
                currentCol = tempCol;
            }

            // Add the start node to the path
            perc_2.path[perc_2.path_length++] = (Node){start.x, start.y};

            // Return the number of nodes in the path
            return perc_2;
        }

        // Check the 4 possible directions
        for (i = 0; i < 4; i++) {
            int newRow = currentRow + dRow[i];
            int newCol = currentCol + dCol[i];

            if (is_valid(newRow, newCol) && is_unblocked(grid_test, newRow, newCol) && !closedList[newRow][newCol]) {
                int gNew = cellDetails[currentRow][currentCol].g + 1;
                int hNew = abs(newRow - dest.x) + abs(newCol - dest.y);  // Heuristic (Manhattan Distance)
                int fNew = gNew + hNew;

                // If a better path is found
                if (fNew < cellDetails[newRow][newCol].f) {
                    openList[openListSize++] = (Node){newRow, newCol};
                    cellDetails[newRow][newCol].f = fNew;
                    cellDetails[newRow][newCol].g = gNew;
                    cellDetails[newRow][newCol].h = hNew;
                    cellDetails[newRow][newCol].parent_x = currentRow;
                    cellDetails[newRow][newCol].parent_y = currentCol;
                }
            }
        }
    }

    return *perc;
}