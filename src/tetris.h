#ifndef TETRIS_H
#define TETRIS_H

#include <string.h>
#include <stdbool.h>

#define GRID_HEIGHT 20
#define GRID_WIDTH 10

#define PIECE_STARTING_X 3
#define PIECE_STARTING_Y GRID_HEIGHT - 4

#define DIFFERENT_PIECES 8

typedef enum {
	BLOCK_NONE,
	BLOCK_I,
	BLOCK_O,
	BLOCK_T,
	BLOCK_S,
	BLOCK_Z,
	BLOCK_J,
	BLOCK_L,
} BlockType;

typedef struct {
	BlockType blocks[GRID_WIDTH][GRID_HEIGHT];
} Grid;

#define PIECE_WIDTH 4
#define PIECE_HEIGHT 4
#define NUM_ROTATIONS 4

typedef struct {
	BlockType piece_type;
	int rotation;
	//BlockType shape[PIECE_WIDTH][PIECE_HEIGHT];
} Piece;

#define PIECE BlockType piece[PIECE_WIDTH][PIECE_HEIGHT]

typedef BlockType Shape[PIECE_WIDTH][PIECE_HEIGHT];

static const BlockType piece_shape_empty[PIECE_WIDTH][PIECE_HEIGHT] = {
	{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}
};

static const Shape piece_shape_O[NUM_ROTATIONS] = {
	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_O,    BLOCK_O,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},
};

static const Shape piece_shape_I[NUM_ROTATIONS] = {
	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_I,    BLOCK_I,    BLOCK_I,    BLOCK_I},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_I, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_I, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_I, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_I, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_I,    BLOCK_I,    BLOCK_I,    BLOCK_I},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_I, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_I, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_I, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_I, BLOCK_NONE, BLOCK_NONE}},
};

static const Shape piece_shape_T[NUM_ROTATIONS] = {
	{{BLOCK_NONE, BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_T,    BLOCK_T,    BLOCK_T,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_T,    BLOCK_T,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_T,    BLOCK_T,    BLOCK_T,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_T,    BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_T,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},
};

static const Shape piece_shape_S[NUM_ROTATIONS]	= {
	{{BLOCK_NONE, BLOCK_S,    BLOCK_S,    BLOCK_NONE},
	 {BLOCK_S,    BLOCK_S,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_S,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_S,    BLOCK_S,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_S,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_S,    BLOCK_S,    BLOCK_NONE},
	 {BLOCK_S,    BLOCK_S,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_S,    BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_S,    BLOCK_S,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_S,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}}
};

static const Shape piece_shape_Z[NUM_ROTATIONS] = {
	{{BLOCK_Z,    BLOCK_Z,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_Z,    BLOCK_Z,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_Z,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_Z,    BLOCK_Z,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_Z,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_Z,    BLOCK_Z,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_Z,    BLOCK_Z,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_Z,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_Z,    BLOCK_Z,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_Z,    BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},
};

static const Shape piece_shape_J[NUM_ROTATIONS] = {
	{{BLOCK_J,    BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_J,    BLOCK_J,    BLOCK_J,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_J,    BLOCK_J,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_J,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_J,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_J,    BLOCK_J,    BLOCK_J,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_J,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_J,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_J,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_J,    BLOCK_J,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},
};

static const Shape piece_shape_L[NUM_ROTATIONS] = {
	{{BLOCK_NONE, BLOCK_NONE, BLOCK_L,    BLOCK_NONE},
	 {BLOCK_L,    BLOCK_L,    BLOCK_L,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_L,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_L,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_L,    BLOCK_L,    BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_L,    BLOCK_L,    BLOCK_L,    BLOCK_NONE},
	 {BLOCK_L,    BLOCK_NONE, BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},

	{{BLOCK_L,    BLOCK_L,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_L,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_L,    BLOCK_NONE, BLOCK_NONE},
	 {BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE}},
};

int next_rotation(int rotation);

int previous_rotation(int rotation);

Piece get_piece(BlockType piece_type, int rotation);

const Shape* get_shape(Piece piece);

Piece get_random_piece();

void place_block(Grid *grid, BlockType block, int x, int y);

bool can_place_piece(Grid *grid, Piece piece, int x, int y);

void place_piece(Grid *grid, Piece piece, int x, int y);

#endif // TETRIS_H