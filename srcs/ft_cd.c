/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:46:32 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 15:46:39 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_cd(char *path)
{
	if (errno == 2)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else if (errno == 13)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else if (errno == 20)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": not a directory\n", 2);
	}
	else
		perror("cd");
	return (-1);
}

int	ft_cd(char *path)
{
	char	*tmp;

	path = path + 3; // skip "cd " at the beginning of the string
	if (path[0] == '\0') // peut etre a changer en fonction de ce qu'on recoit en parametre
	{
		if (chdir(getenv("HOME")) == 0)
			return (0);
		else
			return (error_cd(getenv("HOME")));
	}
	if (path[0] == '/')
	{
		if (chdir(path) == 0)
			return (0);
		else
			return (error_cd(path));
	}
	tmp = ft_strjoin_free(ft_strjoin_free(getcwd(NULL, 0), "/"), path);
	if (chdir(tmp) == 0)
		return (0);
	else
		return (error_cd(path));
	return (0);
}
