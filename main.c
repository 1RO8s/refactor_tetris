#include "tetris.h"

int	main(void)
{
	t_game	game;

	init_game(&game);
	srand(time(0));
	initscr();
	timeout(1);
	if (!is_valid_position(&game, game.current))
		game.is_playing = FALSE;
	set_active_block_position(&game);
	update_screen(&game);
	while (game.is_playing){
		control_active_block(&game);
		falling_active_block(&game);
	}
	free_shape_layout(&(game.current));
	endwin();
	print_result(&game);
	return (0);
}
