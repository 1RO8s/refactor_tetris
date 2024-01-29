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
char GameOn = TRUE;
suseconds_t timer = 400000;
int decrease = 1000;

typedef struct s_shape {
    char **array;
    int width, row, col;
} t_shape;

t_shape current;

const t_shape StructsArray[7]= {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};

t_shape copy_shape(t_shape shape){
	t_shape new_shape = shape;
	char **copyshape = shape.array;
	new_shape.array = (char**)malloc(new_shape.width*sizeof(char*));
    int i, j;
    for(i = 0; i < new_shape.width; i++){
		new_shape.array[i] = (char*)malloc(new_shape.width*sizeof(char));
		for(j=0; j < new_shape.width; j++) {
			new_shape.array[i][j] = copyshape[i][j];
		}
    }
    return new_shape;
}

void delete_shape(t_shape shape){
    int i;
    for(i = 0; i < shape.width; i++){
		free(shape.array[i]);
    }
    free(shape.array);
}

int FunctionCP(t_shape shape){
	char **array = shape.array;
	int i, j;
	for(i = 0; i < shape.width;i++) {
		for(j = 0; j < shape.width ;j++){
			if((shape.col+j < 0 || shape.col+j >= COLUMN || shape.row+i >= ROW)){
				if(array[i][j])
					return FALSE;
				
			}
			else if(Table[shape.row+i][shape.col+j] && array[i][j])
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
				shape.array[i][j] = temp.array[k][i];
		}
	}
	delete_shape(temp);
}

// 現在のブロック位置を画面上に反映する
void update_screen(){
	// ブロック位置の情報をバッファに書き込む
	char Buffer[ROW][COLUMN] = {0};
	int i, j;
	for(i = 0; i < current.width ;i++){
		for(j = 0; j < current.width ; j++){
			if(current.array[i][j])
				Buffer[current.row+i][current.col+j] = current.array[i][j];
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
	{
		if (is_completed_line(Table[n]))
		{
			completed_line++;
			line_clear(Table, n);
			timer -= decrease--; // 更新頻度を短くする
		}
	}
	return completed_line;
}

void set_block_position(t_shape shape){
	int i, j;
	for(i = 0; i < shape.width ;i++){
		for(j = 0; j < shape.width ; j++){
			if(shape.array[i][j])
				Table[shape.row+i][shape.col+j] = shape.array[i][j];
		}
	}
}

struct timeval before_now, now;

// int hasToUpdate(){
int is_updatetime(){
	return ((suseconds_t)(now.tv_sec*1000000 + now.tv_usec) -((suseconds_t)before_now.tv_sec*1000000 + before_now.tv_usec)) > timer;
}

int main() {
  srand(time(0));
  score = 0;
  char ch; // 入力された文字
  initscr();
	gettimeofday(&before_now, NULL);
	timeout(1);
	t_shape new_shape = copy_shape(StructsArray[rand()%7]);
  new_shape.col = rand()%(COLUMN-new_shape.width+1);
  new_shape.row = 0;
  delete_shape(current);
	current = new_shape;
	if(!FunctionCP(current)){
		GameOn = FALSE;
	}
	// set_block_position(current);
  update_screen();
	while(GameOn){
		if ((ch = getch()) != ERR) {
			t_shape temp = copy_shape(current);
			switch(ch){
				case 's':
					temp.row++;  //move down
					if(FunctionCP(temp))
						current.row++;
					else {
						// ブロック位置の情報をTableに書き込む
						int i, j;
						for(i = 0; i < current.width ;i++){
							for(j = 0; j < current.width ; j++){
								if(current.array[i][j])
									Table[current.row+i][current.col+j] = current.array[i][j];
							}
						}
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(Table);
						score += 100*completed_line;
						// 新しいブロックを生成
						t_shape new_shape = copy_shape(StructsArray[rand()%7]);
						new_shape.col = rand()%(COLUMN-new_shape.width+1);
						new_shape.row = 0;
						delete_shape(current);
						current = new_shape;
						if(!FunctionCP(current)){
							GameOn = FALSE;
						}
					}
					break;
				case 'd':
					temp.col++;
					if(FunctionCP(temp))
						current.col++;
					break;
				case 'a':
					temp.col--;
					if(FunctionCP(temp))
						current.col--;
					break;
				case 'w':
					rotate_shape(temp);
					if(FunctionCP(temp))
						rotate_shape(current);
					break;
			}
			delete_shape(temp);
			// set_block_position(current);
			update_screen();
		}
		gettimeofday(&now, NULL);
		// 一定時間ごとにブロックを下に移動
		if (is_updatetime()) {
			t_shape temp = copy_shape(current);
			// switch('s'){
			// 	case 's':
					temp.row++;
					if(FunctionCP(temp))
						current.row++;
					else {
						int i, j;
						for(i = 0; i < current.width ;i++){
							for(j = 0; j < current.width ; j++){
								if(current.array[i][j])
									Table[current.row+i][current.col+j] = current.array[i][j];
							}
						}
						// そろった行を消す
						int completed_line=0;
						completed_line = clear_completed_lines(Table);
						score += 100*completed_line;
						// 新しいブロックを生成
						t_shape new_shape = copy_shape(StructsArray[rand()%7]);
						new_shape.col = rand()%(COLUMN-new_shape.width+1); // 横の位置をランダムに決める
						new_shape.row = 0;
						delete_shape(current);
						current = new_shape;
						if(!FunctionCP(current)){
							GameOn = FALSE;
						}
					}
			// 		break;
			// }
			delete_shape(temp);
			// set_block_position(current);
			update_screen();
			gettimeofday(&before_now, NULL);
		}
	}
	delete_shape(current);
	endwin();
	int i, j;
	for(i = 0; i < ROW ;i++){
		for(j = 0; j < COLUMN ; j++){
			printf("%c ", Table[i][j] ? BLOCK: EMPTY);
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", score);
    return 0;
}
