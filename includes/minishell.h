/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/01/29 17:32:07 by asuc              #+#    #+#             */
/*   Updated: 2024/02/13 22:47:02 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>


/* STRUCTURES */
typedef enum s_token_type
{
	WORD,
	PIPE,
	REDIR,
	END
}	t_token_type;

/**
 * @brief type 0 -> any other char str / type 1 -> pipe / type 2 -> redirect
 *
 */
typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	struct s_command	*next;
}	t_command;

// 1 = pipe, 2 = redirect, 3 = end
typedef struct s_next
{
	int			action;
	t_command	*next;
}	t_next;

typedef struct s_data
{
	t_token		*prompt_top;
	t_token		*selected;
	t_command	*command_top;
	char		*old_cd;
	int			quote_state;
	char		*cmd_prompt;
}	t_data;

/* FUNCTIONS */
void	parser(t_data *data);
void free_command(t_data *data);
void    free_tab(char **tab);
/**
 * @brief turn a str input into a tokenized chained list
 *
 * @param str the string to turn into chained list
 * @param data the main struct
 */
void	lexer(char *str, t_data *data);

/**
 * @brief [DEBUG] - print a tokenized chained list
 *
 * @param node the top node of the list
 */
void	print_stack(t_token *node);

/**
 * @brief addapted version of the libft's ft_lstadd_back
 *
 * @param lst a pointer to the top node of the list
 * @param new the node to ad to the lst
 * @param data the main struct
 */
void	ms_lstadd_back(t_token **lst, t_token *new, t_data *data);

/**
 * @brief addapted version of the libft's ft_lstnew
 *
 * @param type the type your node will have (WORD, PIPE, REDIR or END)
 * @param value the value of the str
 */
t_token	*ms_lstnew(t_token_type type, char *value);

/**
 * @brief get the last node of a tokenized list
 *
 * @param lst the first node of the list
 */
t_token	*ms_lstlast(t_token *lst);

/**
 * @brief print the startup header
 */
void	put_header(void);

int		max_len(char *str, int nb);

/**
 * @brief add a token to the list, trimming spaces and then filtering quotes
 *
 * @param data the main struct
 * @param str is attributed to the node
 * @param len the len of the str you wanna add (can be only a portion of str)
 * @param type the type your node will have (WORD, PIPE, REDIR or END)
 */
void	add_token_to_list(t_data *data, char *str, int len, t_token_type type);

/**
 * @brief add a REDIR type node to the tokenized list
 *
 * @param str is attributed to the node
 * @param i	str[i] = char we are checking
 * @param j str[j] = where last word ended
 * @param data the main struct
 */
void	add_redir(char *str, int *i, int *j, t_data *data);

/**
 * @brief add a WORD type node to the tokenized list
 *
 * @param str is attributed to the node
 * @param i	str[i] = char we are checking
 * @param j str[j] = where last word ended
 * @param data the main struct
 *
 * @return 1 c'est un easter egg bonsoir
 *
 */
int		add_word_to_list(char *str, int *i, int *j, t_data *data);

/**
 * @brief add a PIPE type node to the tokenized list
 *
 * @param str is attributed to the node
 * @param i	str[i] = char we are checking
 * @param j str[j] = where last word ended
 * @param data the main struct
 */
void	check_pipe_redir(char *str, int *i, int *j, t_data *data);

/**
 * @brief return a copy of str of len char
 *
 * @param str the original str
 * @param len the len of the returned str
 *
 * @return the copied str
 */
char	*set_token_str(char *str, int len);

/**
 * @brief find which is the first quote used in a str, between ' and "
 *
 * @param str the scanned str
 *
 * @return the quote type (0 = no, 1 = single, 2 = double)
 */
int		get_quote_type(char *str);

/**
 * @brief removes every iterations of one type of quote
 * @brief(determined by get_quote_type())
 *
 * @param str the scanned str
 *
 * @return the new and filtered str
 */
char	*remove_quotes(char *str, t_data *data);

/**
 * @brief knows if a char is in a single, double or no quote
 *
 * @param i	the actual quote state (0 = no, 1 = single, 2 = double)
 * @param c the scanned char
 *
 * @return the new quote state (0 = no, 1 = single, 2 = double)
 */
int		quote_management(int i, char c);

/**
 * @brief free the curent tokenized chained list
 *
 * @param data the main struct
 */
void	free_token_lst(t_data *data);

/**
 *
 * @brief change the current directory
 *
 * @param path the string of the path to change to
 * @param env the struct env list
 * @return int 0 if success, 1 if error
 */
int		ft_cd(char *path, t_env *env);

/**
 * @brief initialize the prompt of shell
 *
 * @param data the struct data
 * @param env the struct env list
 * @return int 0 if success, 1 if error
 */
int		get_cmd_prompt(t_data *data, t_env *env);

/**
 * @brief set the environment variables from the envp in the struct env list
 *
 * @param env the struct env list
 * @param envp the environment variables
 */
void	get_env(t_env *env, char **envp);

/**
 * @brief Add an environment variable
 *
 * @param env the struct env list
 * @param line the name of the variable to add, ex : "PATH=..." or "HOME=..."
 * @return char* the value of the variable ex : "/usr/bin" or "/home/user"
 */
char	*ft_export(t_env *env, char *name);

/**
 * @brief Get the env value string object
 *
 * @param env the struct env list
 * @param name the name of the variable to get, ex : "PATH" or "HOME"
 * @return char* the value of the variable ex : "/usr/bin" or "/home/user"
 */
char	*get_env_value_string(t_env *env, char *name);

/**
 * @brief Get the env value ptr object
 *
 * @param env the struct env list
 * @param name the name of the variable to get, ex : "PATH=..." or "HOME=..."
 * @return t_env* the pointer to the variable ex : "/usr/bin" or "/home/user"
 */
t_env	*get_env_value_ptr(t_env *env, char *name);

/**
 * @brief Get the env value ptr by name object
 *
 * @param env the struct env list
 * @param name the name of the variable to get, ex : "PATH" or "HOME"
 * @return t_env* the pointer to the variable ex : "/usr/bin" or "/home/user"
 */
t_env	*get_env_value_ptr_by_name(t_env *env, char *name);

/**
 * @brief delete an environment variable
 *
 * @param env the struct env list
 * @param line the name of the variable to delete, ex : "PATH" or "HOME"
 */
void	ft_unset(t_env *env, char *line);

/**
 * @brief print a string with or without a newline
 *
 * @param line the string to print
 * @param mode 0 = with newline, 1 = without newline
 */
void	ft_echo(char *line, int mode);

/**
 * @brief print the path of the current directory
 *
 * @param env the struct env list
 */
void	ft_pwd(t_env *env);

int		execve_path_env(char *cmd, char **args, t_env *env);

#endif
