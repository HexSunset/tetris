#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "draw.h"
#include "tetris.h"
#include "keys.h"

int main() {
	GameState gs;
	init_gamestate(&gs);

	bool close_game = false;

	printf("SCREEN_HEIGHT: %d\n", SCREEN_HEIGHT);
	printf("SCREEN_WIDTH: %d\n", SCREEN_WIDTH);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TETRIS");

	SetExitKey(gs.keys[ACTION_QUIT]);

	srand(time(0));

	SetTargetFPS(60);

	Image game_over_overlay_img = GenImageColor(GRID_WIDTH * GRID_PIXELS, GRID_HEIGHT * GRID_PIXELS, (Color) {0, 0, 0, 0x54});
	Texture2D game_over_overlay_tex = LoadTextureFromImage(game_over_overlay_img);

	while (!WindowShouldClose() && !close_game) {
		if (IsKeyPressed(gs.keys[ACTION_RESTART])) init_gamestate(&gs);

		if (IsKeyPressed(gs.keys[ACTION_TOGGLE_FPS])) gs.show_fps = !gs.show_fps;

		if (IsKeyPressed(gs.keys[ACTION_PAUSE])) {
			switch (gs.scene) {
			case SC_PAUSED:
				gs.scene = SC_GAME;
				break;
			case SC_GAME:
				gs.scene = SC_PAUSED;
				break;
			default:
				break;
			}
		}

		if (gs.scene == SC_GAME) {
			if (IsKeyDown(gs.keys[ACTION_MOVE_LEFT])) move_left(&gs);

			if (IsKeyDown(gs.keys[ACTION_MOVE_RIGHT])) move_right(&gs);

			if (!IsKeyDown(gs.keys[ACTION_MOVE_LEFT]) && !IsKeyDown(gs.keys[ACTION_MOVE_RIGHT])) {
				gs.dir_last_update = 0;
				gs.dir_time_held = 0.0;
				gs.shift_active = false;
			}

			if (IsKeyPressed(gs.keys[ACTION_ROTATE_COUNTERCLOCKWISE])) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, previous_rotation(gs.piece.rotation));
				if (can_place_piece(&gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyPressed(gs.keys[ACTION_ROTATE_CLOCKWISE])) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, next_rotation(gs.piece.rotation));
				if (can_place_piece(&gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyDown(gs.keys[ACTION_SOFT_DROP])) {
				gs.soft_drop = true;
			} else {
				gs.soft_drop = false;
			}

			if (time_to_drop(&gs)) {
				gs.time_since_drop = 0.0;

				if (!can_place_piece(&gs.grid, gs.piece, gs.piece_x, gs.piece_y - 1)) {
					if (gs.piece_y == PIECE_STARTING_Y) {
						gs.scene = SC_GAME_OVER;
					} else {
						place_piece(&gs.grid, gs.piece, gs.piece_x, gs.piece_y);

						clear_lines(&gs);

						next_piece(&gs);
					}

				} else {
					gs.piece_y--;
				}
			} else {
				gs.time_since_drop += GetFrameTime();
			}
		}

		if (gs.scene == SC_PAUSED) {
			// Make sure that we stay in the allowed row range
			if (IsKeyPressed(gs.keys[ACTION_MENU_DOWN])) {
				if (gs.pause_menu_line == PAUSE_MENU_LINES - 1)
					gs.pause_menu_line = 0;
				else
					gs.pause_menu_line += 1;
			}

			if (IsKeyPressed(gs.keys[ACTION_MENU_UP])) {
				if (gs.pause_menu_line == 0)
					gs.pause_menu_line = PAUSE_MENU_LINES - 1;
				else
					gs.pause_menu_line -= 1;
			}

			// TODO: Define menu items and interacting with them.
			if (IsKeyPressed(gs.keys[ACTION_MENU_SELECT])) {
				const char* option_text = pause_menu_options[gs.pause_menu_line];

				if (strcmp(option_text, "RESUME") == 0) gs.scene = SC_GAME;
				if (strcmp(option_text, "QUIT") == 0) close_game = true;
			}
		}

		draw_screen(&gs);
	}

	CloseWindow();

	return 0;
}
