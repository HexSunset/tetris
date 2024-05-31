#include <stdlib.h>

#include "tetris.h"

void init_gamestate(GameState *gs) {
	gs->paused = false;
	gs->game_over = false;
	gs->show_fps = false;

	gs->level = 0;

	gs->piece_x = PIECE_STARTING_X;
	gs->piece_y = PIECE_STARTING_Y;

	gs->base_drop_speed = 4.0;
	gs->fast_drop_speed = 12.0;
	gs->drop_speed = gs->base_drop_speed;

	gs->horizontal_speed = 10.0;
	//gs->grid = {0};
	memset(&(gs->grid), 0, sizeof(Grid));

	gs->piece = get_random_piece();
	gs->next = get_random_piece();

	// Ensure no same pieces in a row
	while (gs->next.piece_type == gs->piece.piece_type)
		gs->next = get_random_piece();
}

bool is_running(GameState *gs) {
	return !gs->paused && !gs->game_over;
}

void next_piece(GameState *gs) {
	gs->piece_x = PIECE_STARTING_X;
	gs->piece_y = PIECE_STARTING_Y;
	gs->piece = gs->next;

	gs->next = get_random_piece();
	while (gs->next.piece_type == gs->piece.piece_type)
		gs->next = get_random_piece();
}

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

Piece clone_piece(Piece piece) {
	return piece;
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
	BlockType piece_type = rand() % BLOCK_TYPE_COUNT;
	BlockType rotation = 0;

	while (piece_type == BLOCK_NONE)
		piece_type = rand() % BLOCK_TYPE_COUNT;

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
			if (grid->blocks[grid_y][grid_x] != BLOCK_NONE) return false;
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
	grid->blocks[y][x] = block;
}

bool line_is_empty(Grid *grid, int y) {
	for (int x = 0; x < GRID_WIDTH; x++) {
		if (grid->blocks[y][x] != BLOCK_NONE) return false;
	}

	return true;
}

bool line_is_full(Grid *grid, int y) {
	for (int x = 0; x < GRID_WIDTH; x++) {
		if (grid->blocks[y][x] == BLOCK_NONE) return false;
	}

	return true;
}

void move_line(Grid *grid, int src, int dest) {
	for (int x = 0; x < GRID_WIDTH; x++) {
		grid->blocks[dest][x] = grid->blocks[src][x];
	}
}

void drop_lines_down(Grid *grid) {
	int y = 0;
	while (y < GRID_HEIGHT) {
		if (!line_is_full(grid, y)) {
			y++;
			continue;
		} else {
			for (int y2 = y + 1; y2 < GRID_HEIGHT; y2++) {
				move_line(grid, y2, y2 - 1);
			}

			if (!line_is_full(grid, y)) y++;
		}
	}
}

bool can_clear_lines(Grid *grid) {
	for (int y = 0; y < GRID_HEIGHT; y++) {
		if (line_is_full(grid, y)) {
			return true;
		}
	}

	return false;
}

int clear_lines(Grid *grid) {
	int full_line_count = 0;

	for (int y = 0; y < GRID_HEIGHT; y++) {
		if (line_is_full(grid, y)) {
			full_line_count++;
		}
	}

	if (full_line_count > 0) drop_lines_down(grid);

	return full_line_count;
}
