# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 17:42:16 by asuc              #+#    #+#              #
#    Updated: 2023/08/12 17:42:16 by asuc             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include libft_file.mk


SRC = $(SRC_LIBFT)
NAME = libft.a
PROJECT_NAME = Libft
COMP = clang
CFLAGS = -Wall -Werror -Wextra
RESET	=	\033[0m
BCyan	=	\033[1;36m
BGreen	=	\033[1;32m
Green	=	\033[0;32m

CACHE_DIR ?= $(shell pwd)/.cache
DEBUG ?= 0

ifeq ($(DEBUG), 1)
	CFLAGS += -gdwarf-4
endif

OBJ_DIR = $(CACHE_DIR)/$(PROJECT_NAME)/obj
DEP_DIR = $(CACHE_DIR)/$(PROJECT_NAME)/dep


OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
DEP = $(addprefix $(DEP_DIR)/,$(SRC:.c=.d))

DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

_CURR = 0
_TOTAL = $(words $(OBJ))
_TOTAL_LEN = $(shell echo $(_TOTAL) | wc -c)
PREFIX_PATH = Libft/

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	$(eval _CURR=$(shell echo $$(($(_CURR)+1))))
	$(eval _PERCENTAGE=$(shell echo $$(($(_CURR)*100/$(_TOTAL)))))
	@printf "\033[2K\r$(Green)[$(shell printf "% 3s" "$(_PERCENTAGE)")%%] $(shell printf "%*d/%d" $(_TOTAL_LEN) $(_CURR) $(_TOTAL)) Compiling $<\r"
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*)
	@$(COMP) $(CFLAGS) $(DEP_FLAGS) -o $@ -c $<
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

all : $(NAME)

$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

$(NAME) : $(OBJ)
	@printf "$(Green)\033[2K\r[100%%] Linking $(NAME)\n"
	@ar -rcs $(NAME) $(OBJ)
	@printf "$(BGreen)Compilation OK libft.a$(RESET)\n\n"

clean :
	@rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean : clean
	@rm -rf $(NAME) $(CACHE_DIR)

re : fclean all

-include $(DEP)

.PHONY: all fclean clean re
