/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetris.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagasak <hnagasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:57:56 by hnagasak          #+#    #+#             */
/*   Updated: 2024/01/30 18:40:28 by hnagasak         ###   ########.fr       */
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
	char	**layout; /**< Pointer to a 2D array containing the layout of the Tetris piece. */
	int		width; /**< Width of the Tetris piece in terms of number of columns. */
	int		row;
	/** Row position of the Tetris piece's top-left corner on the game board. */
	int		col;
	/** Column position of the Tetris piece's top-left corner on the game board. */
}			t_shape;

#endif