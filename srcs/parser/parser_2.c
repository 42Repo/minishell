/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/18 22:52:55 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_output(char *file, t_command *command, int mode, t_data *data)
{
	data->g_return_code = 1;
	command->fd_out = -1;
	if (command->cmd)
		free (command->cmd);
	command->cmd = NULL;
	if (mode == 1 && file)
		free(file);
	return (1);
}

void	error_input(char *file, t_command *command, char *error)
{
	put_error("minishell: ", file, error);
	if (command->cmd)
		free (command->cmd);
	command->cmd = NULL;
	if (command->fd_in > 2)
		close(command->fd_in);
	if (command->fd_out > 2)
		close(command->fd_out);
	command->fd_out = -1;
}

int	check_permission(char *file, t_command *command,
	t_data *data, struct stat sb)
{
	if (!access(file, F_OK) && S_ISDIR(sb.st_mode))
		put_error("minishell: ", file, ": Is a directory\n");
	if (!access(file, F_OK) && S_ISDIR(sb.st_mode))
		return (error_output(file, command, 0, data));
	if (!access(file, F_OK) && access(file, R_OK))
		put_error("minishell: ", file, ": Permission denied\n");
	if (!access(file, F_OK) && access(file, R_OK))
		return (error_output(file, command, 0, data));
	else
		return (0);
}

void	select_output(char *file, int mode, t_command *command, t_data *data)
{
	if (command->fd_out == -1)
		return ;
	if (command->fd_out > 2)
		close(command->fd_out);
	command->fd_out = 1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
	{
		put_error("minishell: ", file, ": Permission denied\n");
		command->fd_out = -1;
		data->g_return_code = 1;
		if (command->cmd)
			free (command->cmd);
		command->cmd = NULL;
		return ;
	}
	if (check_dir(file, command, data))
		return ;
	if (mode == 1)
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

void	select_input(char *file, t_command *command, t_data *data)
{
	if (command->fd_in == -1)
		return ;
	if (command->fd_in != 0)
		close(command->fd_in);
	if (command->fd_in != 0)
		command->fd_in = 0;
	if (access(file, F_OK) == -1)
	{
		error_input(file, command, ": No such file or directory\n");
		data->g_return_code = 1;
		return ;
	}
	if (access(file, R_OK) == -1)
	{
		error_input(file, command, ": Permission denied\n");
		data->g_return_code = 126;
		return ;
	}
	command->fd_in = open(file, O_RDONLY);
	if (command->fd_in == -1)
		command->fd_in = 0;
}
