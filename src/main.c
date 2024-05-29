#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "draw.h"
#include "tetris.h"

//Grid gs.grid = {0};

int main() {
	GameState gs = init_gamestate();

	InitWindow(GRID_WIDTH * GRID_PIXELS, GRID_HEIGHT * GRID_PIXELS, "TETRIS");

	srand(time(0));

	SetTargetFPS(0);

	//float piece_x = PIECE_STARTING_X;
	//float piece_y = PIECE_STARTING_Y;

	//Piece current_piece = get_random_piece();

	//float level_drop_speed = 4.0;
	//float fast_drop_speed = 12.0;

	//float current_drop_speed = level_drop_speed;

	//bool paused = false;
	//bool game_over = false;
	//bool show_fps = false;

	Image game_over_overlay_img = GenImageColor(GRID_WIDTH * GRID_PIXELS, GRID_HEIGHT * GRID_PIXELS, (Color) {0, 0, 0, 0x54});
	Texture2D game_over_overlay_tex = LoadTextureFromImage(game_over_overlay_img);

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_R)) reset_gamestate(&gs);

		if (IsKeyPressed(KEY_F)) gs.show_fps = !gs.show_fps;

		if (IsKeyPressed(KEY_SPACE)) gs.paused = !gs.paused;

		if (is_running(&gs)) {
			if (IsKeyDown(KEY_LEFT)) {
				float delta_x = GetFrameTime() * gs.horizontal_speed;
				if (can_place_piece(gs.grid, gs.piece, gs.piece_x - delta_x, gs.piece_y)) {
					gs.piece_x -= delta_x;
				}
			}

			if (IsKeyDown(KEY_RIGHT)) {
				float delta_x = GetFrameTime() * gs.horizontal_speed;
				if (can_place_piece(gs.grid, gs.piece, gs.piece_x + delta_x, gs.piece_y)) {
					gs.piece_x += delta_x;
				}
			}

			if (IsKeyPressed(KEY_Z)) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, previous_rotation(gs.piece.rotation));
				if (can_place_piece(gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyPressed(KEY_X)) {
				Piece rotated_piece = get_piece(gs.piece.piece_type, next_rotation(gs.piece.rotation));
				if (can_place_piece(gs.grid, rotated_piece, gs.piece_x, gs.piece_y)) {
					gs.piece = rotated_piece;
				}
			}

			if (IsKeyDown(KEY_DOWN)) {
				gs.drop_speed = gs.fast_drop_speed;
			} else {
				gs.drop_speed = gs.base_drop_speed;
			}

			float next_frame_y = floor(gs.piece_y - GetFrameTime() * gs.drop_speed);
			if (!can_place_piece(gs.grid, gs.piece, gs.piece_x, next_frame_y)) {
				place_piece(gs.grid, gs.piece, gs.piece_x, floor(gs.piece_y));

				if (floor(gs.piece_y) >= PIECE_STARTING_Y - 1) {
					gs.game_over = true;
					gs.paused = false;
				} else {
					next_piece(&gs);
				}

				if (can_clear_lines(gs.grid)) clear_lines(gs.grid);
			}

			gs.piece_y -= GetFrameTime() * gs.drop_speed;
		}

		BeginDrawing();

		ClearBackground(BACKGROUND_COLOR);
		if (gs.paused) {
			DrawText("PAUSED", 1.5 * GRID_PIXELS, GRID_HEIGHT/2 * GRID_PIXELS, 45, RAYWHITE);
		} else {
			draw_grid(gs.grid);
			draw_piece(gs.piece, gs.piece_x, floor(gs.piece_y));
		}

		if (gs.game_over) {
			DrawTexture(game_over_overlay_tex, 0, 0, RAYWHITE);
			DrawText("GAME OVER", GRID_PIXELS, GRID_HEIGHT/2 * GRID_PIXELS, 34, RAYWHITE);
		}

		if (gs.show_fps) {
			DrawFPS(0, 0);
		}


		EndDrawing();
	}

	CloseWindow();

	destroy_gamestate(&gs);

	return 0;
}
