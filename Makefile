# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 17:42:16 by asuc              #+#    #+#              #
#    Updated: 2024/02/10 17:43:25 by asuc             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGreen		=	$(shell echo -n "\033[1;32m")
RESET		=	$(shell echo -n "\033[0m")
BRed		=	$(shell echo -n "\033[1;31m")
BCyan		=	$(shell echo -n "\033[1;36m")
Green		=	$(shell echo -n "\033[0;32m")
Blue 		=	$(shell echo -n "\033[0;34m")
Bblue		=	$(shell echo -n "\033[1;34m")
NAME		=	minishell
COMP		=	clang
CFLAGS		=	-Wall -Werror -Wextra
LIBFT_DIR	=	$(shell pwd)/Libft/
LIBFT		=	$(LIBFT_DIR)libft.a
CACHE_DIR	:=	$(addprefix $(shell pwd)/, .cache)
MAKE		=	make --quiet --no-print-directory
AUTHORS		=	asuc & mbuchs
DEBUG		=	0
# SRC			=	srcs/ft_cd.c \
# 				srcs/deco.c \
# 				srcs/utils.c \
# 				srcs/get_cmd_prompt.c \
# 				srcs/ft_export.c \
# 				srcs/ft_unset.c \
# 				srcs/get_env.c\
# 				srcs/lexer/lexer.c\
# 				srcs/ft_echo.c\
# 				srcs/ft_pwd.c\
# 				srcs/lexer/ms_lst.c\
# 				srcs/lexer/add_to_list.c\
# 				srcs/minishell.c\
# 				srcs/parser/parser.c\
# 				srcs/execve.c

SRC = $(shell find srcs -name "*.c")

OBJ_DIR = $(CACHE_DIR)/$(NAME)/obj
DEP_DIR = $(CACHE_DIR)/$(NAME)/dep

OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
DEP = $(addprefix $(DEP_DIR)/,$(SRC:.c=.d))

# LIBFT_DEP = $(shell find $(shell pwd)/.cache/Libft/dep -name "*.d")
LIBFT_OBJS = $(addprefix /home/asuc/Documents/42/minishell/Libft/obj/, $(notdir $(SRC:.c=.o)))

DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

_CURR = 0
_TOTAL = $(words $(OBJ))
_TOTAL_LEN = $(shell echo $(_TOTAL) | wc -c)
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	@if [ $(_CURR) -eq 1 ]; then \
		printf "$(Bblue)Compilation object files for $(NAME)$(RESET)\n"; \
	fi
	$(eval _CURR=$(shell echo $$(($(_CURR)+1))))
	$(eval _PERCENTAGE=$(shell echo $$(($(_CURR)*100/$(_TOTAL)))))
	@printf "\033[2K\r$(Green)[$(shell printf "% 3s" "$(_PERCENTAGE)")%%] $(shell printf "%*d/%d" $(_TOTAL_LEN) $(_CURR) $(_TOTAL)) Compiling $<\r"
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*)
	@$(COMP) -gdwarf-4 -fPIE $(CFLAGS) $(DEP_FLAGS) -o $@ -c $<
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

define banner
$(Green)Welcome to $(RESET)
$(Bblue)                            ##          ##  ## $(RESET)
$(Bblue)                       ####  #           #   #  $(RESET)
$(Bblue)                      #   #  #           #   # $(RESET)
$(Bblue)   ### ###   ### ###  ##     ###    ##   #   # $(RESET)
$(Bblue)  #  # #  # #  # #  #  ###   #  #  #  #  #   #  $(RESET)
$(Bblue)  #     ### #     ###    ##  #  #  ####  #   # $(RESET)
$(Bblue)  #    #  # #    #  # #   #  #  #  #     #   #  $(RESET)
$(Bblue)   ### ####  ### #### ####  ### ##  ### ### ###$(RESET)
$(Green)                              By: $(AUTHORS)$(RESET)
endef

all : _banner  $(NAME)

_banner:
	$(info $(banner))

debug: CFLAGS += -g
debug: DEBUG = 1
debug: re

$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

$(NAME) : $(LIBFT) $(OBJ)
# @printf "$(LIBFT_DEP)\n"
# @printf "$(DEP)"
	@echo "\033[2K\r[100%] Linking $(NAME)"
	@$(COMP) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_DIR)libft.a -lreadline
	@printf "$(BGreen)Compilation Final $(NAME)$(RESET)\n"

 $(LIBFT):
	@printf "\n\n$(Bblue)Compilation object files for libft.a$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) DEBUG=$(DEBUG) CACHE_DIR=$(CACHE_DIR) -j

clean :
	@$(MAKE) -C $(LIBFT_DIR) CACHE_DIR=$(CACHE_DIR) clean
	@rm -rf $(OBJ_DIR) $(DEP_DIR)
	@printf "$(BRed)Erase all obj and deb files$(RESET)\n"

fclean : clean
	@$(MAKE) -C $(LIBFT_DIR) CACHE_DIR=$(CACHE_DIR) fclean
	@rm -rf $(NAME) $(LIBFT) $(CACHE_DIR)
	@printf "$(BRed)Erase $(NAME), libft.a$(RESET)\n\n"

re : fclean all

-include $(DEP)
-include $(LIBFT_OBJS)

# Debugging
test : all
	@valgrind --error-limit=no --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=suppressions.supp ./$(NAME)

test2 : all
	@valgrind --error-limit=no --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

.PHONY: all fclean clean re banner


