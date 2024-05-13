/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:37:11 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 23:47:11 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_command_pipe(t_command *command, t_data *data, int input_fd,
			int output_fd)
{
	if (data->prompt_top->type == END
		|| command == NULL || command->cmd == NULL)
		return ;
	command->pid = fork();
	if (command->pid == -1)
	{
		perror("fork");
		return ;
	}
	if (command->pid == 0)
	{
		setup_redirections(input_fd, output_fd);
		if (command->pipe[1] == output_fd)
			close(command->pipe[0]);
		if (ft_strcmp(command->cmd, "exit") == 0)
			ft_exit(command, data, "", 0);
		if (execute_builtin(command, data->env, data) == 1)
			ft_exit(command, data, "", 1);
		g_return_code = execve_path_env(command->cmd,
				command->args, data->env, data);
		exit(g_return_code);
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
		(*prev_fd) = command->fd_in;
		command->fd_in = STDIN_FILENO;
	}
	if (command->next && pipe(command->pipe) == -1)
	{
		perror("pipe");
		return ;
	}
}

void	execute_command(t_command *command, t_data *data, int input_fd,
			int output_fd)
{
	if (command == NULL || command->cmd == NULL)
		return ;
	if (handle_builtin(command, data, output_fd, input_fd) == 1)
		return ;
	command->pid = fork();
	if (command->pid == -1)
	{
		perror("fork");
		return ;
	}
	if (command->pid == 0)
	{
		setup_redirections(input_fd, output_fd);
		g_return_code = execve_path_env(command->cmd, command->args,
				data->env, data);
		exit(g_return_code);
	}
	waitpid(command->pid, &g_return_code, 0);
	if (WIFSIGNALED(g_return_code))
		g_return_code = 128 + WTERMSIG(g_return_code);
	if (WIFEXITED(g_return_code))
		g_return_code = WEXITSTATUS(g_return_code);
	if (g_return_code == 130)
		printf("\n");
}
