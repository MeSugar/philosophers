NAME		=	philo

CFLAGS		=	-Wall -Wextra -Werror

CC			=	gcc

INCLUDES	=	./

SRCS		=	philos/main.c philos/philos.h

all : $(NAME)

$(NAME) : $(SRCS)
		@$(CC) -g -o $(NAME) $(CFLAGS) $(SRCS) -I $(INCLUDES) -pthread

clean :
		@rm -rf *.o

fclean : clean
		@rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re