NAME		=	philo

CFLAGS		=	-Wall -Wextra -Werror

CC			=	gcc

INCLUDES	=	philos/includes

HEADER 		= 	philos/includes/philos.h

SRCS		=	philos/main.c

all : $(NAME)

$(NAME) : $(SRCS) $(HEADER)
		@$(CC) $(SRCS) -o $(NAME) -I $(INCLUDES) -pthread

clean :
		@rm -rf *.o

fclean : clean
		@rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re