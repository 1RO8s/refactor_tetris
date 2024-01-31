#include "tetris.h"

// 操作中のブロック位置を画面上に反映する
void update_screen(t_game *game){
	clear();
	printw("         42 Tetris\n");
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printw("%c ", (game->Table[i][j] + game->block_position[i][j])? BLOCK: EMPTY);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->score);
}

// 操作中のブロックの位置をGridに記録する
void set_active_block_position(t_game *game){
	t_shape shape = game->current;
	init_table(game->block_position);
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				game->block_position[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

// n列目のブロックをTableから削除する
// void	line_clear(char table[ROW][COLUMN], int n)
void	line_clear(t_game *game, int n)
{
	int i, j;
	// nより上の列を下にずらす
	for (i = n; i >= 1; i--)
		for (j = 0; j < COLUMN; j++)
			game->Table[i][j] = game->Table[i - 1][j];
	// 一番上の列は空にする
	for (j = 0; j < COLUMN; j++)
		game->Table[0][j] = 0;
}

// ブロックがそろった列を消す
int	clear_completed_lines(t_game *game)
{
	int n, m, completed_line = 0;
	for (n = 0; n < ROW; n++)
		if (is_completed_line(game->Table[n]))
		{
			completed_line++;
			line_clear(game, n);
			game->update_interval -= game->decrease--; // 更新頻度を短くする
		}
	game->score += 100*completed_line;
	return completed_line;
}

// ブロック位置を確定させる
void fix_shape_position(t_game *game){
	t_shape shape = game->current;
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				game->Table[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

void init_game(t_game *game){
	init_table(game->Table);
	init_table(game->block_position);
	game->is_playing = TRUE;
	game->update_interval = 400000;
	game->decrease = 1000;
	gettimeofday(&(game->before_now), NULL);
	gettimeofday(&(game->now), NULL);
	game->score = 0;
	game->current = generate_new_shape();
}