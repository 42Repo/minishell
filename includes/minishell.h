/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:59:17 by asuc              #+#    #+#             */
/*   Updated: 2024/04/05 15:59:17 by asuc             ###   ########.fr       */
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
# include <limits.h>

# define LLONG_MAX_STR "9223372036854775807"
# define LLONG_MIN_STR "9223372036854775808"

extern sig_atomic_t	g_signal_received;

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
	int					fd_in;
	int					fd_out;
	int					fd_heredoc;
	pid_t				pid;
	char				random_name[21];
	int					pipe[2];
}	t_command;

// 1 = pipe, 2 = redirect, 3 = end
typedef struct s_next
{
	int			action;
	t_command	*next;
}	t_next;

typedef struct s_data
{
	t_token			*prompt_top;
	t_token			*selected;
	t_command		*command_top;
	t_env			*env;
	int				quote_state;
	char			*cmd_prompt;
	int				fd_in;
	int				fd_out;
	struct termios	*term;
	char			*line;
	int				g_return_code;
}	t_data;

typedef struct s_parser
{
	char	**tmp;
	char	*tmp2;
	char	*error;
}	t_parser;

/*  FUNCTIONS  */

/**
 * @brief 
 * 
 * @param selected 
 * @param i 
 * @param quote_state 
 * @return int 
 */
int			check_envar(char *str, int i, int quote_state);

/**
 * @brief 
 * 
 * @param tab 
 * @return char* 
 */
char		*join_replaced(char **tab);

/**
 * @brief Get the envar object
 * 
 * @param str 
 * @param len 
 * @param data 
 * @return char* 
 */
char		*get_envar(char *str, int len, t_data *data);

/**
 * @brief Get the envar len object
 * 
 * @param str 
 * @return int 
 */
int			get_envar_len(char *str);

/**
 * @brief Set the envar object
 * 
 * @param data 
 * @param selected 
 */
void		set_envar(t_data *data, t_token *selected);

/**
 * @brief the main parsing function
 *
 * @param data the main struct
 */
int			parser(t_data *data);

/**
 * @brief the main parsing the command
 *
 * @param selected the selected token
 * @param data the main struct
 * @param command the command struct
 * @param parser the parser struct
 *
 * @return the error message
 */
char		*parse_misc(t_token **selected, \
			t_command *command, t_parser *parser, t_data *data);

/**
 * @brief free the t_command linked list
 * 
 * @param data the main struct
 */
void		free_command(t_data *data);

/**
 * @brief get the redirections and calls the right 
 * functions to change in and out fd
 * 
 * @param selected 
 * @param data 
 * @param command 
 */
void		get_redir(t_token *selected, t_command *command, t_data *data);

/**
 * @brief a function to free a char ** tab
 * 
 * @param tab the tab you want to free
 */
void		free_tab(char **tab);

/**
 * @brief 
 * 
 * @param cmd 
 * @param args 
 * @param env 
 * @param data 
 * @return int 
 */
int			execve_path_env(char *cmd, char **args, t_env *env, t_data *data);

/**
 * @brief alocate memory for a new command struct and set values to NULL
 * 
 * @return t_command* 
 */
t_command	*init_command(void);

/**
 * @brief set the command->cmd to the selected token value
 * 
 * @param selected the selected token
 * @param command the command struct
 */
void		parse_word(t_token *selected, t_command *command);

/**
 * @brief removes every quotes from the tokenized list
 * 
 * @param data the main struct
 */
void		clear_token_quotes(t_data *data);

/**
 * @brief 
 * 
 * @param cmd 
 * @param path_env 
 * @return char* 
 */
char		*find_cmd_path(char *cmd, char *path_env);

/**
 * @brief check if the redir is valid
 * 
 * @param selected the selected token
 * @param command the command struct
 * @return if NULL -> no error, else -> error message
 */
char		*parse_redir(t_token **selected, t_command *command, t_data *data);

/**
 * @brief takes 3 str and print them in the error fd
 * 
 * @param str1 1st str
 * @param str2 2nd str
 * @param str3 3rd str
 */
void		put_error(char *str1, char *str2, char *str3);

/**
 * @brief removes every token with a strlen(value) == 0
 * 
 * @param data the main struct
 */
void		remove_empty_tokens(t_data *data);

/**
 * @brief check if the pipe is valid, and set the next command
 * 
 * @param selected the pipe token
 * @param command the command struct
 * @return if NULL -> no error, else -> error message
 */
char		*parse_pipe(t_token **selected, t_command **command);

/**
 * @brief set the fd_in of the command struct
 * 
 * @param command the command struct
 * @param selected the selected token
 */
int			check_exec_command_path(char *path);

/**
 * @brief calls ft_strtrim and free the input str
 * 
 * @param s1 the str to trim
 * @param set the charset you trim from s1
 * @return the trimmed str
 */
char		*ft_strtrim_free(char *s1, char *set);

/**
 * @brief 
 * 
 * @param path 
 * @return int 
 */
int			execve_error(char *path);

/**
 * @brief put the t_env list in a char ** tab
 * 
 * @param env the struct env list
 * @return the char ** env
 */
char		**env_to_tab(t_env *env);

/**
 * @brief exits quoicoushell
 * 
 * @param cmd 1st struct of the command linked list
 * @param data the main struct
 * @param env the env list
 * @param exit_msg the message to print before exiting
 */
void		ft_exit(t_command *command, t_data *data, char *exit_msg,
				int check_arg);

/**
 * @brief get the len of a char ** tab
 * 
 * @param tab the tab you want to get the len
 * @return len
 */
int			ft_tablen(char **tab);

/**
 * @brief manage the signals
 * 
 * @param num signal number
 */
void		sig_handler(int num);

/**
 * @brief get the value of the environment variable
 * 
 * @param str the str where there is the name of the variable
 * @param len the len of the name
 * @param data the main struct
 * @return the value of the variable 
 */
char		*get_envar(char *str, int len, t_data *data);

/**
 * @brief replace every environment variable in a token->value
 * 
 * @param data the main struct
 * @param selected the selected token
 * @param i the index of the token
 * @return the new value of the token
 */
char		*replace_envar(t_data *data, t_token *selected, int i, char ***tab);

/**
 * @brief add a char * to a char ** tab
 * 
 * @param tab the tab you want to add the str
 * @param line the line to add
 * @return the new tab
 */
char		**join_tab(char **tab, char *line);

/**
 * @brief print the env alphabetically sorted
 * 
 * @param env 
 */
void		print_sorted_env(t_env *env);

/**
 * @brief the envar replacement function
 * 
 * @param data the main struct
 * @return if NULL -> no error, else -> error message
 */
char		*expander(t_data *data);

/**
 * @brief 
 * 
 * @param command 
 * @param env 
 * @return int 
 */
int			ft_env(t_command *command, t_env *env, t_data *data);

/**
 * @brief free the env list
 * 
 * @param env 
 */
void		free_env(t_env *env);

/**
 * @brief open the heredoc files
 * 
 * @param file the EOF str
 * @param data 
 * @param command 
 */
int			heredoc(char *file, t_data *data, t_command *command);

/**
 * @brief a ft_strsplit that doesn't split if the char is in a quote
 * 
 * @param str the str to split
 * @param charset the charset to split with
 * @return char** the splited string
 */
char		**ft_split_quote_state(char *str, const char *charset);

/**
 * @brief create a new token and add it next to the selected token
 * 
 * @param token the selected token
 * @param str the value of the new token
 * @param len the len of the value
 * @param type the type of the new token
 */
void		add_token_next(t_token *token, char *str, int len,
				t_token_type type);

/**
 * @brief add a token to the list next to the selected token
 * 
 * @param token the selected token
 * @param new the new one
 * @return t_token* 
 */
t_token		*ms_lstadd_next(t_token *token, t_token *new);

/**
 * @brief turn a str input into a tokenized chained list
 *
 * @param str the string to turn into chained list
 * @param data the main struct
 */
int			lexer(char *str, t_data *data);

/**
 * @brief addapted version of the libft's ft_lstadd_back
 *
 * @param lst a pointer to the top node of the list
 * @param new the node to ad to the lst
 * @param data the main struct
 */
void		ms_lstadd_back(t_token **lst, t_token *new, t_data *data);

/**
 * @brief addapted version of the libft's ft_lstnew
 *
 * @param type the type your node will have (WORD, PIPE, REDIR or END)
 * @param value the value of the str
 */
t_token		*ms_lstnew(t_token_type type, char *value);

/**
 * @brief get the last node of a tokenized list
 *
 * @param lst the first node of the list
 */
t_token		*ms_lstlast(t_token *lst);

/**
 * @brief print the startup header
 */
void		put_header(void);

/**
 * @brief add a token to the list, trimming spaces and then filtering quotes
 *
 * @param data the main struct
 * @param str is attributed to the node
 * @param len the len of the str you wanna add (can be only a portion of str)
 * @param type the type your node will have (WORD, PIPE, REDIR or END)
 */
void		add_token_to_list(t_data *data, char *str,
				int len, t_token_type type);

/**
 * @brief add a REDIR type node to the tokenized list
 *
 * @param str is attributed to the node
 * @param i	str[i] = char we are checking
 * @param j str[j] = where last word ended
 * @param data the main struct
 */
void		add_redir(char *str, int *i, int *j, t_data *data);

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
int			add_word_to_list(char *str, int *i, int *j, t_data *data);

/**
 * @brief add a PIPE type node to the tokenized list
 *
 * @param str is attributed to the node
 * @param i	str[i] = char we are checking
 * @param j str[j] = where last word ended
 * @param data the main struct
 */
void		check_pipe_redir(char *str, int *i, int *j, t_data *data);

/**
 * @brief return a copy of str of len char
 *
 * @param str the original str
 * @param len the len of the returned str
 *
 * @return the copied str
 */
char		*set_token_str(char *str, int len);

/**
 * @brief find which is the first quote used in a str, between ' and "
 *
 * @param str the scanned str
 *
 * @return the quote type (0 = no, 1 = single, 2 = double)
 */
int			get_quote_type(char *str);

/**
 * @brief removes every iterations of one type of quote
 * @brief(determined by get_quote_type())
 *
 * @param str the scanned str
 *
 * @return the new and filtered str
 */
char		*remove_quotes(char *str, int mode);

/**
 * @brief knows if a char is in a single, double or no quote
 *
 * @param i	the actual quote state (0 = no, 1 = single, 2 = double)
 * @param c the scanned char
 *
 * @return the new quote state (0 = no, 1 = single, 2 = double)
 */
int			quote_management(int i, char c);

/**
 * @brief free the curent tokenized chained list
 *
 * @param data the main struct
 */
void		free_token_lst(t_data *data);

/**
 *
 * @brief change the current directory
 *
 * @param path the string of the path to change to
 * @param env the struct env list
 * @return int 0 if success, 1 if error
 */
int			ft_cd(t_command *command, t_env *env, t_data *data);

/**
 * @brief initialize the prompt of shell
 *
 * @param command the struct command
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
 * @return int, always 0
 */
int			ft_export(t_env *env, char **args, t_data *data);

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
void		ft_unset(t_env *env, t_command *command, t_data *data);

/**
 * @brief print a string with or without a newline
 *
 * @param line the string to print
 * @param mode 0 = with newline, 1 = without newline
 */
void		ft_echo(t_command *command, t_data *data);

/**
 * @brief print the path of the current directory
 *
 * @param env the struct env list
 */
void		ft_pwd(t_env *env, t_data *data);

int			check_dir(char *file, t_command *command, t_data *data);
char		*ft_strtrim_free(char *s1, char *set);
int			is_quote(char c);
int			is_delimiter(char c, const char *charset);
int	error_output(char *file, t_command *command, int mode, t_data *data);
int	check_permission(char *file, t_command *command,
	t_data *data, struct stat sb);
int			get_strdup_len(const char *src, const char *charset,
				char *current_quote);
void		set_split_var(int *i, int *j, int *in_qt, char *current_quote);
char		*ft_strdup_split_quote(const char *src, const char *charset);
void		if_case_quote(int *in_quote, char *current_quote, char str);
int			count_words_quote(const char *str, const char *charset);
void		if_quote_case(int *in_quote, char *current_quote, char *str, int j);
char		**ft_split_quote_state(char *str, const char *charset);
void		select_output(char *file, int mode, t_command *command,
				t_data *data);
void		select_input(char *file, t_command *command, t_data *data);
char		*get_path(t_env *env);
int			has_slash(char *cmd);
char		*skip_pipe(t_command **command, t_token **selected,
				t_parser *parser, t_data *data);
int			handle_path_error(char *cmd);
void		count_heredoc(t_data *data);
int			check_executable(const char *path, struct stat *buf);

/* BUILT-IN */
// ft_cd
char		*ft_export_single_arg(t_env *env, char *name);
void		update_env(t_env *env, char *new_value);
int			error_cd(char *path, t_env *env, int mode, t_data *data);
int			error_to_many_args(t_env *env, t_data *data);

// ft_exit
int			check_arg_exit(t_command *command, t_data *data);

// ft_export
int			ft_is_equal(char *str);
int			is_valid_identifier(char *str);
/**
 * @brief 
 * 
 * @param env 
 * @param arg	
 */
void		add_new_env_variable(t_env *env, char *arg);

// heredoc 
char		*expand_heredoc(char *str, t_data *data);
void		sig_child_handler(int sig);
void		sig_heredoc_handler(int sig);
void		set_signals(void (*handler));
int			test_open(t_command *command);
void		random_init(t_command *command);
void		handle_child_process(int fd, char *eof,
				t_command *command, t_data *data);

// main
void		setup_redirections(int *input_fd, int *output_fd);
void		choose_case(t_data *data);
void		setup_loop(t_data *data);
int			execute_builtin(t_command *command, t_env *env, t_data *data);
int			handle_builtin(t_command *command, t_data *data, int output_fd,
				int input_fd);
void		setup_signals(void *handler);
void		useless(int sig);
void		execute_command(t_command *command, t_data *data, int input_fd,
				int output_fd);
void		setup_command_execution(t_command *command, int *prev_fd);
void		execute_pipes(t_command *command, t_data *data, int prev_fd);
void		close_and_dup_pipes(t_command *command, int *prev_fd);
void		wait_for_commands(t_command *command, t_data *data);
void		init_data(t_data *data);
void		set_fd_in(t_command *command, t_token *selected);
void		free_resources(t_data *data, t_env *env);

#endif // MINISHELL_H
