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

void draw_grid(Grid *grid) {
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
			BlockType bt = grid->blocks[y][x];
			draw_block(x, y, block_color(bt));
		}
	}
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
	char str[500];
	sprintf(str, "LEVEL: %d", level);

	DrawText(str, 0, 0, 20, RAYWHITE);
}

void draw_cleared_lines(int lines) {
	char str[500];
	sprintf(str, "LINES: %d", lines);

	DrawText(str, 0, 20, 20, RAYWHITE);
}

void draw_score(int score) {
	char str[500];
	sprintf(str, "SCORE: %d", score);

	DrawText(str, 0, 40, 20, RAYWHITE);
}
