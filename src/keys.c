#include <raylib.h>
#include "keys.h"

void initialize_default_keys(Keymap *keys) {
	(*keys)[ACTION_MOVE_LEFT] = KEY_LEFT;
	(*keys)[ACTION_MOVE_RIGHT] = KEY_RIGHT;
	(*keys)[ACTION_ROTATE_FORWARD] = KEY_X;
	(*keys)[ACTION_ROTATE_BACKWARD] = KEY_Z;
	(*keys)[ACTION_SOFT_DROP] = KEY_DOWN;
	(*keys)[ACTION_PAUSE] = KEY_SPACE;
	(*keys)[ACTION_RESTART] = KEY_R;
	(*keys)[ACTION_QUIT] = KEY_Q;
	(*keys)[ACTION_MENU_UP] = KEY_UP;
	(*keys)[ACTION_MENU_DOWN] = KEY_DOWN;
	(*keys)[ACTION_MENU_SELECT] = KEY_X;
	(*keys)[ACTION_MENU_BACK] = KEY_Z;
	(*keys)[ACTION_TOGGLE_FPS] = KEY_F;
}

const char* key_to_str(KeyboardKey key) {
	switch (key) {
	case KEY_NULL:
		return "?";
	case KEY_APOSTROPHE:
		return "'";
	case KEY_COMMA:
		return ",";
	case KEY_MINUS:
		return "-";
	case KEY_PERIOD:
		return ".";
	case KEY_SLASH:
		return "/";
	case KEY_ZERO:
		return "0";
	case KEY_ONE:
		return "1";
	case KEY_TWO:
		return "2";
	case KEY_THREE:
		return "3";
	case KEY_FOUR:
		return "4";
	case KEY_FIVE:
		return "5";
	case KEY_SIX:
		return "6";
	case KEY_SEVEN:
		return "7";
	case KEY_EIGHT:
		return "8";
	case KEY_NINE:
		return "9";
	case KEY_SEMICOLON:
		return ";";
	case KEY_EQUAL:
		return "=";
	case KEY_A:
		return "A";
	case KEY_B:
		return "B";
	case KEY_C:
		return "C";
	case KEY_D:
		return "D";
	case KEY_E:
		return "E";
	case KEY_F:
		return "F";
	case KEY_G:
		return "G";
	case KEY_H:
		return "H";
	case KEY_I:
		return "I";
	case KEY_J:
		return "J";
	case KEY_K:
		return "K";
	case KEY_L:
		return "L";
	case KEY_M:
		return "M";
	case KEY_N:
		return "N";
	case KEY_O:
		return "O";
	case KEY_P:
		return "P";
	case KEY_Q:
		return "Q";
	case KEY_R:
		return "R";
	case KEY_S:
		return "S";
	case KEY_T:
		return "T";
	case KEY_U:
		return "U";
	case KEY_V:
		return "V";
	case KEY_W:
		return "W";
	case KEY_X:
		return "X";
	case KEY_Y:
		return "Y";
	case KEY_Z:
		return "Z";
	case KEY_BACKSLASH:
		return "\\";
	case KEY_GRAVE:
		return "`";
	case KEY_SPACE:
		return "SPC";
	case KEY_ESCAPE:
		return "ESC";
	case KEY_ENTER:
		return "RET";
	case KEY_TAB:
		return "TAB";
	case KEY_BACKSPACE:
		return "BACK";
	case KEY_RIGHT:
		return "RIGHT";
	case KEY_LEFT:
		return "LEFT";
	case KEY_DOWN:
		return "DOWN";
	case KEY_UP:
		return "UP";
	default:
		return "";
	}
}
