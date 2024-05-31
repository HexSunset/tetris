#ifndef TETRIS_H
#define TETRIS_H

#include <string.h>
#include <stdbool.h>

#include "tetrominos.h"

#define GRID_HEIGHT 20
#define GRID_WIDTH 10

#define PIECE_STARTING_X 3
#define PIECE_STARTING_Y GRID_HEIGHT - 4

// This is based on NES Tetris gravity data
// We divide by 60 because that's the speed the
// NES runs on and the gravity is specified as
// "frames per gridcell", so this translates
// the values into "seconds per gridcell".
// There's thirty distinct values,
// so levels 29+ use the last value.
static float level_gravity[30] = {
	48 / 60, 43 / 60, 38 / 60, 33 / 60, 28 / 60,
	23 / 60, 18 / 60, 13 / 60,  8 / 60,  6 / 60,
	5 / 60,  4 / 60,  3 / 60,   2 / 60,  1 / 60
};

typedef struct {
	BlockType blocks[GRID_HEIGHT][GRID_WIDTH];
} Grid;

typedef struct {
	BlockType piece_type;
	int rotation;
} Piece;

typedef struct {
	bool paused;
	bool game_over;
	bool show_fps;

	unsigned int level;

	float piece_x;
	float piece_y;

	float base_drop_speed;
	float fast_drop_speed;
	float drop_speed;

	float horizontal_speed;

	Piece next;
	Piece piece;

	Grid grid;
} GameState;

void init_gamestate(GameState *gs);

void reset_gamestate(GameState *gs);

void next_piece(GameState *gs);

bool is_running(GameState *gs);

int next_rotation(int rotation);

int previous_rotation(int rotation);

Piece get_piece(BlockType piece_type, int rotation);

Piece get_random_piece();

Piece clone_piece(Piece piece);

const Shape* get_shape(Piece piece);

void place_block(Grid *grid, BlockType block, int x, int y);

bool can_place_piece(Grid *grid, Piece piece, int x, int y);

void place_piece(Grid *grid, Piece piece, int x, int y);

bool line_is_full(Grid *grid, int y);

bool line_is_empty(Grid *grid, int y);

void move_line(Grid *grid, int src, int dest);

void drop_lines(Grid *grid);

bool can_clear_lines(Grid *grid);

int clear_lines(Grid *grid);

#endif // TETRIS_H
