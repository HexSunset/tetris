#include <stdlib.h>

#include "tetris.h"

int next_rotation(int rotation) {
	if (rotation == 3) {
		return 0;
	} else {
		return rotation + 1;
	}
}

int previous_rotation(int rotation) {
	if (rotation == 0) {
		return 3;
	} else {
		return rotation - 1;
	}
}

Piece get_piece(BlockType piece_type, int rotation) {
	Piece piece = {0};
	piece.piece_type = piece_type;
	piece.rotation = rotation;

	return piece;
}

const Shape* get_shape(Piece piece) {
	switch (piece.piece_type) {
	case BLOCK_NONE:
		return NULL;
	case BLOCK_I:
		return &piece_shape_I[piece.rotation];
	case BLOCK_O:
		return &piece_shape_O[piece.rotation];
	case BLOCK_T:
		return &piece_shape_T[piece.rotation];
	case BLOCK_J:
		return &piece_shape_J[piece.rotation];
	case BLOCK_L:
		return &piece_shape_L[piece.rotation];
	case BLOCK_S:
		return &piece_shape_S[piece.rotation];
	case BLOCK_Z:
		return &piece_shape_Z[piece.rotation];
	}

	return NULL;
}

Piece get_random_piece() {
	BlockType piece_type = rand() % DIFFERENT_PIECES;
	BlockType rotation = rand() % NUM_ROTATIONS;

	if (piece_type == BLOCK_NONE) piece_type++;

	return get_piece(piece_type, rotation);
}

bool can_place_piece(Grid *grid, Piece piece, int x, int y) {
	int grid_x, grid_y;

	const Shape* shape = get_shape(piece);

	for (int x_i = 0; x_i < PIECE_WIDTH; x_i++) {
		for (int y_i = 0; y_i < PIECE_HEIGHT; y_i++) {
			grid_x = x + x_i;
			grid_y = y + PIECE_HEIGHT - y_i - 1;

			BlockType block = (*shape)[y_i][x_i];

			if (block == BLOCK_NONE) continue;

			if (grid_x < 0 || grid_x >= GRID_WIDTH) return false;
			if (grid_y < 0 || grid_x >= GRID_HEIGHT) return false;
			if (grid->blocks[grid_x][grid_y] != BLOCK_NONE) return false;
		}
	}

	return true;
}

void place_piece(Grid *grid, Piece piece, int x, int y) {
	int grid_x;
	int grid_y;

	const Shape* shape = get_shape(piece);

	for (int x_i = 0; x_i < PIECE_WIDTH; x_i++) {
		for (int y_i = 0; y_i < PIECE_HEIGHT; y_i++) {
			grid_x = x + x_i;
			grid_y = y + PIECE_HEIGHT - y_i - 1;

			BlockType block = (*shape)[y_i][x_i];

			place_block(grid, block, grid_x, grid_y);
		}
	}
}

void place_block(Grid *grid, BlockType block, int x, int y) {
	if (block == BLOCK_NONE) return;
	grid->blocks[x][y] = block;
}
