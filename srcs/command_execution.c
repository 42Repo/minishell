/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:37:11 by asuc              #+#    #+#             */
/*   Updated: 2024/05/18 22:18:44 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_and_dup_pipes(t_command *command, int *prev_fd)
{
	if ((*prev_fd) != STDIN_FILENO)
	{
		if ((*prev_fd) > 2)
			close((*prev_fd));
		(*prev_fd) = STDIN_FILENO;
	}
	if (command->next)
	{
		(*prev_fd) = dup(command->pipe[0]);
		close(command->pipe[0]);
		close(command->pipe[1]);
	}
}

void	setup_command_execution(t_command *command, int *prev_fd)
{
	if (command->fd_in != STDIN_FILENO)
	{
		if (*prev_fd > 2)
			close(*prev_fd);
		(*prev_fd) = command->fd_in;
		command->fd_in = STDIN_FILENO;
	}
	if (command->next && pipe(command->pipe) == -1)
	{
		perror("pipe");
		return ;
	}
}

void	close_cmd_fd(t_command *command)
{
	if (command->fd_in > 2)
		close(command->fd_in);
	if (command->fd_out > 2)
		close(command->fd_out);
}

void	execute_command(t_command *cmd, t_data *data, int input_fd,
		int output_fd)
{
	int	ret;

	if (cmd == NULL || cmd->cmd == NULL || cmd->fd_in == -1 || cmd->fd_out == -1
		|| handle_builtin(cmd, data, output_fd, input_fd) == 1)
		return ;
	cmd->pid = fork();
	if (cmd->pid == -1)
		perror("fork");
	if (cmd->pid == -1)
		return ;
	if (cmd->pid == 0)
	{
		setup_redirections(&input_fd, &output_fd);
		ret = execve_path_env(cmd->cmd, cmd->args, data->env, data);
		exit(ret);
	}
	waitpid(cmd->pid, &data->g_return_code, 0);
	if (WIFSIGNALED(data->g_return_code))
		data->g_return_code = 128 + WTERMSIG(data->g_return_code);
	if (WIFEXITED(data->g_return_code))
		data->g_return_code = WEXITSTATUS(data->g_return_code);
	if (data->g_return_code == 130)
		printf("\n");
	close_cmd_fd(cmd);
}
