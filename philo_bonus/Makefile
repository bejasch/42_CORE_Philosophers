# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/07/10 16:28:34 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
HEADER = header/philo_bonus.h
SRC_DIR = src
OBJ_DIR = obj
SOURCES = init_free_bonus.c philosophers_bonus.c utils_bonus.c main_bonus.c\
			check_routines_bonus.c sem_name_bonus.c
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))
FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(NAME): $(OBJECTS) $(HEADER)
	cc $(FLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	cc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all bonus clean fclean re
