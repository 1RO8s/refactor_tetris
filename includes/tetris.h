/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetris.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:57:56 by hnagasak          #+#    #+#             */
/*   Updated: 2024/01/30 19:55:26 by hnagasak         ###   ########.fr       */
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
# define TRUE 1
# define FALSE 0
# define BLOCK '*'
# define EMPTY '.'

/**
 * @struct s_shape
 * @brief Represents a Tetris piece's shape and position on the game board.
 *
 * This structure is used to store the layout and dimensions of a Tetris piece,
 * as well as its current position on the game board. The layout is represented
 * as a 2D array of characters, where each character can indicate a part of the
 * piece or an empty space. The width denotes the size of the piece's layout,

	* and the row and col fields specify the piece's top-left position on the board.
 *
 * @var char **layout
 * 2D array representing the piece's layout.
 *
 * @var int width
 * The width of the piece's layout (how many columns it occupies).
 *
 * @var int row
 * The row position of the piece's top-left corner on the game board.
 *
 * @var int col
 * The column position of the piece's top-left corner on the game board.
 */
typedef struct s_shape
{
	char	**layout;
	int		width;
	int		row;
	int		col;
}			t_shape;

extern char Table[ROW][COLUMN];     // 現在の盤面を表す
extern struct timeval before_now;   // 前回の画面更新時間
extern struct timeval now;          // 現在の時間
extern suseconds_t update_interval; // 画面更新間隔
extern int decrease;                // 画面更新間隔の減少量
extern int score;                   // スコア

// shape.c
t_shape		generate_new_shape(void);
t_shape		copy_shape(t_shape shape);
void		delete_shape(t_shape shape);
void		rotate_shape(t_shape shape);
// table.c
void		update_screen(t_shape shape);
void		set_active_shape_position(t_shape shape);
void		line_clear(char table[ROW][COLUMN], int n);
int			clear_completed_lines(char table[ROW][COLUMN]);

// utils.c
int			is_completed_line(char line[COLUMN]);

#endif