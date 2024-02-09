/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:32:07 by asuc              #+#    #+#             */
/*   Updated: 2024/02/09 23:56:48 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

typedef enum s_token_type
{
	WORD,
	PIPE,
	REDIR,
	END
}	t_token_type;

typedef struct s_token
/*
 	type 0 -> any other char str
	type 1 -> pipe
	type 2 -> redirect
*/
{
	int				type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	t_token	*prompt_top;
	char	*old_cd;
	int		quote_state;
	char	*cmd_prompt;
}	t_data;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

t_token		*lexer(char *str, t_data *data);
void		print_stack(t_token *node);
void		ms_lstadd_front(t_token **lst, t_token *new);
void		ms_lstadd_back(t_token **lst, t_token *new, t_data *data);
t_token		*ms_lstnew(t_token_type type, char *value);
t_token		*ms_lstlast(t_token *lst);
int			max_len(char *str, int nb);
void		put_header(void);

/**
 * @brief change the current directory
 *
 * @param path the string of the path to change to
 * @param env the struct env list
 * @return int 0 if success, 1 if error
 */
int			ft_cd(char *path, t_env *env);

/**
 * @brief initialize the prompt of shell
 *
 * @param data the struct data
 * @param env the struct env list
 * @return int 0 if success, 1 if error
 */
int			get_cmd_prompt(t_data *data, t_env *env);

/**
 * @brief set the environment variables from the envp in the struct env list
 *
 * @param env the struct env list
 * @param envp the environment variables
 */
void		get_env(t_env *env, char **envp);

/**
 * @brief Add an environment variable
 *
 * @param env the struct env list
 * @param line the name of the variable to add, ex : "PATH=..." or "HOME=..."
 * @return char* the value of the variable ex : "/usr/bin" or "/home/user"
 */
char		*ft_export(t_env *env, char *name);

/**
 * @brief Get the env value string object
 *
 * @param env the struct env list
 * @param name the name of the variable to get, ex : "PATH" or "HOME"
 * @return char* the value of the variable ex : "/usr/bin" or "/home/user"
 */
char		*get_env_value_string(t_env *env, char *name);

/**
 * @brief Get the env value ptr object
 *
 * @param env the struct env list
 * @param name the name of the variable to get, ex : "PATH=..." or "HOME=..."
 * @return t_env* the pointer to the variable ex : "/usr/bin" or "/home/user"
 */
t_env		*get_env_value_ptr(t_env *env, char *name);

/**
 * @brief Get the env value ptr by name object
 *
 * @param env the struct env list
 * @param name the name of the variable to get, ex : "PATH" or "HOME"
 * @return t_env* the pointer to the variable ex : "/usr/bin" or "/home/user"
 */
t_env		*get_env_value_ptr_by_name(t_env *env, char *name);

/**
 * @brief delete an environment variable
 *
 * @param env the struct env list
 * @param line the name of the variable to delete, ex : "PATH" or "HOME"
 */
void		ft_unset(t_env *env, char *line);

/**
 * @brief print a string with or without a newline
 *
 * @param line the string to print
 * @param mode 0 = with newline, 1 = without newline
 */
void		ft_echo(char *line, int mode);

/**
 * @brief print the path of the current directory
 *
 * @param env the struct env list
 */
void		ft_pwd(t_env *env);

/*
	0 = no quote
	1 = simple quote
	2 = double quote
	3 = simple quote close
	4 = double quote close
*/
int			quote_management(int i, char c);

/* FUNCTIONS */

#endif
