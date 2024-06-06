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
	DrawLine(1, 1, 1, SCREEN_HEIGHT, RAYWHITE);
	DrawLine(1, SCREEN_HEIGHT - 1, GRID_WIDTH_PX, SCREEN_HEIGHT, RAYWHITE);
	DrawLine(GRID_WIDTH_PX, SCREEN_HEIGHT, GRID_WIDTH_PX, 0, RAYWHITE);
	DrawLine(1, 1, GRID_WIDTH_PX, 1, RAYWHITE);
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

void draw_screen(GameState *gs) {
	if (!gs->paused && !gs->game_over) {
		draw_grid(&gs->grid);
		draw_piece(gs->piece, gs->piece_x, gs->piece_y);
	} else if (gs->game_over) {
		DrawText("GAME OVER", 3, GRID_HEIGHT/2 * GRID_PIXELS, 40, RAYWHITE);
	} else {
		DrawText("PAUSED", 1.5 * GRID_PIXELS, GRID_HEIGHT/2 * GRID_PIXELS, 45, RAYWHITE);
	}

	draw_grid_outline();

	draw_score(gs->score);
	draw_level(gs->level);
	draw_cleared_lines(gs->lines);

	draw_next_piece(gs->next);
}
