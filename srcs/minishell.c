/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:59:39 by asuc              #+#    #+#             */
/*   Updated: 2024/05/15 20:14:45 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_return_code = 0;

void	close_all_fd(t_command *command)
{
	if (command->next == NULL && (command->fd_out == -1
			|| command->fd_in == -1))
	{
		g_return_code = 1;
	}
	if (command->fd_in > 2)
		close(command->fd_in);
	if (command->fd_out > 2)
		close(command->fd_out);
}

void	wait_for_commands(t_command *command, t_data *data)
{
	int	status;

	status = 0;
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
		close_all_fd(command);
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
	g_return_code = 0;
	data->line = NULL;
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
		data->line = ft_strtrim_free(readline(data->cmd_prompt), " ");
		if (data->line == NULL)
			ft_exit(data->command_top, data, "exit", 1);
		if (ft_strlen(data->line) > 0)
			add_history(data->line);
		execute_loop(data, &data->line);
		if (data->line != NULL)
			free(data->line);
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
