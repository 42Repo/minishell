/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:59:39 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 23:54:54 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_return_code = 0;

void	wait_for_commands(t_command *command, t_data *data)
{
	int	status;

	command = data->command_top;
	while (command)
	{
		waitpid(command->pid, &status, 0);
		if (WIFEXITED(status))
			g_return_code = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			g_return_code = 128 + WTERMSIG(status);
		if (g_return_code == 130)
			printf("\n");
		command = command->next;
	}
	command = data->command_top;
	while (command)
	{
		if (command->next == NULL && (command->fd_out == -1
				|| command->fd_in == -1))
			g_return_code = 1;
		command = command->next;
	}
}

void	execute_loop(t_data *data, char **line)
{
	if (lexer((*line), data) == 0)
	{
		free((*line));
		(*line) = NULL;
		if (parser(data) == 0)
			choose_case(data);
		else
		{
			free_token_lst(data);
			free_command(data);
		}
	}
	else
		free_token_lst(data);
}

void	loop(t_data *data)
{
	char	*line;

	g_return_code = 0;
	line = NULL;
	while (1)
	{
		setup_loop(data);
		if (data->prompt_top)
			free_token_lst(data);
		if (data->command_top)
			free_command(data);
		get_cmd_prompt(data, data->env);
		if (data->cmd_prompt == NULL)
			break ;
		line = ft_strtrim_free(readline(data->cmd_prompt), " ");
		if (line == NULL)
			ft_exit(data->command_top, data, "exit", 1);
		if (ft_strlen(line) > 0)
			add_history(line);
		execute_loop(data, &line);
		if (line != NULL)
			free(line);
	}
}

int	main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)),
		char **envp)
{
	t_data	*data;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	data = ft_calloc(sizeof(t_data), 1);
	data->env = ft_calloc(sizeof(t_env), 1);
	init_data(data);
	get_env(data->env, envp);
	loop(data);
	return (0);
}
