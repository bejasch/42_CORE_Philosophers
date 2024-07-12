# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/07/10 16:41:26 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
HEADER = header/philo.h
SRC_DIR = src
OBJ_DIR = obj
SOURCES = check_routines.c init_free.c philosophers.c utils.c main.c
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))
FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	cc $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS) $(HEADER)
	cc $(FLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all bonus clean fclean re
