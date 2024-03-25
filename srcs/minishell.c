/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/13 00:56:22 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_return_code	= 0;

void	init_data(t_data *data)
{
	rl_catch_signals = 0;
	data->prompt_top = NULL;
	data->cmd_prompt = NULL;
	data->quote_state = 0;
	data->command_top = NULL;
	data->fd_out = 1;
	data->fd_in = 0;
}

static int	ft_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

void	choose_case(t_env *env, t_data *data, char *line)
{
	(void)line;
	if (data->command_top == NULL || data->command_top->cmd == NULL)
		return ;
	if (ft_strcmp(data->command_top->cmd, "exit") == 0)
		ft_exit(data, env, "exit", 0);
	if (ft_strcmp(data->command_top->cmd, "cd") == 0)
		ft_cd(data, env);
	else if (ft_strcmp(data->command_top->cmd, "export") == 0)
		ft_export(env, data->command_top->args[1]);
	else if (ft_strcmp(data->command_top->cmd, "env") == 0)
		ft_env(env);
	else if (ft_strcmp(data->command_top->cmd, "unset") == 0)
		ft_unset(env, data);
	else if (ft_strcmp(data->command_top->cmd, "echo") == 0)
		ft_echo(data);
	else if (ft_strcmp(data->command_top->cmd, "pwd") == 0)
		ft_pwd(env);
	else
		g_return_code = execve_path_env(data->command_top->cmd,
			data->command_top->args, env, data);
}

int	wait_cmd_prompt(t_data *data, t_env *env)
{
	char	*line;

	while (1)
	{
		if (data->prompt_top)
			free_token_lst(data);
		if (data->command_top)
			free_command(data);
		get_cmd_prompt(data, env);
		if (data->cmd_prompt == NULL)
			return (-1);
		line = readline(data->cmd_prompt);
		if (line == NULL)
			ft_exit(data, env, "exit", 0);
		add_history(line);
		lexer(line, data);
		parser(data);
		choose_case(env, data, line);
		if (line != NULL)
			free(line);
		if (data->fd_out != 1)
		{
			// dup2(1, data->fd_out);
			close(data->fd_out);
			data->fd_out = 1;
		}
	}
}

int	main(int argc __attribute__((unused)),
		char *argv[] __attribute__((unused)),
		char **envp)
{
	t_data	*data;
	t_env	*env;

	signal(SIGINT, (void (*)(int))sig_handler);
	signal(SIGQUIT, (void (*)(int))sig_handler);
	data = malloc(sizeof(t_data));
	env = malloc(sizeof(t_env));
	data->env = env;
	// printf("\033c");
	init_data(data);
	get_env(env, envp);
	wait_cmd_prompt(data, env);
	return (0);
}
