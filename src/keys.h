#ifndef KEYS_H
#define KEYS_H

#include <raylib.h>

// Possible actions in the game
typedef enum {
	ACTION_MOVE_LEFT,
	ACTION_MOVE_RIGHT,
	ACTION_ROTATE_FORWARD,
	ACTION_ROTATE_BACKWARD,
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

static const char* action_names[] = {
	"MOVE LEFT",
	"MOVE RIGHT",
	"ROTATE FORWARD",
	"ROTATE BACKWARD",
	"SOFT DROP",
	"PAUSE",
	"RESTART",
	"QUIT",
	"MENU UP",
	"MENU DOWN",
	"MENU SELECT",
	"MENU BACK",
	"TOGGLE FPS",
};

typedef KeyboardKey Keymap[ACTION_COUNT];

void initialize_default_keys(Keymap *keys);

const char* key_to_str(KeyboardKey key);

#endif //KEYS_H
