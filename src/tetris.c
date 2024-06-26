#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#include "tetris.h"

#define MAX_INT pow(2, 32) - 1

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) > (Y)) ? (Y) : (X))

void init_gamestate(GameState *gs) {
	gs->paused = false;
	gs->game_over = false;
	gs->show_fps = false;

	gs->start_level = 8;
	gs->level = gs->start_level;
	gs->lines_level = 0;
	gs->lines = 0;

	gs->score = 0;

	gs->piece_x = PIECE_STARTING_X;
	gs->piece_y = PIECE_STARTING_Y;

	gs->soft_drop = false;
	gs->time_since_drop = 0;

	gs->shift_active = false;
	gs->dir_last_update = 0;
	gs->dir_time_held = 0.0;

	memset(&(gs->grid), 0, sizeof(gs->grid));

	gs->full_line_count = 0;

	gs->clear_animation = false;
	gs->animation_progress = 0;

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

float get_gravity(GameState *gs) {
	float gravity;

	if (gs->level <= 9)                     gravity = level_gravity[gs->level];
	if (gs->level >= 10 && gs->level <= 12) gravity = level_gravity[10];
	if (gs->level >= 13 && gs->level <= 15) gravity = level_gravity[11];
	if (gs->level >= 16 && gs->level <= 18) gravity = level_gravity[12];
	if (gs->level >= 19 && gs->level <= 28) gravity = level_gravity[13];
	if (gs->level >= 29)                    gravity = level_gravity[14];

	if (gs->soft_drop) return gravity / 2.0;
	else return gravity;
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

bool time_to_drop(GameState *gs) {
	if (gs->time_since_drop >= get_gravity(gs)) return true;

	return false;
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

void move_left(GameState *gs) {
	if (gs->dir_last_update == -1)
		gs->dir_time_held += GetFrameTime();
	else
		gs->dir_time_held = GetFrameTime();

	if (!gs->shift_active) {
		if (gs->dir_last_update != -1 && can_place(gs, -1, 0))
			gs->piece_x--;

		if (gs->dir_time_held >= SHIFT_DELAY) {
			gs->shift_active = true;
		}
	}

	if (gs->shift_active) {
		if (gs->dir_last_update == -1) {
			if (gs->dir_time_held >= SHIFT_INTERVAL && can_place(gs, -1, 0)) {
				gs->piece_x--;
				gs->dir_time_held = 0.0;
			}
		}
		else {
			gs->shift_active = false;
		}
	}

	gs->dir_last_update = -1;
}

void move_right(GameState *gs) {
	if (gs->dir_last_update == 1)
		gs->dir_time_held += GetFrameTime();
	else
		gs->dir_time_held = GetFrameTime();

	if (!gs->shift_active) {
		if (gs->dir_last_update != 1 && can_place(gs, 1, 0))
			gs->piece_x++;

		if (gs->dir_time_held >= SHIFT_DELAY) {
			gs->shift_active = true;
		}
	}

	if (gs->shift_active) {
		if (gs->dir_last_update == 1) {
			if (gs->dir_time_held >= SHIFT_INTERVAL && can_place(gs, 1, 0)) {
				gs->piece_x++;
				gs->dir_time_held = 0.0;
			}
		}
		else {
			gs->shift_active = false;
		}
	}

	gs->dir_last_update = 1;
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

bool can_place(GameState *gs, int x_offset, int y_offset) {
	return can_place_piece(&(gs->grid), gs->piece, gs->piece_x + x_offset, gs->piece_y + y_offset);
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

int level_line_req(int level, int start_level) {
	if (level == start_level) {
		return MIN(start_level * 10 + 10, MAX(100, start_level * 10 -50));
	}

	return level * 10;
}

bool can_increase_level(GameState *gs) {
	if (level_line_req(gs->level + 1, gs->start_level) <= gs->lines) return true;
	else return false;
}

int calculate_score(int lines, int level) {
	if (lines == 1) return 40   * (level + 1);
	if (lines == 2) return 100  * (level + 1);
	if (lines == 3) return 300  * (level + 1);
	if (lines == 4) return 1200 * (level + 1);

	return 0;
}

void clear_lines(GameState *gs) {
	gs->full_line_count = 0;

	for (int y = 0; y < GRID_HEIGHT; y++) {
		if (line_is_full(&gs->grid, y)) {
			gs->full_lines[gs->full_line_count] = y;
			gs->full_line_count++;
		}
	}

	/* if (full_line_count > 0) { */
	/*	drop_lines_down(&gs->grid); */
	/* } */

	gs->lines += gs->full_line_count;
	gs->score += calculate_score(gs->full_line_count, gs->level);

	if (can_increase_level(gs)) gs->level++;
}
