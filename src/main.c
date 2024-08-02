#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include "draw.h"
#include "tetris.h"
#include "keys.h"

int main() {
	GameState gs;
	init_gamestate(&gs);

	printf("SCREEN_HEIGHT: %d\n", SCREEN_HEIGHT);
	printf("SCREEN_WIDTH: %d\n", SCREEN_WIDTH);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TETRIS");

	SetExitKey(KEY_NULL);

	srand(time(0));

	SetTargetFPS(60);

	while (!WindowShouldClose() && !gs.close_game) {
		handle_keys(&gs);

		if (gs.clear_anim.active && gs.scene == SC_GAME) {
			update_clear_anim(&gs);
		}

		if (can_update_game(&gs))
			update_game(&gs);

		draw_screen(&gs);
	}

	CloseWindow();

	return 0;
}
