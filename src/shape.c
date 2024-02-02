#include "tetris.h"

// 降ってくるブロックのパターン
const t_shape BLOCK_PATTERN[7]= {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4}
};

t_shape generate_new_shape(){
	t_shape new_shape = copy_shape(BLOCK_PATTERN[rand()%7]);
	new_shape.col = rand()%(COLUMN-new_shape.width+1);
	new_shape.row = 0;
	return new_shape;
}

t_shape copy_shape(t_shape shape){
	t_shape new_shape = shape;
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
// void delete_shape(t_shape *shape){
void free_shape_layout(t_shape *shape){
    int i;
    for(i = 0; i < shape->width; i++){
		free(shape->layout[i]);
		shape->layout[i] = NULL;
    }
    free(shape->layout);
		shape->layout = NULL;
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
	free_shape_layout(&temp);
}