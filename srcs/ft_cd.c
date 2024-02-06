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

static int	put_program_name(t_env *env)
{
	char		*tmp;
	const char	*tmp_error = "minishell";
	int			i;

	tmp = get_env_value_string(env, "_");
	if (tmp == NULL)
	{
		tmp = (char *)tmp_error;
	}
	i = ft_strlen(tmp) - 1;
	while (i >= 0 && tmp[i] != '/')
		i--;
	ft_putstr_fd(tmp + i + 1, 2);
	return (0);
}

static int	error_cd(char *path, t_env *env)
{
	put_program_name(env);
	ft_putstr_fd(": cd: ", 2);
	if (errno == 14)
	{
		ft_putstr_fd("HOME not set\n", 2);
		return (-1);
	}
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

static int	other_case_cd(char *path, t_env *env)
{
	char	*tmp;

	tmp = NULL;
	if (path[0] == '\0')
	{
		if (chdir(get_env_value_string(env, "HOME")) != 0)
		{
			return (error_cd(get_env_value_string(env, "HOME"), env));
		}
		return (0);
	}
	if (path[0] == '/')
	{
		if (chdir(path) != 0)
			return (error_cd(path, env));
		return (0);
	}
	if (path[0] == '~')
	{
		tmp = ft_strjoin(get_env_value_string(env, "HOME"), path + 1);
		if (chdir(tmp) != 0)
			return (error_cd(tmp, env));
		return (0);
	}
	return (1);
}

static int	old_cd(t_data *data, t_env *env)
{
	char	*tmp;
	char	*old_pwd;

	if (get_env_value_string(env, "OLDPWD") == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (-1);
	}
	tmp = getcwd(NULL, 0);
	if (chdir(data->old_cd) != 0)
		return (error_cd(data->old_cd, env));
	printf("%s\n", data->old_cd);
	free(data->old_cd);
	data->old_cd = tmp;
	return (0);
}

int	ft_cd(t_data *data, char *path, t_env *env)
{
	char	*tmp;

	path = path + 3;
	if (path == NULL)
		return (0);
	if (ft_strncmp(path, "-", max_len(path, 1)) == 0)
		return (old_cd(data, env));
	free(data->old_cd);
	data->old_cd = getcwd(NULL, 0);
	if (other_case_cd(path, env) == 0)
		return (0);
	tmp = ft_strjoin_free(ft_strjoin_free(getcwd(NULL, 0), "/"), path);
	if (chdir(tmp) != 0)
	{
		free(tmp);
		return (error_cd(path, env));
	}
	free(tmp);
	return (0);
}
