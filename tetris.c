# include "tetris.h"

// char Table[ROW][COLUMN] = {0};
// int GameOn = TRUE;
// suseconds_t update_interval = 400000;
// int decrease = 1000;
// struct timeval before_now;
// struct timeval now;
// int score = 0;


void init_game(t_game *game){
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			game->Table[i][j] = 0;
		}
	}
	game->is_playing = TRUE;
	game->update_interval = 400000;
	game->decrease = 1000;
	gettimeofday(&(game->before_now), NULL);
	gettimeofday(&(game->now), NULL);
	game->score = 0;
	game->current = generate_new_shape();
}

int main() {
	t_game game;
	init_game(&game);
  srand(time(0));
  char ch; // 入力された文字
  initscr();
	timeout(1);
	// t_shape current; // 現在操作中のブロック
	// delete_shape(current);
	// t_shape new_shape = generate_new_shape();
	// current = new_shape;
	// current = generate_new_shape();
	if(!is_valid_position(&game,game.current)){
		game.is_playing = FALSE;
	}
	// set_active_shape_position(current);
  update_screen(&game);
	while(game.is_playing){
		if ((ch = getch()) != ERR) {
			t_shape temp = copy_shape(game.current);
			switch(ch){
				case 's':
					temp.row++;
					if(is_valid_position(&game,temp))
						game.current.row++;
					else {
						// ブロック位置を確定
						fix_shape_position(&game);
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(&game);
						game.score += 100*completed_line;
						// 新しいブロックを生成
						// delete_shape(current);
						// t_shape new_shape = generate_new_shape();
						// current = new_shape;
						game.current = generate_new_shape();
						if(!is_valid_position(&game,game.current)){
							game.is_playing = FALSE;
						}
					}
					break;
				case 'd':
					temp.col++;
					if(is_valid_position(&game,temp))
						game.current.col++;
					break;
				case 'a':
					temp.col--;
					if(is_valid_position(&game,temp))
						game.current.col--;
					break;
				case 'w':
					rotate_shape(temp);
					if(is_valid_position(&game,temp))
						rotate_shape(game.current);
					break;
			}
			delete_shape(temp);
			// set_active_shape_position(current);
			update_screen(&game);
		}
		gettimeofday(&(game.now), NULL);
		// 一定時間ごとにブロックを下に移動
		if (is_updatetime(&game)) {
			t_shape temp = copy_shape(game.current);
					temp.row++;
					if(is_valid_position(&game,temp))
						game.current.row++;
					else {
						// ブロック位置を確定
						fix_shape_position(&game);
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(&game);
						game.score += 100*completed_line;
						// 新しいブロックを生成
						// delete_shape(current);
						// t_shape new_shape = generate_new_shape();
						// current = new_shape;
						game.current = generate_new_shape();
						if(!is_valid_position(&game,game.current)){
							game.is_playing = FALSE;
						}
					}
			delete_shape(temp);
			// set_active_shape_position(current);
			update_screen(&game);
			gettimeofday(&(game.before_now), NULL);
		}
	}
	delete_shape(game.current);
	endwin();
	print_result(&game);
  return 0;
}
