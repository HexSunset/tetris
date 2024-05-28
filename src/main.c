#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "draw.h"
#include "tetris.h"

Grid grid = {0};

int main() {
	InitWindow(GRID_WIDTH * GRID_PIXELS, GRID_HEIGHT * GRID_PIXELS, "TETRIS");

	srand(time(0));

	SetTargetFPS(0);

	// TODO: GameState struct that contains all of this
	float piece_x = PIECE_STARTING_X;
	float piece_y = PIECE_STARTING_Y;

	Piece current_piece = get_random_piece();

	float level_drop_speed = 4.0;
	float fast_drop_speed = 12.0;

	float current_drop_speed = level_drop_speed;

	bool paused = false;
	bool game_over = false;
	bool show_fps = false;

	// 50% opacity black
	Image game_over_overlay_img = GenImageColor(GRID_WIDTH * GRID_PIXELS, GRID_HEIGHT * GRID_PIXELS, (Color) {0, 0, 0, 0x54});
	Texture2D game_over_overlay_tex = LoadTextureFromImage(game_over_overlay_img);

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_F)) show_fps = !show_fps;
		
		if (!game_over) {
			if (IsKeyPressed(KEY_SPACE)) paused = !paused;
					
			if (IsKeyPressed(KEY_LEFT)) {
				if (can_place_piece(&grid, current_piece, piece_x - 1, piece_y)) {
					piece_x--;
				}
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				if (can_place_piece(&grid, current_piece, piece_x + 1, piece_y)) {
					piece_x++;
				}
			}

			if (IsKeyPressed(KEY_Z)) {
				Piece rotated_piece = get_piece(current_piece.piece_type, previous_rotation(current_piece.rotation));
				if (can_place_piece(&grid, rotated_piece, piece_x, piece_y)) {
					current_piece = rotated_piece;
				}
			}

			if (IsKeyPressed(KEY_X)) {
				Piece rotated_piece = get_piece(current_piece.piece_type, next_rotation(current_piece.rotation));
				if (can_place_piece(&grid, rotated_piece, piece_x, piece_y)) {
					current_piece = rotated_piece;
				}
			}

			if (IsKeyDown(KEY_DOWN)) {
				current_drop_speed = fast_drop_speed;
			} else {
				current_drop_speed = level_drop_speed;
			}

			if (!paused) {
				float next_frame_y = floor(piece_y - GetFrameTime() * current_drop_speed);
				if (!can_place_piece(&grid, current_piece, piece_x, next_frame_y)) {
					place_piece(&grid, current_piece, piece_x, floor(piece_y));

					if (floor(piece_y) >= PIECE_STARTING_Y - 1) {
						game_over = true;
						paused = false;
					} else {
						piece_x = PIECE_STARTING_X;
						piece_y = PIECE_STARTING_Y;
						current_piece = get_random_piece();
					}
				}

				piece_y -= GetFrameTime() * current_drop_speed;
			}
		}

		BeginDrawing();

		ClearBackground(BACKGROUND_COLOR);
		if (paused) {
			DrawText("PAUSED", 1.5 * GRID_PIXELS, GRID_HEIGHT/2 * GRID_PIXELS, 45, RAYWHITE);
		} else {
			draw_grid(&grid);

			if (!game_over)
				draw_piece(current_piece, piece_x, floor(piece_y));

			if (game_over) {
				DrawTexture(game_over_overlay_tex, 0, 0, RAYWHITE);
				DrawText("GAME OVER", GRID_PIXELS, GRID_HEIGHT/2 * GRID_PIXELS, 34, RAYWHITE);
			}

			if (show_fps) {
				DrawFPS(0, 0);
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
