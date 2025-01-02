#include "../pacman/pacman.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"

/* -------------------- VARIABLES DECLARATION -------------------- */

extern int boardMatrix[ROWS][COLS];
extern int direction;
extern ghost g;

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

/* -------------------- FUNCTIONS DEFINITION -------------------- */

void move_Ghost(ghost *ghost, route *r, player *p) {
    int current_x = ghost->ghost_coord.pos.x;
    int current_y = ghost->ghost_coord.pos.y;
    uint32_t color;

    // Check if the ghost has been eaten
    if (ghost->eaten) {
			
        return; // Ghost remains out of play until respawn logic
    }

    r->path_length--;
    ghost->ghost_coord.next_pos.x = r->path[r->path_length].x;
    ghost->ghost_coord.next_pos.y = r->path[r->path_length].y;

    // Check for collision with Pacman
    if (ghost->ghost_coord.next_pos.x == p->player_coord.pos.x &&
        ghost->ghost_coord.next_pos.y == p->player_coord.pos.y) {
        
        if (!ghost->vulnerable) {
            // Ghost damages Pacman
            decrement_Life(p);

            // Redraw Pacman twice for visual feedback
            redraw_Pacman(p->player_coord.pos.x, p->player_coord.pos.y,
                          p->player_coord.pos.x, p->player_coord.pos.y, 0);
            redraw_Pacman(p->player_coord.pos.x, p->player_coord.pos.y,
                          p->player_coord.pos.x, p->player_coord.pos.y, 0);

            // Handle game over
            if (p->nlives <= 0) {
                draw_WallFull(p->player_coord.pos.x, p->player_coord.pos.y, Black, BOXSIZE);
								display_GameOver();
                return;
            }

            // Stop ghost's movement
            ghost->ghost_coord.next_pos.x = ghost->ghost_coord.pos.x;
            ghost->ghost_coord.next_pos.y = ghost->ghost_coord.pos.y;

            // Reinitialize the route
            init_Route(r);
            return;
        }
    }

    // Clear ghost's current position
    if (boardMatrix[current_y][current_x] == GHOSTPOS) {
        boardMatrix[current_y][current_x] = NOSPAWN;
    }

    // Update the grid and redraw the ghost
    draw_WallFull(current_x, current_y, Black, BOXSIZE);
    switch (boardMatrix[current_y][current_x]) {
        case STDSCORE:
            draw_Circle(current_x, current_y, STDRADIUS, Red);
            break;
        case POWERSCORE:
            draw_Circle(current_x, current_y, POWERRADIUS, Green);
            break;
        default:
            break;
    }

    // Update ghost position
    ghost->ghost_coord.pos.x = ghost->ghost_coord.next_pos.x;
    ghost->ghost_coord.pos.y = ghost->ghost_coord.next_pos.y;

    // Set ghost color based on vulnerability
    color = (ghost->vulnerable) ? Cyan : Red;
    draw_Character(ghost->ghost_coord.pos.x, ghost->ghost_coord.pos.y, ghostMatrix, color);
}
