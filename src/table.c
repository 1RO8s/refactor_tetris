#include "tetris.h"

// 現在のブロック位置を画面上に反映する
void update_screen(t_game *game){
	// t_shape shape = game->current;
	// char active_shape_position[ROW][COLUMN] = {0};
	// // 動いているブロックの位置をGridに格納
	int i, j;
	// for(i = 0; i < shape.width ;i++){
	// 	for(j = 0; j < shape.width ; j++){
	// 		if(shape.layout[i][j])
	// 			active_shape_position[shape.row+i][shape.col+j] = shape.layout[i][j];
	// 	}
	// }
	// set_active_block_position(game);
	clear();
	printw("         42 Tetris\n");
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printw("%c ", (game->Table[i][j] + game->block_position[i][j])? BLOCK: EMPTY);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", game->score);
}

// 操作中のブロックの位置を更新する
void set_active_block_position(t_game *game){
	// char active_shape_position[ROW][COLUMN] = {0};
	// int i, j;
	// for(i = 0; i < shape.width ;i++){
	// 	for(j = 0; j < shape.width ; j++){
	// 		if(shape.layout[i][j])
	// 			active_shape_position[shape.row+i][shape.col+j] = shape.layout[i][j];
	// 	}
	// }
	t_shape shape = game->current;
	// char active_shape_position[ROW][COLUMN] = {0};
	init_table(game->block_position);
	// 動いているブロックの位置をGridに格納
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
	// char Table[ROW][COLUMN] = game->Table;
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
	// char Table[ROW][COLUMN] = game->Table;
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