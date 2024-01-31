# refactor_tetris

### 実行方法
```sh
refactor_tetris % make
gcc  -Iincludes -c tetris.c -o tetris.o
gcc  -Iincludes -c src/shape.c -o src/shape.o
gcc  -Iincludes -c src/table.c -o src/table.o
gcc  -Iincludes -c src/utils.c -o src/utils.o
gcc  -Iincludes -c src/game.c -o src/game.o
gcc  -Iincludes tetris.o src/shape.o src/table.o src/utils.o src/game.o -o tetris -lncurses
refactor_tetris % ./tetris 
```