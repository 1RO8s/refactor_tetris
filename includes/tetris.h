/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetris.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:57:56 by hnagasak          #+#    #+#             */
/*   Updated: 2024/02/01 02:59:31 by hnagasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TETRIS_H
# define TETRIS_H

# include <ncurses.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>

# define ROW 20    // 縦のマス数
# define COLUMN 15 // 横のマス数
# define BLOCK '#' // ブロックを表す文字
# define EMPTY '.' // ブロックがないことを表す文字
# define TRUE 1
# define FALSE 0

typedef struct s_shape
{
	char			**layout;
	int				width;
	int				row;
	int				col;
}					t_shape;

typedef struct s_game
{
	char			Table[ROW][COLUMN];
	char 			block_position[ROW][COLUMN];
	int				is_playing;
	suseconds_t		update_interval;
	int				decrease;
	struct timeval	before_now;
	struct timeval	now;
	int				score;
	t_shape			current;
}					t_game;

// extern char Table[ROW][COLUMN];     // 現在の盤面を表す
// extern struct timeval before_now;   // 前回の画面更新時間
// extern struct timeval now;          // 現在の時間
// extern suseconds_t update_interval; // 画面更新間隔
// extern int decrease;                // 画面更新間隔の減少量
// extern int score;                   // スコア

// shape.c
t_shape				generate_new_shape(void);
t_shape				copy_shape(t_shape shape);
void				delete_shape(t_shape shape);
void				rotate_shape(t_shape shape);
// table.c
void				update_screen(t_game *game);
void				set_active_block_position(t_game *game);
// void				line_clear(char table[ROW][COLUMN], int n);
void				line_clear(t_game *game, int n);
// int			clear_completed_lines(char table[ROW][COLUMN]);
int					clear_completed_lines(t_game *game);
void				fix_shape_position(t_game *game);
// utils.c
void				print_result(t_game *game);
int					is_valid_position(t_game *game, t_shape shape);
int					is_completed_line(char line[COLUMN]);
int					is_updatetime(t_game *game);

// tetris.c
void init_table(char table[ROW][COLUMN]);

#endif