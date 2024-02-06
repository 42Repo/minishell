/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:32:07 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 20:17:32 by asuc             ###   ########.fr       */
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

typedef enum e_token_type
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
t_token		*ms_lstnew(int type, char *value);
t_token		*ms_lstlast(t_token *lst);
void		put_header(void);
int			ft_cd(char *path, t_env *env);
int			max_len(char *str, int nb);
int			get_cmd_prompt(t_data *data, t_env *env);
void		get_env(t_env *env, char **envp);
char		*ft_export(t_env *env, char *name);
t_env		*get_env_value_ptr(t_env *env, char *name);
char		*get_env_value_string(t_env *env, char *name);

#endif
