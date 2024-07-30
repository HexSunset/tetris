#include <stdio.h>
#include <raylib.h>
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

	SetExitKey(KEY_NULL);

	srand(time(0));

	SetTargetFPS(60);

	while (!WindowShouldClose() && !close_game) {
		if (!gs.select_new_key) {
			if (IsKeyPressed(gs.keys[AC_RESTART])) init_gamestate(&gs);

			if (IsKeyPressed(gs.keys[AC_TOGGLE_FPS])) gs.show_fps = !gs.show_fps;

			if (IsKeyPressed(gs.keys[AC_PAUSE])) {
				switch (gs.scene) {
				case SC_PAUSED:
					gs.scene = SC_GAME;
					break;
				case SC_GAME:
					gs.scene = SC_PAUSED;
					gs.pause_menu_line = 0;
					break;
				default:
					break;
				}
			}

			if (IsKeyPressed(gs.keys[AC_QUIT]))
				close_game = true;
		}

		if (gs.scene == SC_GAME) {
			if (IsKeyDown(gs.keys[AC_MOVE_LEFT])) move_left(&gs);

			if (IsKeyDown(gs.keys[AC_MOVE_RIGHT])) move_right(&gs);

			if (!IsKeyDown(gs.keys[AC_MOVE_LEFT]) && !IsKeyDown(gs.keys[AC_MOVE_RIGHT])) {
				gs.dir_last_update = 0;
				gs.dir_time_held = 0.0;
				gs.shift_active = false;
			}

			if (IsKeyPressed(gs.keys[AC_ROTATE_BACKWARD])) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, previous_rotation(gs.piece.rotation));
				if (can_place_piece(&gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyPressed(gs.keys[AC_ROTATE_FORWARD])) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, next_rotation(gs.piece.rotation));
				if (can_place_piece(&gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyDown(gs.keys[AC_SOFT_DROP])) {
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

		else if (gs.scene == SC_PAUSED) {
			// Make sure that we stay in the allowed row range
			if (IsKeyPressed(gs.keys[AC_MENU_DOWN])) {
				if (gs.pause_menu_line == PAUSE_MENU_LINES - 1)
					gs.pause_menu_line = 0;
				else
					gs.pause_menu_line += 1;
			}

			if (IsKeyPressed(gs.keys[AC_MENU_UP])) {
				if (gs.pause_menu_line == 0)
					gs.pause_menu_line = PAUSE_MENU_LINES - 1;
				else
					gs.pause_menu_line -= 1;
			}

			if (IsKeyPressed(gs.keys[AC_MENU_BACK])) gs.scene = SC_GAME;

			// TODO: Define menu items and interacting with them.
			if (IsKeyPressed(gs.keys[AC_MENU_SELECT])) {
				const char* option_text = pause_menu_options[gs.pause_menu_line];

				if (strcmp(option_text, "RESUME") == 0) gs.scene = SC_GAME;
				if (strcmp(option_text, "CONTROLS") == 0) {
					gs.scene = SC_CONTROLS_MENU;
					gs.controls_menu_line = 0;
					while (GetKeyPressed() != 0) {}
				}
				if (strcmp(option_text, "QUIT") == 0) close_game = true;
			}
		}

		else if (gs.scene == SC_CONTROLS_MENU) {
			if (gs.select_new_key) {
				KeyboardKey new_key = GetKeyPressed();
				if (new_key != 0) {
					gs.keys[gs.controls_menu_line] = new_key;
					gs.select_new_key = false;
				}
			} else {
				// TODO: expand this when there's more ways to get to controls menu
				if (IsKeyPressed(gs.keys[AC_MENU_BACK])) gs.scene = SC_PAUSED;
				if (IsKeyPressed(gs.keys[AC_MENU_DOWN])) {
					// Also includes the reset controls option
					if (gs.controls_menu_line == AC_COUNT)
						gs.controls_menu_line = 0;
					else
						gs.controls_menu_line++;
				}
				if (IsKeyPressed(gs.keys[AC_MENU_UP])) {
					// Also includes the reset controls option
					if (gs.controls_menu_line == 0)
						gs.controls_menu_line = AC_COUNT;
					else
						gs.controls_menu_line--;
				}
				if (IsKeyPressed(gs.keys[AC_MENU_SELECT])) {
					if (gs.controls_menu_line == AC_COUNT) {
						initialize_default_keys(&(gs.keys));
					} else {
						gs.keys[gs.controls_menu_line] = KEY_NULL;
						gs.select_new_key = true;
					}
				}
			}
		}

		draw_screen(&gs);
	}

	CloseWindow();

	return 0;
}
