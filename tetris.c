# include "tetris.h"

char Table[ROW][COLUMN] = {0};
int GameOn = TRUE;
suseconds_t update_interval = 400000;
int decrease = 1000;
struct timeval before_now;
struct timeval now;
int score = 0;

t_shape current; // 現在操作中のブロック
// t_shape current; // 現在操作中のブロック

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

// ブロック位置を確定させる
void fix_shape_position(t_shape shape){
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				Table[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

int is_updatetime(){
	return ((suseconds_t)(now.tv_sec*1000000 + now.tv_usec) -((suseconds_t)before_now.tv_sec*1000000 + before_now.tv_usec)) > update_interval;
}



void print_result(){
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printf("%c ", Table[i][j] ? BLOCK: EMPTY);
		}
		printf("\n");
	}
	printf("\nGame over!!!!!!!!!\n");
	printf("\nScore: %d\n", score);
}

int main() {
  srand(time(0));
  score = 0;
  char ch; // 入力された文字
  initscr();
	gettimeofday(&before_now, NULL);
	timeout(1);
	delete_shape(current);
	t_shape new_shape = generate_new_shape();
	current = new_shape;
	// if(!FunctionCP(current)){
	if(!is_valid_position(current)){
		GameOn = FALSE;
	}
	// set_active_shape_position(current);
  update_screen(current);
	while(GameOn){
		if ((ch = getch()) != ERR) {
			t_shape temp = copy_shape(current);
			switch(ch){
				case 's':
					temp.row++;
					if(is_valid_position(temp))
						current.row++;
					else {
						// ブロック位置の情報をTableに書き込む
						fix_shape_position(current);
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(Table);
						score += 100*completed_line;
						// 新しいブロックを生成
						t_shape new_shape = generate_new_shape();
						delete_shape(current);
						current = new_shape;
						if(!is_valid_position(current)){
							GameOn = FALSE;
						}
					}
					break;
				case 'd':
					temp.col++;
					if(is_valid_position(temp))
						current.col++;
					break;
				case 'a':
					temp.col--;
					if(is_valid_position(temp))
						current.col--;
					break;
				case 'w':
					rotate_shape(temp);
					if(is_valid_position(temp))
						rotate_shape(current);
					break;
			}
			delete_shape(temp);
			// set_active_shape_position(current);
			update_screen(current);
		}
		gettimeofday(&now, NULL);
		// 一定時間ごとにブロックを下に移動
		if (is_updatetime()) {
			t_shape temp = copy_shape(current);
					temp.row++;
					if(is_valid_position(temp))
						current.row++;
					else {
						// ブロック位置の情報をTableに書き込む
						fix_shape_position(current);
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(Table);
						score += 100*completed_line;
						// 新しいブロックを生成
						t_shape new_shape = generate_new_shape();
						delete_shape(current);
						current = new_shape;
						if(!is_valid_position(current)){
							GameOn = FALSE;
						}
					}
			delete_shape(temp);
			// set_active_shape_position(current);
			update_screen(current);
			gettimeofday(&before_now, NULL);
		}
	}
	delete_shape(current);
	endwin();
	print_result();
  return 0;
}
