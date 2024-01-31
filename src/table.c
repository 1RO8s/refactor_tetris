#include "tetris.h"

void init_table(char table[ROW][COLUMN]){
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			table[i][j] = 0;
		}
	}
}

// 操作中のブロックの位置をGridに記録する
void set_active_block_position(t_game *game){
	t_shape shape = game->current;
	init_table(game->active_block_position);
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				game->active_block_position[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

// n列目のブロックをTableから削除する
void	line_clear(t_game *game, int n)
{
	int i, j;
	// nより上の列を下にずらす
	for (i = n; i >= 1; i--)
		for (j = 0; j < COLUMN; j++)
			game->locked_block_position[i][j] = game->locked_block_position[i - 1][j];
	// 一番上の列は空にする
	for (j = 0; j < COLUMN; j++)
		game->locked_block_position[0][j] = 0;
}

// ブロックがそろった列を消す
int	clear_completed_lines(t_game *game)
{
	int n, m, completed_line = 0;
	for (n = 0; n < ROW; n++)
		if (is_completed_line(game->locked_block_position[n]))
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
				game->locked_block_position[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

