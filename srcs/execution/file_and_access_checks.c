/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_and_access_checks.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:44:59 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 18:50:33 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_executable(const char *path, struct stat *buf)
{
	if (!access(path, F_OK))
	{
		stat(path, buf);
		if (!access(path, X_OK) && !S_ISDIR(buf->st_mode))
			return (1);
		else
		{
			if (S_ISDIR(buf->st_mode))
				errno = EISDIR;
			else
				errno = EACCES;
			return (0);
		}
	}
	return (0);
}

int	has_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}
