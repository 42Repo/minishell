/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:37:11 by asuc              #+#    #+#             */
/*   Updated: 2024/05/15 13:14:52 by asuc             ###   ########.fr       */
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

void	execute_command(t_command *cmd, t_data *data, int input_fd,
		int output_fd)
{
	if (cmd == NULL || cmd->cmd == NULL || handle_builtin(cmd, data, output_fd,
			input_fd) == 1)
		return ;
	cmd->pid = fork();
	if (cmd->pid == -1)
		perror("fork");
	if (cmd->pid == -1)
		return ;
	if (cmd->pid == 0)
	{
		setup_redirections(&input_fd, &output_fd);
		g_return_code = execve_path_env(cmd->cmd, cmd->args, data->env, data);
		exit(g_return_code);
	}
	waitpid(cmd->pid, &g_return_code, 0);
	if (WIFSIGNALED(g_return_code))
		g_return_code = 128 + WTERMSIG(g_return_code);
	if (WIFEXITED(g_return_code))
		g_return_code = WEXITSTATUS(g_return_code);
	if (g_return_code == 130)
		printf("\n");
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
}
