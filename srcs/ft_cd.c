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

static int	error_cd(char *path, t_env *env, int mode)
{
	put_program_name(env);
	ft_putstr_fd(": cd: ", 2);
	if (errno == 14)
	{
		ft_putstr_fd("HOME not set\n", 2);
		return (-1);
	}
	ft_putstr_fd(path, 2);
	if (mode == 1)
		free(path);
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
			return (error_cd(get_env_value_string(env, "HOME"), env, 0));
		return (0);
	}
	if (path[0] == '/')
	{
		if (chdir(path) != 0)
			return (error_cd(path, env, 0));
		return (0);
	}
	if (path[0] == '~')
	{
		tmp = ft_strjoin(get_env_value_string(env, "HOME"), path + 1);
		if (chdir(tmp) != 0)
			return (error_cd(tmp, env, 1));
		free(tmp);
		return (0);
	}
	return (1);
}

static int	old_cd(t_env *env)
{
	char	*tmp;
	char	*tmp2;

	if (get_env_value_string(env, "OLDPWD") == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (-1);
	}
	tmp = getcwd(NULL, 0);
	if (chdir(get_env_value_string(env, "OLDPWD")) != 0)
	{
		tmp2 = (tmp);
		tmp = ft_strjoin("OLDPWD=", tmp2);
		free(tmp2);
		ft_export(env, tmp);
		free(tmp);
		printf("%s\n", get_env_value_string(env, "OLDPWD"));
		return (0);
	}
	printf("%s\n", get_env_value_string(env, "OLDPWD"));
	tmp2 = ft_strjoin("OLDPWD=", tmp);
	ft_export(env, tmp2);
	free(tmp2);
	free(tmp);
	return (0);
}

int	ft_cd(t_data *data, t_env *env)
{
	char	*tmp;
	char	*tmp2;
	char	*path;
	int		mode;

	if (data->command_top->args[1])
	{
		path = data->command_top->args[1];
		mode = 0;
	}
	else
	{
		path = ft_strdup("~/");
		printf("path = %s\n", path);
		mode = 1;
	}
	if (ft_strcmp(path, "-") == 0)
		return (old_cd(env));
	tmp2 = getcwd(NULL, 0);
	tmp = ft_strjoin("OLDPWD=", tmp2);
	ft_export(env, tmp);
	free(tmp2);
	free(tmp);
	if (other_case_cd(path, env) == 0)
		return (0);
	tmp = ft_strjoin_free(ft_strjoin_free(getcwd(NULL, 0), "/"), path);
	if (chdir(tmp) != 0)
	{
		free(tmp);
		return (error_cd(path, env, 0));
	}
	if (mode)
		free(path);
	return (0);
}
