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

void	init_data(t_data *data)
{
	rl_catch_signals = 0;
	data->prompt_top = NULL;
	data->cmd_prompt = NULL;
	data->quote_state = 0;
	data->command_top = NULL;
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

void	chose_case(t_env *env, t_data *data, char *line)
{
	if (ft_strncmp(line, "exit", max_len(line, 4)) == 0)
		ft_exit(data, env, "exit", 0);
	if (ft_strncmp(line, "cd ", 3) == 0)
		ft_cd(data, env);
	else if (ft_strncmp(line, "export ", 7) == 0)
		ft_export(env, data->command_top->args[1]);
	else if (ft_strncmp(line, "env", 3) == 0)
		ft_env(env);
	else if (ft_strncmp(line, "unset ", 6) == 0)
		ft_unset(env, data);
	else if (ft_strncmp(line, "echo ", 5) == 0)
		ft_echo(data);
	else if (ft_strncmp(line, "pwd", 3) == 0)
		ft_pwd(env);
	else
		execve_path_env(data->command_top->cmd,
			data->command_top->args, env, data);
}

int	wait_cmd_prompt(t_data *data, t_env *env)
{
	char	*line;

	while (1)
	{
		get_cmd_prompt(data, env);
		if (data->cmd_prompt == NULL)
			return (-1);
		line = readline(data->cmd_prompt);
		if (line == NULL)
			ft_exit(data, env, "exit", 0);
		add_history(line);
		lexer(line, data);
		parser(data);
		chose_case(env, data, line);
		if (line != NULL)
			free(line);
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
	data = sig_handler(0);
	env = malloc(sizeof(t_env));
	data->env = env;
	printf("\033c");
	init_data(data);
	get_env(env, envp);
	wait_cmd_prompt(data, env);
	return (0);
}
