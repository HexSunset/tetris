#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "draw.h"
#include "tetris.h"

int main() {
	GameState gs;
	init_gamestate(&gs);

	printf("SCREEN_HEIGHT: %d\n", SCREEN_HEIGHT);
	printf("SCREEN_WIDTH: %d\n", SCREEN_WIDTH);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TETRIS");

	SetExitKey(KEY_Q);

	srand(time(0));

	SetTargetFPS(60);

	Image game_over_overlay_img = GenImageColor(GRID_WIDTH * GRID_PIXELS, GRID_HEIGHT * GRID_PIXELS, (Color) {0, 0, 0, 0x54});
	Texture2D game_over_overlay_tex = LoadTextureFromImage(game_over_overlay_img);

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_R)) init_gamestate(&gs);

		if (IsKeyPressed(KEY_F)) gs.show_fps = !gs.show_fps;

		if (IsKeyPressed(KEY_SPACE)) gs.paused = !gs.paused;

		if (is_running(&gs)) {
			if (IsKeyDown(KEY_LEFT)) move_left(&gs);

			if (IsKeyDown(KEY_RIGHT)) move_right(&gs);

			if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
				gs.dir_last_update = 0;
				gs.dir_time_held = 0.0;
				gs.shift_active = false;
			}

			if (IsKeyPressed(KEY_Z)) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, previous_rotation(gs.piece.rotation));
				if (can_place_piece(&gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyPressed(KEY_X)) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, next_rotation(gs.piece.rotation));
				if (can_place_piece(&gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyDown(KEY_DOWN)) {
				gs.soft_drop = true;
			} else {
				gs.soft_drop = false;
			}

			if (time_to_drop(&gs)) {
				gs.time_since_drop = 0.0;

				if (!can_place_piece(&gs.grid, gs.piece, gs.piece_x, gs.piece_y - 1)) {
					if (gs.piece_y == PIECE_STARTING_Y) {
						gs.game_over = true;
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

		BeginDrawing();

		ClearBackground(BACKGROUND_COLOR);

		draw_screen(&gs);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
