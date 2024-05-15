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

include Libft/libft_file.mk

BGreen		=	$(shell printf "\033[1;32m")
RESET		=	$(shell printf "\033[0m")
BRed		=	$(shell printf "\033[1;31m")
BCyan		=	$(shell printf "\033[1;36m")
Green		=	$(shell printf "\033[0;32m")
Blue 		=	$(shell printf "\033[0;34m")
Bblue		=	$(shell printf "\033[1;34m")
NAME		=	minishell
COMP		=	clang
CFLAGS		=	-Wall -Werror -Wextra
LIBFT_DIR	=	$(shell pwd)/Libft/
LIBFT		=	$(LIBFT_DIR)libft.a
CACHE_DIR	:=	$(addprefix $(shell pwd)/, .cache)
MAKE		=	make --no-print-directory
AUTHORS		=	asuc & mbuchs
DEBUG		=	0
SRC			=	srcs/signals.c \
		srcs/signal.c \
		srcs/get_env.c \
		srcs/command_execution.c \
		srcs/utils/split_quote_state_utils.c \
		srcs/utils/utils.c \
		srcs/utils/split_quote_state.c \
		srcs/built-in/ft_exit.c \
		srcs/built-in/ft_echo.c \
		srcs/built-in/ft_cd_utils.c \
		srcs/built-in/ft_env.c \
		srcs/built-in/ft_export.c \
		srcs/built-in/ft_export_utils.c \
		srcs/built-in/ft_cd.c \
		srcs/built-in/ft_export_args_less.c \
		srcs/built-in/ft_unset.c \
		srcs/built-in/ft_pwd.c \
		srcs/built-in/ft_exit_utils.c \
		srcs/data_initialization.c \
		srcs/redirections.c \
		srcs/parser/manage_command.c \
		srcs/parser/here_doc/heredoc_expander.c \
		srcs/parser/here_doc/signal_handlers.c \
		srcs/parser/here_doc/heredoc_child.c \
		srcs/parser/here_doc/random_name_generator.c \
		srcs/parser/here_doc/here_doc.c \
		srcs/parser/parse_token.c \
		srcs/parser/parser_2.c \
		srcs/parser/parser.c \
		srcs/minishell.c \
		srcs/builtin_handling.c \
		srcs/execution/command_execution.c \
		srcs/execution/environment_management.c \
		srcs/execution/error_management.c \
		srcs/execution/file_and_access_checks.c \
		srcs/lexer/ms_lst.c \
		srcs/lexer/lexer.c \
		srcs/lexer/lexer_2.c \
		srcs/lexer/add_to_list.c \
		srcs/lexer/expander/envar_2.c \
		srcs/lexer/expander/envar.c \
		srcs/lexer/expander/expander.c \
		srcs/get_cmd_prompt.c \
		srcs/command_execution_pipes.c \
		srcs/parser/parser_3.c

OBJ_DIR = $(CACHE_DIR)/$(NAME)/obj
DEP_DIR = $(CACHE_DIR)/$(NAME)/dep

OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
DEP = $(addprefix $(DEP_DIR)/,$(SRC:.c=.d))

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
	@printf "\033[2K$(Green)[$(shell printf "% 3s" "$(_PERCENTAGE)")%%] $(shell printf "%*d/%d" $(_TOTAL_LEN) $(_CURR) $(_TOTAL)) Compiling $<\r"
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*)
	@$(COMP) $(CFLAGS) $(DEP_FLAGS) -o $@ -c $<
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

define banner
$(Green)Welcome to $(RESET)
$(Bblue)                           oo                            .d88888b  dP                dP dP
                                                         88.    "' 88                88 88
.d8888b. dP    dP .d8888b. dP .d8888b. .d8888b. dP    dP `Y88888b. 88d888b. .d8888b. 88 88
88'  `88 88    88 88'  `88 88 88'  `"" 88'  `88 88    88       `8b 88'  `88 88ooood8 88 88
88.  .88 88.  .88 88.  .88 88 88.  ... 88.  .88 88.  .88 d8'   .8P 88    88 88.  ... 88 88
`8888P88 `88888P' `88888P' dP `88888P' `88888P' `88888P'  Y88888P  dP    dP `88888P' dP dP
      88
      dP                                                                                   $(RESET)
$(Green)                                                                           By: $(AUTHORS)$(RESET)
endef

all : _banner $(NAME)

_banner:
	$(info $(banner))

debug: CFLAGS += -gdwarf-4
debug: DEBUG = 1
debug: re

$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

LIBFT_TIMESTAMP := $(LIBFT_DIR).timestamp

$(LIBFT_TIMESTAMP): $(addprefix $(LIBFT_DIR)/, $(SRC_LIBFT)) $(addprefix $(LIBFT_DIR)/, $(HEADER_LIBFT))
	@printf "\n\n$(Bblue)Compilation object files for libft.a$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) DEBUG=$(DEBUG) CACHE_DIR=$(CACHE_DIR) -j
	@touch $(LIBFT_TIMESTAMP)

$(NAME) : $(LIBFT_TIMESTAMP) $(LIBFT) $(OBJ)
	@printf "$(Green)\033[2K[100%%] Linking $(NAME)\n"
	@$(COMP) $(CFLAGS) -o $(NAME) $(OBJ) -lreadline -L$(LIBFT_DIR) -lft
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

PWD = $(shell pwd)

test : debug
	@valgrind --error-limit=no --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=$(PWD)/suppressions.supp -s --track-fds=yes --trace-children=yes --log-fd=2 ./$(NAME)

.PHONY: all fclean clean re banner
