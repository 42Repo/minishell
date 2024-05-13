/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:40:44 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 18:50:30 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execve_error(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (126);
}

void	put_error(char *str1, char *str2, char *str3)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd(str3, 2);
}

int	check_exec_command_path(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
	{
		if (errno == ENOENT)
		{
			put_error("", path, ": No such file or directory\n");
			return (127);
		}
		else
			return (execve_error(path));
	}
	if (S_ISDIR(buf.st_mode))
	{
		put_error("minishell: ", path, ": Is a directory\n");
		return (126);
	}
	if (!(buf.st_mode & S_IXUSR))
	{
		put_error("minishell: ", path, ": Permission denied\n");
		return (126);
	}
	return (0);
}

int	check_exec_command(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
	{
		if (errno == ENOENT)
		{
			put_error("minishell: ", path, ": command not found\n");
			return (127);
		}
		else
			return (execve_error(path));
	}
	if (S_ISDIR(buf.st_mode))
	{
		put_error("minishell: ", path, ": Is a directory\n");
		return (126);
	}
	if (!(buf.st_mode & S_IXUSR))
	{
		put_error("minishell: ", path, ": Permission denied\n");
		return (126);
	}
	return (0);
}

int	handle_path_error(char *cmd)
{
	if (errno == EACCES)
	{
		put_error("minishell: ", cmd, ": Permission denied\n");
		return (126);
	}
	else
	{
		put_error("minishell: ", cmd, ": command not found\n");
		return (127);
	}
}
