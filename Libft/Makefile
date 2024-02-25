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

NAME = libft.a
PROJECT_NAME = Libft
COMP = clang
CFLAGS = -Wall -Werror -Wextra
RESET	=	\033[0m
BCyan	=	\033[1;36m
BGreen	=	\033[1;32m
Green	=	\033[0;32m

SRC = ft_strlen.c\
	ft_atoi.c\
	ft_strdup.c\
	ft_strlcat.c\
	ft_strlcpy.c\
	ft_strncmp.c\
	ft_isalpha.c\
	ft_isdigit.c\
	ft_isalnum.c\
	ft_isascii.c\
	ft_bzero.c\
	ft_calloc.c\
	ft_isprint.c\
	ft_tolower.c\
	ft_toupper.c\
	ft_strchr.c\
	ft_strrchr.c\
	ft_memset.c\
	ft_strnstr.c\
	ft_memcmp.c\
	ft_memcpy.c\
	ft_memmove.c\
	ft_memchr.c\
	ft_strjoin_free.c\
	ft_isnamespace.c \
	ft_strtok_r.c\
	ft_strndup.c\
	ft_strcmp.c\
	ft_substr.c\
	ft_strjoin.c\
	ft_strtrim.c\
	ft_split.c\
	ft_itoa.c\
	ft_strmapi.c\
	ft_striteri.c\
	ft_putchar_fd.c\
	ft_putstr_fd.c\
	ft_putendl_fd.c\
	ft_putnbr_fd.c \
	ft_lstadd_back_bonus.c\
	ft_lstadd_front_bonus.c\
	ft_lstlast_bonus.c\
	ft_lstnew_bonus.c\
	ft_lstsize_bonus.c\
	ft_lstdelone_bonus.c\
	ft_lstclear_bonus.c\
	ft_lstiter_bonus.c\
	ft_lstmap_bonus.c\
	get_next_line.c\
	ft_printf_fomat_utils.c\
	ft_printf.c\
	ft_printf_utils.c \

CACHE_DIR ?= $(shell pwd)/.cache
DEBUG ?= 0

ifeq ($(DEBUG), 1)
	CFLAGS += -g3
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
	# @sed -i 's|^\(.*\):|$(PREFIX_PATH)/\1:|' $(DEP_DIR)/$*.d
	# @sed -i 's| \./| $(PREFIX_PATH)/|g' $(DEP_DIR)/$*.d
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d


all : $(NAME)

-include $(DEP)
$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

$(NAME) : $(OBJ)
	@echo "\033[2K\r[100%] Linking $(NAME)"
	@ar -rcs $(NAME) $(OBJ)
	@printf "$(BGreen)Compilation OK libft.a$(RESET)\n\n"

clean :
	@rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean : clean
	@rm -rf $(NAME) $(CACHE_DIR)

re : fclean all


.PHONY: all fclean clean re
