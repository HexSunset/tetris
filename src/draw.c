#include <stdio.h>

#include "draw.h"
#include "tetris.h"

Color block_color(BlockType bt) {
	switch (bt) {
	case BLOCK_NONE:
		return BACKGROUND_COLOR;
	case BLOCK_I:
		return SKYBLUE;
	case BLOCK_O:
		return YELLOW;
	case BLOCK_T:
		return PURPLE;
	case BLOCK_S:
		return GREEN;
	case BLOCK_Z:
		return RED;
	case BLOCK_J:
		return DARKBLUE;
	case BLOCK_L:
		return ORANGE;
	}

	return BLACK;
}

void draw_block(int x, int y, Color color) {
	int top_left_x_coord = x * GRID_PIXELS + GRID_BORDER_PIXELS;
	int top_left_y_coord = (GRID_HEIGHT - y - 1) * GRID_PIXELS + GRID_BORDER_PIXELS;
	int block_width = GRID_PIXELS - GRID_BORDER_PIXELS;
	int block_height = GRID_PIXELS - GRID_BORDER_PIXELS;

	DrawRectangle(top_left_x_coord, top_left_y_coord, block_width, block_height, color);
}

void draw_block_screen(int screen_x, int screen_y, Color color) {
	int top_left_x = screen_x + GRID_BORDER_PIXELS;
	int top_left_y = screen_y + GRID_BORDER_PIXELS;
	int block_width = GRID_PIXELS - GRID_BORDER_PIXELS;
	int block_height = GRID_PIXELS - GRID_BORDER_PIXELS;

	DrawRectangle(top_left_x, top_left_y, block_width, block_height, color);
}

void draw_grid(Grid *grid) {
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
			BlockType bt = grid->blocks[y][x];
			draw_block(x, y, block_color(bt));
		}
	}
}

void draw_grid_outline() {

	int left_x = 1;
	int right_x = GRID_WIDTH_PX + GRID_BORDER_PIXELS;
	int top_y = 1;
	int bottom_y = SCREEN_HEIGHT - 1;

	DrawLine(left_x, top_y, left_x, bottom_y, RAYWHITE);
	DrawLine(left_x, bottom_y, right_x, bottom_y, RAYWHITE);
	DrawLine(right_x, bottom_y, right_x, top_y, RAYWHITE);
	DrawLine(left_x, top_y, right_x, top_y, RAYWHITE);
}

void draw_piece(Piece piece, int x, int y) {
	const Shape* shape = get_shape(piece);

	for (int x_i = 0; x_i < PIECE_WIDTH; x_i++) {
		for (int y_i = 0; y_i < PIECE_HEIGHT; y_i++) {
			int grid_x = x + x_i;
			int grid_y = y + y_i;

			BlockType block = (*shape)[PIECE_HEIGHT - y_i - 1][x_i];

			if (block == BLOCK_NONE) continue;

			draw_block(grid_x, grid_y, block_color(block));
		}
	}
}

void draw_level(int level) {
	char str1[100];
	char str2[100];
	sprintf(str1, "LEVEL");
	DrawText(str1, GRID_WIDTH_PX + 15, 20, 20, RAYWHITE);
	sprintf(str2, "%d", level);
	DrawText(str2, GRID_WIDTH_PX + 15, 40, 20, RAYWHITE);
}

void draw_cleared_lines(int lines) {
	char str1[100];
	char str2[100];
	sprintf(str1, "LINES");
	DrawText(str1, GRID_WIDTH_PX + 15, 80, 20, RAYWHITE);
	sprintf(str2, "%d", lines);
	DrawText(str2, GRID_WIDTH_PX + 15, 100, 20, RAYWHITE);

	//DrawText(str, 0, 20, 20, RAYWHITE);
}

void draw_score(int score) {
	char str1[100];
	char str2[100];
	sprintf(str1, "SCORE");
	DrawText(str1, GRID_WIDTH_PX + 15, 140, 20, RAYWHITE);
	sprintf(str2, "%d", score);
	DrawText(str2, GRID_WIDTH_PX + 15, 160, 20, RAYWHITE);
}

void draw_next_piece(Piece piece) {
	char *text = "NEXT";

	const Shape *shape = get_shape(piece);

	int x_offset = GRID_WIDTH + 1;
	int y_offset = GRID_HEIGHT - 15;

	//printf("\rx_offset = %d; y_offset = %d;", x_offset, y_offset);

	DrawText(text, GRID_WIDTH_PX + 15, (y_offset + 6) * GRID_PIXELS, 20, RAYWHITE);

	for (int x = 0; x < PIECE_WIDTH; x++) {
		for (int y = 0; y < PIECE_HEIGHT; y++) {
			BlockType block = (*shape)[PIECE_HEIGHT - y - 1][x];

			if (block == BLOCK_NONE) continue;

			draw_block(x_offset + x, y_offset + y, block_color(block));
		}
	}
}

void draw_pause_menu(GameState *gs) {
	//DrawText(const char *text, int posX, int posY, int fontSize, Color color)
	int menu_start_x = 2.5 * GRID_PIXELS;
	int spacing = 2 * GRID_PIXELS;
	int menu_start_y = GRID_HEIGHT/4 * GRID_PIXELS + 2 * spacing;

	for (size_t i = 0; i < PAUSE_MENU_LINES; i++) {
		DrawText(pause_menu_options[i], menu_start_x, menu_start_y + i * spacing, 20, RAYWHITE);
		if (i == gs->pause_menu_line)
			DrawRectangle(1.5 * GRID_PIXELS, menu_start_y + i * spacing + 3, 11, 11, RAYWHITE);
	}
}

void draw_controls_menu(GameState *gs) {
	int menu_start_x = 1.5 * GRID_PIXELS;
	int spacing = GRID_PIXELS;
	int menu_start_y = GRID_HEIGHT/6 * GRID_PIXELS;

	for (size_t i = 0; i < ACTION_COUNT; i++) {
		DrawText(action_names[i], menu_start_x, menu_start_y + i * spacing, 20, RAYWHITE);
		if (gs->controls_menu_line == i) {
			DrawText(key_to_str(gs->keys[i]), SCREEN_WIDTH - 5 * GRID_PIXELS, menu_start_y + i * spacing, 20, RED);
		} else {
			DrawText(key_to_str(gs->keys[i]), SCREEN_WIDTH - 5 * GRID_PIXELS, menu_start_y + i * spacing, 20, RAYWHITE);
		}
	}
	if (gs->controls_menu_line == ACTION_COUNT) {
		DrawText("RESET CONTROLS", menu_start_x, menu_start_y + (ACTION_COUNT + 1) * spacing, 20, RED);
	} else {
		DrawText("RESET CONTROLS", menu_start_x, menu_start_y + (ACTION_COUNT + 1) * spacing, 20, RAYWHITE);
	}
}

void draw_screen(GameState *gs) {
	BeginDrawing();

	ClearBackground(BACKGROUND_COLOR);

	if (gs->scene == SC_GAME) {
		draw_grid(&gs->grid);
		draw_piece(gs->piece, gs->piece_x, gs->piece_y);
	} else if (gs->scene == SC_GAME_OVER) {
		DrawText("GAME OVER", 3, GRID_HEIGHT/2 * GRID_PIXELS, 40, RAYWHITE);
	} else if (gs->scene == SC_PAUSED) {
		DrawText("PAUSED", 1.5 * GRID_PIXELS, GRID_HEIGHT/4 * GRID_PIXELS, 45, RAYWHITE);
		draw_pause_menu(gs);
	} else if (gs->scene == SC_CONTROLS_MENU) {
		DrawText("CONTROLS", 1.5 * GRID_PIXELS, GRID_PIXELS, 45, RAYWHITE);
		draw_controls_menu(gs);
	}

	if (gs->show_fps)
		DrawFPS(0, 0);

	if (gs->scene == SC_GAME || gs->scene == SC_GAME_OVER || gs->scene == SC_PAUSED) {
		if (gs->scene != SC_PAUSED)
			draw_next_piece(gs->next);

		draw_grid_outline();

		draw_score(gs->score);
		draw_level(gs->level);
		draw_cleared_lines(gs->lines);
	}

	if (gs->full_line_count > 0) {
		for (int step = gs->animation_progress; step < CLEAR_ANIMATION_STEPS; step++) {
			int left_x = 4 - step;
			int right_x = 5 + step;

			BeginDrawing();

			for (int y_i = 0; y_i < gs->full_line_count; y_i++) {
				int y = gs->full_lines[y_i];
				draw_block(left_x, y, BACKGROUND_COLOR);
				draw_block(right_x, y, BACKGROUND_COLOR);
			}

			EndDrawing();

			WaitTime(CLEAR_ANIMATION_INTERVAL);
		}

		drop_lines_down(&gs->grid);
		gs->clear_animation = false;
		gs->full_line_count = 0;
	}

	EndDrawing();
}
