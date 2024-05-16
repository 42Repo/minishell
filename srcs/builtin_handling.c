/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:39:32 by asuc              #+#    #+#             */
/*   Updated: 2024/05/16 17:30:24 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_builtin(t_command *command, t_env *env, t_data *data)
{
	if (data->prompt_top->type == END)
		return (0);
	else if (ft_strcmp(command->cmd, "cd") == 0)
		ft_cd(command, env, data);
	else if (ft_strcmp(command->cmd, "export") == 0)
		ft_export(env, command->args, data);
	else if (ft_strcmp(command->cmd, "env") == 0)
		ft_env(command, env, data);
	else if (ft_strcmp(command->cmd, "unset") == 0)
		ft_unset(env, command, data);
	else if (ft_strcmp(command->cmd, "echo") == 0)
		ft_echo(command, data);
	else if (ft_strcmp(command->cmd, "pwd") == 0)
		ft_pwd(env, data);
	else
		return (0);
	return (1);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	handle_builtin(t_command *command, t_data *data, int output_fd,
		int input_fd)
{
	int	fd_in;
	int	fd_out;

	if (is_builtin(command->cmd))
	{
		if (input_fd != STDIN_FILENO)
			dup2(input_fd, STDIN_FILENO);
		if (output_fd != STDOUT_FILENO)
			dup2(output_fd, STDOUT_FILENO);
		if (ft_strcmp(command->cmd, "exit") == 0)
			ft_exit(command, data, "exit", 0);
		if (ft_strcmp(command->cmd, "exit") == 0)
			return (1);
		execute_builtin(command, data->env, data);
		fd_in = dup(data->fd_in);
		fd_out = dup(data->fd_out);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in > 2)
			close(fd_in);
		if (fd_out > 2)
			close(fd_out);
		return (1);
	}
	return (0);
}
