#ifndef KEYS_H
#define KEYS_H

#include <raylib.h>

// Possible actions in the game
typedef enum {
	AC_MOVE_LEFT,
	AC_MOVE_RIGHT,
	AC_ROTATE_FORWARD,
	AC_ROTATE_BACKWARD,
	AC_SOFT_DROP,
	AC_PAUSE,
	AC_RESTART,
	AC_QUIT,
	AC_MENU_UP,
	AC_MENU_DOWN,
	AC_MENU_SELECT,
	AC_MENU_BACK,
	AC_TOGGLE_FPS,
	AC_COUNT, // This is the total number of variants in the enum
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

typedef KeyboardKey Keymap[AC_COUNT];

void initialize_default_keys(Keymap *keys);

const char* key_to_str(KeyboardKey key);

#endif //KEYS_H
