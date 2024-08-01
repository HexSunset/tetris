#ifndef TETRIS_H
#define TETRIS_H

#include <string.h>
#include <stdbool.h>

#include "tetrominos.h"

#include "keys.h"

#define GRID_HEIGHT 20
#define GRID_WIDTH 10

#define PIECE_STARTING_X GRID_WIDTH / 2 - 2
// Align at the very top with rotation 0
#define PIECE_STARTING_Y GRID_HEIGHT - 3

#define SHIFT_INTERVAL 6.0 / 60.0
#define SHIFT_DELAY 16.0 / 60.0

#define CLEAR_ANIMATION_STEPS 5
#define CLEAR_ANIMATION_INTERVAL 4.0 / 60.0

// Look at get_gravity to find out how these
// are matched to level numbers.
// Based entirely on NES tetris speeds.
static const float level_gravity[30] = {
	48.0 / 60.0,
	43.0 / 60.0,
	38.0 / 60.0,
	33.0 / 60.0,
	28.0 / 60.0,
	23.0 / 60.0,
	18.0 / 60.0,
	13.0 / 60.0,
	8.0 / 60.0,
	6.0 / 60.0,
	5.0 / 60.0,
	4.0 / 60.0,
	3.0 / 60.0,
	2.0 / 60.0,
	1.0 / 60.0
};

typedef struct {
	BlockType blocks[GRID_HEIGHT][GRID_WIDTH];
} Grid;

typedef struct {
	BlockType piece_type;
	int rotation;
} Piece;

#define PAUSE_MENU_LINES 3
static const char* pause_menu_options[] = {
	"RESUME",
	"CONTROLS",
	"QUIT",
};

typedef enum {
	SC_START_SCREEN,
	SC_GAME,
	SC_CONTROLS_MENU,
	SC_PAUSED,
	SC_GAME_OVER,
} Scene;

typedef struct {
	bool active;
	int step;
	float step_time;
} ClearAnimation;

typedef struct {
	// TODO: Previous scene variable?
	//       because there are gonna be multiple
	//       ways to get to the controls menu
	Scene scene;

	bool show_fps;

	size_t pause_menu_line;
	size_t controls_menu_line;
	bool select_new_key;

	int start_level;
	int level;
	int lines_level;
	int lines;

	int score;

	int piece_x;
	int piece_y;

	bool  soft_drop; // stronger gravity when holding down
	float time_since_drop;

	bool  shift_active;
	int   dir_last_update;
	float dir_time_held;

	// indexes of the full lines
	int full_lines[4];
	int full_line_count;

	ClearAnimation clear_anim;

	Piece next;
	Piece piece;

	Keymap keys;

	Grid grid;
} GameState;

void init_gamestate(GameState *gs);

void reset_gamestate(GameState *gs);

void next_piece(GameState *gs);

float get_gravity(GameState *gs);

int next_rotation(int rotation);

int previous_rotation(int rotation);

bool time_to_drop(GameState *gs);

Piece get_piece(BlockType piece_type, int rotation);

Piece get_random_piece();

Piece clone_piece(Piece piece);

const Shape* get_shape(Piece piece);

void place_block(Grid *grid, BlockType block, int x, int y);

void move_left(GameState *gs);

void move_right(GameState *gs);

bool can_place_piece(Grid *grid, Piece piece, int x, int y);

bool can_place(GameState *gs, int x_offset, int y_offset);

void place_piece(Grid *grid, Piece piece, int x, int y);

bool line_is_full(Grid *grid, int y);

bool line_is_empty(Grid *grid, int y);

void move_line(Grid *grid, int src, int dest);

void drop_lines_down(Grid *grid);

bool can_clear_lines(Grid *grid);

void clear_lines(GameState *gs);

#endif // TETRIS_H
