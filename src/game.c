#include "tetris.h"

void init_game(t_game *game){
	init_table(game->locked_block_position);
	init_table(game->active_block_position);
	game->is_playing = TRUE;
	game->update_interval = 400000;
	game->decrease = 1000;
	gettimeofday(&(game->before_now), NULL);
	gettimeofday(&(game->now), NULL);
	game->score = 0;
	game->current = generate_new_shape();
}

void operate_active_block(t_game *game){
	char ch; // 入力された文字
	if ((ch = getch()) != ERR) {
	t_shape temp = copy_shape(game->current);
	switch(ch){
		case 's':
			temp.row++;
			if(is_valid_position(game,temp))
				game->current.row++;
			else {
				fix_shape_position(game);
				clear_completed_lines(game);
				game->current = generate_new_shape();
				if(!is_valid_position(game,game->current)){
					game->is_playing = FALSE;
				}
			}
			break;
		case 'd':
			temp.col++;
			if(is_valid_position(game,temp))
				game->current.col++;
			break;
		case 'a':
			temp.col--;
			if(is_valid_position(game,temp))
				game->current.col--;
			break;
		case 'w':
			rotate_shape(temp);
			if(is_valid_position(game,temp))
				rotate_shape(game->current);
			break;
	}
	delete_shape(temp);
	set_active_block_position(game);
	update_screen(game);
	}
}

void falling_active_block(t_game *game){
	gettimeofday(&(game->now), NULL);
	// 一定時間ごとにブロックを下に移動
	if (is_updatetime(game)) {
		t_shape temp = copy_shape(game->current);
				temp.row++;
				if(is_valid_position(game,temp))
					game->current.row++;
				else {
					fix_shape_position(game);
					clear_completed_lines(game);
					game->current = generate_new_shape();
					if(!is_valid_position(game,game->current)){
						game->is_playing = FALSE;
					}
				}
		delete_shape(temp);
		set_active_block_position(game);
		update_screen(game);
		gettimeofday(&(game->before_now), NULL);
	}
}

// 操作中のブロック位置を画面上に反映する
void update_screen(t_game *game){
	clear();
	printw("         42 Tetris\n");
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printw("%c ", (game->locked_block_position[i][j] + game->active_block_position[i][j])? BLOCK: EMPTY);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->score);
}