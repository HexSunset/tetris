#ifndef TETRIS_H
#define TETRIS_H

#include <string.h>
#include <stdbool.h>

#include "tetrominos.h"

#define GRID_HEIGHT 20
#define GRID_WIDTH 10

#define PIECE_STARTING_X 3
// Align at the very top with rotation 0
#define PIECE_STARTING_Y GRID_HEIGHT - 3

// Look at get_gravity to find out how these
// are matched to level numbers.
// Based entirely on NES tetris speeds.
static const float level_gravity[30] = {
	48.0 / 60.0,
	43.0 / 60.0,
	38.0 / 60.0,
	33.0 / 60.0,
	28.0 / 60.0,
	23.0 / 60.0,
	18.0 / 60.0,
	13.0 / 60.0,
	8.0 / 60.0,
	6.0 / 60.0,
	5.0 / 60.0,
	4.0 / 60.0,
	3.0 / 60.0,
	2.0 / 60.0,
	1.0 / 60.0
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

	int start_level;
	int level;
	int lines_level;
	int lines;

	int score;

	int piece_x;
	int piece_y;

	bool soft_drop; // stronger gravity when holding down
	float time_since_drop;

	float horizontal_speed;

	Piece next;
	Piece piece;

	Grid grid;
} GameState;

void init_gamestate(GameState *gs);

void reset_gamestate(GameState *gs);

void next_piece(GameState *gs);

float get_gravity(GameState *gs);

bool is_running(GameState *gs);

int next_rotation(int rotation);

int previous_rotation(int rotation);

bool time_to_drop(GameState *gs);

Piece get_piece(BlockType piece_type, int rotation);

Piece get_random_piece();

Piece clone_piece(Piece piece);

const Shape* get_shape(Piece piece);

void place_block(Grid *grid, BlockType block, int x, int y);

bool can_place_piece(Grid *grid, Piece piece, int x, int y);

bool can_place(GameState *gs, int x_offset, int y_offset);

void place_piece(Grid *grid, Piece piece, int x, int y);

bool line_is_full(Grid *grid, int y);

bool line_is_empty(Grid *grid, int y);

void move_line(Grid *grid, int src, int dest);

void drop_lines(Grid *grid);

bool can_clear_lines(Grid *grid);

void clear_lines(GameState *gs);

#endif // TETRIS_H
