#ifndef TETRIS_DRAW_H
#define TETRIS_DRAW_H

#include <raylib.h>

#include "tetris.h"

#define GRID_PIXELS 25
#define GRID_BORDER_PIXELS 3

#define GRID_WIDTH_PX GRID_WIDTH * GRID_PIXELS
#define GRID_HEIGHT_PX GRID_HEIGHT * GRID_PIXELS

#define INFO_DISPLAY_WIDTH 6 * GRID_PIXELS

#define NEXT_PIECE_DISPLAY_SCREEN_WIDTH INFO_DISPLAY_WIDTH
#define NEXT_PIECE_DISPLAY_SCREEN_HEIGHT INFO_DISPLAY_WIDTH

#define SECTION_BORDER_PX 3

#define SCREEN_WIDTH GRID_WIDTH_PX + SECTION_BORDER_PX + NEXT_PIECE_DISPLAY_SCREEN_WIDTH
#define SCREEN_HEIGHT GRID_HEIGHT_PX + SECTION_BORDER_PX

#define BACKGROUND_COLOR (Color) {0x16, 0x16, 0x16, 0xFF}

Color block_color(BlockType bt);

void draw_block(int x, int y, Color color);

void draw_piece(Piece piece, int x, int y);

void draw_grid(Grid *grid);
void draw_grid_outline();

void draw_level(int level);
void draw_cleared_lines(int lines);
void draw_score(int score);

void draw_next_piece(Piece piece);

void draw_pause_menu(GameState *gs);

void draw_controls_menu(GameState *gs);

void draw_screen(GameState *gs);

void draw_line_clear(int full_lines[4]);

#endif // TETRIS_DRAW_H
