#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#include "tetris.h"

#define MAX_INT pow(2, 32) - 1

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) > (Y)) ? (Y) : (X))

void init_gamestate(GameState *gs) {

	bool should_start_at_sc_start_screen = true;
	gs->scene = SC_GAME;

	gs->close_game = false;

	gs->show_fps = false;

	gs->pause_menu_line = 0;
	gs->controls_menu_line = 0;
	gs->game_over_menu_line = 0;
	gs->select_new_key = false;

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

	gs->clear_anim.active = false;
	gs->clear_anim.step = 0;
	// Run first step immediately
	gs->clear_anim.step_time = CLEAR_ANIMATION_INTERVAL;

	gs->piece = get_random_piece();
	gs->next = get_random_piece();

	initialize_default_keys(&(gs->keys));

	// Ensure no same pieces in a row
	while (gs->next.piece_type == gs->piece.piece_type)
		gs->next = get_random_piece();
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
	case BLOCK_HIDDEN:
		// This should absolutely not happen ever.
		return 0;
	}

	return NULL;
}

Piece get_random_piece() {
	BlockType piece_type = rand() % BLOCK_COUNT;
	BlockType rotation = 0;

	while (piece_type == BLOCK_NONE || piece_type == BLOCK_HIDDEN)
		piece_type = rand() % BLOCK_COUNT;

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

		if (!can_place(gs, -1, 0)) gs->shift_active = true;

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

		if (!can_place(gs, 1, 0)) gs->shift_active = true;

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

void update_clear_anim(GameState *gs) {
	if (gs->full_line_count > 0 && gs->clear_anim.active) {
		if (gs->clear_anim.step < CLEAR_ANIMATION_STEPS) {
			gs->clear_anim.step_time += GetFrameTime();
			if (gs->clear_anim.step_time >= CLEAR_ANIMATION_INTERVAL) {
				for (int i = 0; i < gs->full_line_count; i++) {
					int y = gs->full_lines[i];

					int left_x = 4 - gs->clear_anim.step;
					int right_x = 5 + gs->clear_anim.step;

					place_block(&gs->grid, BLOCK_HIDDEN, left_x, y);
					place_block(&gs->grid, BLOCK_HIDDEN, right_x, y);

				}
				gs->clear_anim.step++;
				gs->clear_anim.step_time = 0.0;
			}
		} else {
			drop_lines_down(&gs->grid);

			gs->clear_anim.active = false;
			gs->clear_anim.step = 0;
			gs->clear_anim.step_time = CLEAR_ANIMATION_INTERVAL;

			gs->full_line_count = 0;
		}
	}
}

bool can_update_game(GameState *gs) {
	if (gs->scene == SC_GAME && !gs->clear_anim.active) return true;

	return false;
}

// Drop the piece down, check if it lands
void update_game(GameState *gs) {
	gs->time_since_drop += GetFrameTime();

	if (time_to_drop(gs)) {
		gs->time_since_drop = 0.0;

		if (!can_place_piece(&gs->grid, gs->piece, gs->piece_x, gs->piece_y - 1)) {
			if (gs->piece_y == PIECE_STARTING_Y) {
				gs->scene = SC_GAME_OVER;
			} else {
				place_piece(&gs->grid, gs->piece, gs->piece_x, gs->piece_y);

				clear_lines(gs);

				if (gs->full_line_count > 0)
					gs->clear_anim.active = true;

				next_piece(gs);
			}
		} else {
			gs->piece_y--;
		}
	}
}

void handle_keys(GameState *gs) {
	switch (gs->scene) {
	case SC_START_SCREEN:
		// TODO
		break;
	case SC_CONTROLS_MENU:
		if (gs->select_new_key) {
			KeyboardKey new_key = GetKeyPressed();
			if (new_key != 0) {
				gs->keys[gs->controls_menu_line] = new_key;
				gs->select_new_key = false;
			}
		}

		if (IsKeyPressed(gs->keys[AC_MENU_SELECT])) {
			if (gs->controls_menu_line == AC_COUNT) {
				initialize_default_keys(&gs->keys);
			} else {
				gs->keys[gs->controls_menu_line] = KEY_NULL;
				gs->select_new_key = true;
			}
		}

		if (IsKeyPressed(gs->keys[AC_MENU_BACK])) {
			// TODO: Fix this when the time comes
			gs->scene = SC_PAUSED;
		}

		if (IsKeyPressed(gs->keys[AC_MENU_UP])) {
			// Also includes the reset controls option
			if (gs->controls_menu_line == 0)
				gs->controls_menu_line = AC_COUNT;
			else
				gs->controls_menu_line--;
		}

		if (IsKeyPressed(gs->keys[AC_MENU_DOWN])) {
			// Also includes the reset controls option
			if (gs->controls_menu_line == AC_COUNT)
				gs->controls_menu_line = 0;
			else
				gs->controls_menu_line++;
		}

		if (IsKeyPressed(gs->keys[AC_QUIT])) gs->close_game = true;

		break;
	case SC_PAUSED:
		if (IsKeyPressed(gs->keys[AC_MENU_BACK])) gs->scene = SC_GAME;
		if (IsKeyPressed(gs->keys[AC_PAUSE])) gs->scene = SC_GAME;

		if (IsKeyPressed(gs->keys[AC_MENU_DOWN])) {
			if (gs->pause_menu_line == PAUSE_MENU_LINES - 1)
				gs->pause_menu_line = 0;
			else
				gs->pause_menu_line++;
		}

		if (IsKeyPressed(gs->keys[AC_MENU_UP])) {
			if (gs->pause_menu_line == 0)
				gs->pause_menu_line = PAUSE_MENU_LINES - 1;
			else
				gs->pause_menu_line--;
		}

		if (IsKeyPressed(gs->keys[AC_MENU_SELECT])) {
			const char *option_text = pause_menu_options[gs->pause_menu_line];

			if (strcmp(option_text, "RESUME") == 0) gs->scene = SC_GAME;
			if (strcmp(option_text, "QUIT") == 0) gs->close_game = true;
			if (strcmp(option_text, "RESTART") == 0) {
				init_gamestate(gs);
				gs->scene = SC_GAME;
			}
			if (strcmp(option_text, "CONTROLS") == 0) {
				gs->scene = SC_CONTROLS_MENU;
				gs->controls_menu_line = 0;
			}
		}

		if (IsKeyPressed(gs->keys[AC_QUIT])) gs->close_game = true;

		break;
	case SC_GAME:
		if (!gs->clear_anim.active) {
			// Should this be handled differently? Allow both directions to be held?
			if (IsKeyDown(gs->keys[AC_MOVE_LEFT])) {
				move_left(gs);
			} else if (IsKeyDown(gs->keys[AC_MOVE_RIGHT])) {
				move_right(gs);
			} else {
				gs->dir_last_update = 0;
				gs->dir_time_held = 0.0;
				gs->shift_active = false;
			}

			if (IsKeyPressed(gs->keys[AC_ROTATE_FORWARD])) {
				Piece rotated_piece = get_piece(gs->piece.piece_type, next_rotation(gs->piece.rotation));
				if (can_place_piece(&gs->grid, rotated_piece, gs->piece_x, gs->piece_y)) {
					gs->piece = rotated_piece;
				}
			}

			if (IsKeyPressed(gs->keys[AC_ROTATE_BACKWARD])) {
				Piece rotated_piece = get_piece(gs->piece.piece_type, previous_rotation(gs->piece.rotation));
				if (can_place_piece(&gs->grid, rotated_piece, gs->piece_x, gs->piece_y)) {
					gs->piece = rotated_piece;
				}
			}

			if (IsKeyDown(gs->keys[AC_SOFT_DROP])) {
				gs->soft_drop = true;
			} else {
				gs->soft_drop = false;
			}
		}

		if (IsKeyPressed(gs->keys[AC_PAUSE])) gs->scene = SC_PAUSED;
		if (IsKeyPressed(gs->keys[AC_RESTART])) init_gamestate(gs);
		if (IsKeyPressed(gs->keys[AC_QUIT])) gs->close_game = true;

		break;
	case SC_GAME_OVER:
		if (IsKeyPressed(gs->keys[AC_RESTART])) {
			gs->scene = SC_GAME;
			init_gamestate(gs);
		}

		if (IsKeyPressed(gs->keys[AC_QUIT])) gs->close_game = true;

		if (IsKeyPressed(gs->keys[AC_MENU_DOWN])) {
			if (gs->game_over_menu_line == GAME_OVER_MENU_LINES - 1)
				gs->game_over_menu_line = 0;
			else
				gs->game_over_menu_line++;
		}

		if (IsKeyPressed(gs->keys[AC_MENU_UP])) {
			if (gs->game_over_menu_line == 0)
				gs->game_over_menu_line = GAME_OVER_MENU_LINES - 1;
			else
				gs->game_over_menu_line--;
		}

		if (IsKeyPressed(gs->keys[AC_MENU_SELECT])) {
			const char *option_text = game_over_menu_options[gs->game_over_menu_line];

			if (strcmp(option_text, "QUIT") == 0) gs->close_game = true;
			if (strcmp(option_text, "RESTART") == 0) {
				init_gamestate(gs);
				gs->scene = SC_GAME;
			}
		}

		break;
	default:
		// TODO: log error properly
		fprintf(stderr, "[ERROR] Unhandled scene reached!\n");
		exit(1);
		break;
	}
}
