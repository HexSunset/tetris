#ifndef KEYS_H
#define KEYS_H

#include <raylib.h>

#define ACTION_COUNT 12
// Possible actions in the game
typedef enum {
	ACTION_MOVE_LEFT = 0,
	ACTION_MOVE_RIGHT = 1,
	ACTION_ROTATE_CLOCKWISE = 2,
	ACTION_ROTATE_COUNTERCLOCKWISE = 3,
	ACTION_SOFT_DROP = 4,
	ACTION_PAUSE = 5,
	ACTION_RESTART = 6,
	ACTION_QUIT = 7,
	ACTION_MENU_UP = 8,
	ACTION_MENU_DOWN = 9,
	ACTION_MENU_SELECT = 10,
	ACTION_TOGGLE_FPS = 11,
} Action;

typedef KeyboardKey Keymap[ACTION_COUNT];

void initialize_default_keys(Keymap *keys);

#endif //KEYS_H
