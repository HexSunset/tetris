#ifndef TETRIS_DRAW_H
#define TETRIS_DRAW_H

#include <raylib.h>

#include "tetris.h"

#define GRID_PIXELS 25
#define GRID_BORDER_PIXELS 3

#define BACKGROUND_COLOR (Color) {0x16, 0x16, 0x16, 0xFF}

Color block_color(BlockType bt);

void  draw_block(int x, int y, Color color);

void  draw_grid(Grid *grid);

void draw_piece(Piece piece, int x, int y);

#endif // TETRIS_DRAW_H
