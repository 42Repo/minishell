/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_pipes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:13:40 by asuc              #+#    #+#             */
/*   Updated: 2024/05/28 16:52:39 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_all_pipes(t_command *command)
{
	if (command->next)
	{
		if (command->pipe[0] > 2)
			close(command->pipe[0]);
		if (command->pipe[1] > 2)
			close(command->pipe[1]);
	}
}

void	exit_and_close(t_command *cmd, t_data *data)
{
	close(0);
	close(1);
	close(2);
	close_all_pipes(cmd);
	ft_exit(cmd, data, ft_strdup(""), 0);
}

void	close_all_fd_fork(t_command *command)
{
	while (command)
	{
		if (command->fd_in > 2)
			close(command->fd_in);
		if (command->fd_out > 2)
			close(command->fd_out);
		command = command->next;
	}
}

void	execute_command_pipe(t_command *cmd, t_data *data, int input_fd,
		int output_fd)
{
	int	ret;

	if (data->prompt_top->type == END || cmd == NULL || cmd->cmd == NULL)
		return ;
	cmd->pid = fork();
	if (cmd->pid == -1)
		perror("fork");
	if (cmd->pid == -1)
		return ;
	if (cmd->pid == 0)
	{
		setup_redirections(&input_fd, &output_fd);
		if (ft_strcmp(cmd->cmd, "exit") == 0)
			exit_and_close(cmd, data);
		if (execute_builtin(cmd, data->env, data) == 1)
			exit_and_close(cmd, data);
		close_all_pipes(cmd);
		close_all_fd_fork(data->command_top);
		ret = execve_path_env(cmd->cmd, cmd->args, data->env, data);
		exit(ret);
	}
}

void	execute_pipes(t_command *command, t_data *data, int prev_fd)
{
	if (command->fd_out != -1 && command->fd_in != -1 && command->next)
	{
		if (command->fd_out != STDOUT_FILENO)
			execute_command_pipe(command, data, prev_fd, command->fd_out);
		else
			execute_command_pipe(command, data, prev_fd, command->pipe[1]);
	}
	else if (command->fd_out != -1 && command->fd_in != -1)
		execute_command_pipe(command, data, prev_fd, command->fd_out);
}
