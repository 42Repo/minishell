# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 17:42:16 by asuc              #+#    #+#              #
#    Updated: 2024/02/05 19:02:08 by mbuchs           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGreen		=	$(shell echo "\033[1;32m")
RESET		=	$(shell echo "\033[0m")
BRed		=	$(shell echo "\033[1;31m")
BCyan		=	$(shell echo "\033[1;36m")
Green		=	$(shell echo "\033[0;32m")
NAME		=	minishell
COMP		=	clang
CFLAGS		=	-Wall -Werror -Wextra
libft		=	Libft/
SRC			=	srcs/ft_cd.c \
				srcs/deco.c \
				srcs/utils.c \
				srcs/get_cmd_prompt.c \
				srcs/debug_main.c
# srcs/lexer/lexer.c
# srcs/minishell.c

OBJ = $(SRC:.c=.o)

TOTAL = $(words $(OBJ))
CURR = 0

define update_progress
  $(eval CURR=$(shell echo $$(($(CURR)+1))))
  @echo -n "\033[1A\033[K"
  @echo -n "$(Green)[`echo "scale=2; $(CURR)/$(TOTAL)*100" | bc`%] Compiling $< \n$(RESET)"
endef

all : $(NAME)

%.o : %.c
	@if [ $(CURR) -eq 0 ]; then \
		echo "\n$(BCyan)Compiling object files for $(NAME)...$(RESET)\n"; \
	fi
	@$(COMP) -gdwarf-4 -fPIE $(CFLAGS) -o $@ -c $<
	@$(B)
	@$(call update_progress)

start :
	@echo "$(BCyan)Compilation Start$(NAME)$(RESET)\n\n"

$(NAME) : $(OBJ)
	@make --quiet --no-print-directory -C $(libft)
	@cp $(libft)libft.a libft.a
	@clang -gdwarf-4 -fPIE $(CFLAGS) -o $(NAME) $(OBJ) libft.a -lreadline
	@echo "\n$(BGreen)Compilation Final $(NAME)$(RESET)"

clean :
	@make clean --quiet --no-print-directory -C $(libft)
	@rm -f $(OBJ)
	@echo "$(BRed)Erase all .o files$(RESET)"

fclean : clean
	@make fclean --quiet --no-print-directory -C $(libft)
	@rm -f $(NAME) libft.a
	@echo "$(BRed)Erase $(NAME), libft.a$(RESET)"

test : all
	@valgrind --error-limit=no --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=suppressions.supp ./$(NAME)

re : fclean all

.PHONY: all fclean clean re


