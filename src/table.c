#include "tetris.h"

// 現在のブロック位置を画面上に反映する
void update_screen(t_shape shape){
	char active_shape_position[ROW][COLUMN] = {0};
	// 動いているブロックの位置をGridに格納
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				active_shape_position[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
	clear();
	printw("         42 Tetris\n");
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printw("%c ", (Table[i][j] + active_shape_position[i][j])? BLOCK: EMPTY);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", score);
}

// 操作中のブロックの位置を更新する
void set_active_shape_position(t_shape shape){
	char active_shape_position[ROW][COLUMN] = {0};
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				active_shape_position[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

// n列目のブロックをTableから削除する
void	line_clear(char table[ROW][COLUMN], int n)
{
	int i, j;
	// nより上の列を下にずらす
	for (i = n; i >= 1; i--)
		for (j = 0; j < COLUMN; j++)
			Table[i][j] = Table[i - 1][j];
	// 一番上の列は空にする
	for (j = 0; j < COLUMN; j++)
		Table[0][j] = 0;
}

// ブロックがそろった列を消す
int	clear_completed_lines(char table[ROW][COLUMN])
{
	int n, m, completed_line = 0;
	for (n = 0; n < ROW; n++)
		if (is_completed_line(Table[n]))
		{
			completed_line++;
			line_clear(Table, n);
			update_interval -= decrease--; // 更新頻度を短くする
		}
	return completed_line;
}