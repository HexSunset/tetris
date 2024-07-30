#ifndef KEYS_H
#define KEYS_H

#include <raylib.h>

// Possible actions in the game
typedef enum {
	ACTION_MOVE_LEFT,
	ACTION_MOVE_RIGHT,
	ACTION_ROTATE_CLOCKWISE,
	ACTION_ROTATE_COUNTERCLOCKWISE,
	ACTION_SOFT_DROP,
	ACTION_PAUSE,
	ACTION_RESTART,
	ACTION_QUIT,
	ACTION_MENU_UP,
	ACTION_MENU_DOWN,
	ACTION_MENU_SELECT,
	ACTION_MENU_BACK,
	ACTION_TOGGLE_FPS,
	ACTION_COUNT, // This is the total number of variants in the enum
} Action;

typedef KeyboardKey Keymap[ACTION_COUNT];

void initialize_default_keys(Keymap *keys);

#endif //KEYS_H
