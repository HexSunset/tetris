#include <raylib.h>
#include "keys.h"

void initialize_default_keys(Keymap *keys) {
	(*keys)[ACTION_MOVE_LEFT] = KEY_LEFT;
	(*keys)[ACTION_MOVE_RIGHT] = KEY_RIGHT;
	(*keys)[ACTION_ROTATE_CLOCKWISE] = KEY_X;
	(*keys)[ACTION_ROTATE_COUNTERCLOCKWISE] = KEY_Z;
	(*keys)[ACTION_SOFT_DROP] = KEY_DOWN;
	(*keys)[ACTION_PAUSE] = KEY_SPACE;
	(*keys)[ACTION_RESTART] = KEY_R;
	(*keys)[ACTION_QUIT] = KEY_Q;
	(*keys)[ACTION_MENU_UP] = KEY_UP;
	(*keys)[ACTION_MENU_DOWN] = KEY_DOWN;
	(*keys)[ACTION_MENU_SELECT] = KEY_X;
	(*keys)[ACTION_TOGGLE_FPS] = KEY_F;
}
