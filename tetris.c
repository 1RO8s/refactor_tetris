#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define ROW 20 // 縦のマス数
#define COLUMN 15 // 横のマス数
#define TRUE 1
#define FALSE 0

#define BLOCK '*'
#define EMPTY '.'

char Table[ROW][COLUMN] = {0}; // 現在の盤面を表す
int score = 0;
int GameOn = TRUE;
// suseconds_t timer = 400000;
suseconds_t update_interval = 400000;
int decrease = 1000;

typedef struct s_shape {
		char **layout;
    int width, row, col;
} t_shape;

t_shape current;

// const t_shape StructsArray[7]= {
const t_shape BLOCK_PATTERN[7]= {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};
// const t_shape BLOCK_PATTERN[7]= {
// 	{{{0,1,1},{1,1,0},{0,0,0}}, 3},
// 	{{{1,1,0},{0,1,1},{0,0,0}}, 3},
// 	{{{0,1,0},{1,1,1},{0,0,0}}, 3},
// 	{{{0,0,1},{1,1,1},{0,0,0}}, 3},
// 	{{{1,0,0},{1,1,1},{0,0,0}}, 3},
// 	{{{1,1},{1,1}}, 2},
// 	{{{0,0,0,0},{1,1,1,1}, {0,0,0,0}}, 4},
// };

t_shape copy_shape(t_shape shape){
	t_shape new_shape = shape;
	// char **copyshape = shape.layout;
	new_shape.layout = (char**)malloc(new_shape.width*sizeof(char*));
    int i, j;
    for(i = 0; i < new_shape.width; i++){
		new_shape.layout[i] = (char*)malloc(new_shape.width*sizeof(char));
		for(j=0; j < new_shape.width; j++) {
			new_shape.layout[i][j] = shape.layout[i][j];
		}
    }
    return new_shape;
}

void delete_shape(t_shape shape){
    int i;
    for(i = 0; i < shape.width; i++){
		free(shape.layout[i]);
    }
    free(shape.layout);
}

// int FunctionCP(t_shape shape){
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

void rotate_shape(t_shape shape){
	t_shape temp = copy_shape(shape);
	int i, j, k, width;
	width = shape.width;
	for(i = 0; i < width ; i++){
		for(j = 0, k = width-1; j < width ; j++, k--){
				shape.layout[i][j] = temp.layout[k][i];
		}
	}
	delete_shape(temp);
}

// 現在のブロック位置を画面上に反映する
void update_screen(t_shape shape){
	// ブロック位置の情報をバッファに書き込む
	char Buffer[ROW][COLUMN] = {0};
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				Buffer[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
	clear();
	printw("         42 Tetris\n");
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printw("%c ", (Table[i][j] + Buffer[i][j])? BLOCK: EMPTY);
			// printw("%c ", Table[i][j]? BLOCK: EMPTY);
		}
		printw("\n");
	}
	printw("\nScore: %d\n", score);
}

int is_completed_line(char line[COLUMN]) {
	int i;
	for (i = 0; i < COLUMN; i++) {
		if (line[i] == 0) {
			return FALSE;
		}
	}
	return TRUE;
}

// n行目を消す
void	line_clear(char table[ROW][COLUMN], int n)
{
	int i, j;
	// nより上の行を下にずらす
	for (i = n; i >= 1; i--)
		for (j = 0; j < COLUMN; j++)
			Table[i][j] = Table[i - 1][j];
	// 一番上の行は空行にする
	for (j = 0; j < COLUMN; j++)
		Table[0][j] = 0;
}

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

void set_shape_position(t_shape shape){
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.layout[i][j])
				Table[shape.row+i][shape.col+j] = shape.layout[i][j];
		}
	}
}

struct timeval before_now, now;

int is_updatetime(){
	return ((suseconds_t)(now.tv_sec*1000000 + now.tv_usec) -((suseconds_t)before_now.tv_sec*1000000 + before_now.tv_usec)) > update_interval;
}

t_shape generate_new_shape(){
	t_shape new_shape = copy_shape(BLOCK_PATTERN[rand()%7]);
	new_shape.col = rand()%(COLUMN-new_shape.width+1);
	new_shape.row = 0;
	// delete_shape(current);
	// current = new_shape;
	// if(!FunctionCP(current)){
	// 	GameOn = FALSE;
	// }
	return new_shape;
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
	t_shape new_shape = generate_new_shape();
  delete_shape(current);
	current = new_shape;
	// if(!FunctionCP(current)){
	if(!is_valid_position(current)){
		GameOn = FALSE;
	}
	// set_shape_position(current);
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
						set_shape_position(current);
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
			// set_shape_position(current);
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
						set_shape_position(current);
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
			// set_shape_position(current);
			update_screen(current);
			gettimeofday(&before_now, NULL);
		}
	}
	delete_shape(current);
	endwin();
	print_result();
  return 0;
}
