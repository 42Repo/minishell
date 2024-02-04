/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/02/03 15:46:32 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 15:46:39 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_cd(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	if (errno == ENOENT)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (errno == EACCES)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (errno == ENOTDIR)
		ft_putstr_fd(": Not a directory\n", 2);
	else
		perror("");
	return (-1);
}

static int	other_case_cd(char *path)
{
	char	*tmp;

	tmp = NULL;
	if (path[0] == '\0')	// peut etre a changer en fonction de ce qu'on recoit en parametre
	{
		if (chdir(getenv("HOME")) != 0)
			return (error_cd(getenv("HOME")));
		return (0);
	}
	if (path[0] == '/')
	{
		if (chdir(path) != 0)
			return (error_cd(path));
		return (0);
	}
	if (path[0] == '~')
	{
		tmp = ft_strjoin(getenv("HOME"), path + 1);
		if (chdir(tmp) == 0)
			return (0);
		else
			return (error_cd(tmp));
	}
	return (1);
}

static int	old_cd(t_data *data)
{
	char	*tmp;

	if (data->old_cd == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (-1);
	}
	tmp = getcwd(NULL, 0);
	if (chdir(data->old_cd) != 0)
		return (error_cd(data->old_cd));
	free(data->old_cd);
	data->old_cd = tmp;
	return (0);
}

int	ft_cd(t_data *data, char *path)
{
	char	*tmp;

	path = path + 3;		// skip "cd " at the beginning of the string
	if (path == NULL)
		return (0);
	if (ft_strncmp(path, "-", max_len(path, 1)) == 0)
		return (old_cd(data));
	free(data->old_cd);
	data->old_cd = getcwd(NULL, 0);
	if (other_case_cd(path) == 0)
		return (0);
	tmp = ft_strjoin_free(ft_strjoin_free(getcwd(NULL, 0), "/"), path);
	if (chdir(tmp) != 0)
	{
		free(tmp);
		return (error_cd(path));
	}
	free(tmp);
	return (0);
}
