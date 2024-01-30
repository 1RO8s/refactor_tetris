#include "tetris.h"

void print_result(){
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printf("%c ", Table[i][j] ? BLOCK: EMPTY);
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", score);
}

// ブロック位置が有効範囲内かを判定する
int is_valid_position(t_shape shape){
	char **layout = shape.layout;
	int i, j;
	for(i = 0; i < shape.width;i++) {
		for(j = 0; j < shape.width ;j++){
			// 画面外に出たらFALSE
			if((shape.col+j < 0 || shape.col+j >= COLUMN || shape.row+i >= ROW)){
				if(layout[i][j])
					return FALSE;
				
			} // 他のブロックと重なったらFALSE
			else if(Table[shape.row+i][shape.col+j] && layout[i][j])
				return FALSE;
		}
	}
	return TRUE;
}

// 引数で渡された列がそろっているかを判定する
int is_completed_line(char line[COLUMN]) {
	int i;
	for (i = 0; i < COLUMN; i++) {
		if (line[i] == 0) {
			return FALSE;
		}
	}
	return TRUE;
}

int is_updatetime(){
	return ((suseconds_t)(now.tv_sec*1000000 + now.tv_usec) -((suseconds_t)before_now.tv_sec*1000000 + before_now.tv_usec)) > update_interval;
}