CC		:= gcc
# CFLAGS	:= -Wall -Wextra -Werror
NAME	:= tetris
RM		:= rm -f
SRCS	:= tetris.c src/shape.c src/table.c src/utils.c
OBJS	:= $(SRCS:.c=.o)
# SRCS_DIR	:= srcs
INC_DIR	:= includes
INC := -I$(INC_DIR)
LIB_NCURSES := -lncurses

all:	$(NAME)
$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(INC) $(OBJS) -o $(NAME) $(LIB_NCURSES)

%.o: %.c
	$(CC) ${CFLAGS} ${INC} -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:		fclean all

.PHONY: all clean fclean re