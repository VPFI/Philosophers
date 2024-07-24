# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/23 13:59:42 by vperez-f          #+#    #+#              #
#    Updated: 2024/07/16 13:57:52 by vperez-f         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

END=\033[0m

NAME = philo

CFILES = philo.c

OFILES = $(CFILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_DIR),$(OFILES))

OBJ_DIR = obj/

OBJ_PATH = obj

CFLAGS = -Wall -Wextra -Werror -fsanitize=address -fsanitize=leak -O3

CC = cc

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@printf "\n$(NAME) COMPILED!\n$(END)"

$(OBJ_DIR)%.o: src/%.c philo.h Makefile
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)

fclean:	clean
	@$(RM) $(NAME)
	@rm -rf $(OBJ_PATH)

re:	fclean all

.PHONY: all clean fclean re