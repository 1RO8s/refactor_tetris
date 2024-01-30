# include "tetris.h"

char Table[ROW][COLUMN] = {0};
int GameOn = TRUE;
suseconds_t update_interval = 400000;
int decrease = 1000;
struct timeval before_now;
struct timeval now;
int score = 0;

int main() {
  srand(time(0));
  score = 0;
  char ch; // 入力された文字
  initscr();
	gettimeofday(&before_now, NULL);
	timeout(1);
	t_shape current; // 現在操作中のブロック
	// delete_shape(current);
	// t_shape new_shape = generate_new_shape();
	// current = new_shape;
	current = generate_new_shape();
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
						// ブロック位置を確定
						fix_shape_position(current);
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(Table);
						score += 100*completed_line;
						// 新しいブロックを生成
						// delete_shape(current);
						// t_shape new_shape = generate_new_shape();
						// current = new_shape;
						current = generate_new_shape();
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
						// ブロック位置を確定
						fix_shape_position(current);
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(Table);
						score += 100*completed_line;
						// 新しいブロックを生成
						// delete_shape(current);
						// t_shape new_shape = generate_new_shape();
						// current = new_shape;
						current = generate_new_shape();
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
