/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:05:01 by asuc              #+#    #+#             */
/*   Updated: 2024/05/09 15:05:01 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		g_return_code = 1;
		return (-1);
	}
	if (path != NULL)
		ft_putstr_fd(path, 2);
	if (mode == 1)
		free(path);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	g_return_code = 1;
	return (-1);
}

static int	other_case_cd(t_command *command, t_env *env)
{
	char	*tmp;

	tmp = NULL;
	if (command->args[1] == NULL)
	{
		if (chdir(get_env_value_string(env, "HOME")) != 0)
			return (error_cd(get_env_value_string(env, "HOME"), env, 0));
		return (0);
	}
	if (command->args[1][0] == '/')
	{
		if (chdir(command->args[1]) != 0)
			return (error_cd(command->args[1], env, 0));
		return (0);
	}
	if (command->args[1][0] == '~')
	{
		tmp = ft_strjoin(get_env_value_string(env, "HOME"),
				command->args[1] + 1);
		if (chdir(tmp) != 0)
			return (error_cd(tmp, env, 1));
		free(tmp);
		return (0);
	}
	return (1);
}

static char	*ft_export_single_arg(t_env *env, char *name)
{
	t_env	*tmp;

	if (name == NULL)
		return (NULL);
	tmp = get_env_value_ptr(env, name);
	if (tmp)
	{
		free(tmp->value);
		tmp->value = ft_strdup(ft_strchr(name, '=') + 1);
		return (tmp->value);
	}
	else
	{
		tmp = env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = ft_calloc(sizeof(t_env), 1);
		if (!tmp->next)
			return (NULL);
		tmp->next->name = ft_strndup(name, ft_strchr(name, '=') - name);
		tmp->next->value = ft_strdup(ft_strchr(name, '=') + 1);
		tmp->next->next = NULL;
		return (tmp->next->value);
	}
	return (NULL);
}

static int	old_cd(t_env *env)
{
	char	*tmp;
	char	*tmp2;

	if (get_env_value_string(env, "OLDPWD") == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		g_return_code = 1;
		return (-1);
	}
	tmp = getcwd(NULL, 0);
	if (chdir(get_env_value_string(env, "OLDPWD")) != 0)
	{
		tmp2 = (tmp);
		tmp = ft_strjoin("OLDPWD=", tmp2);
		free(tmp2);
		ft_export_single_arg(env, tmp);
		free(tmp);
		printf("%s\n", get_env_value_string(env, "OLDPWD"));
		g_return_code = 1;
		return (0);
	}
	printf("%s\n", get_env_value_string(env, "OLDPWD"));
	tmp2 = ft_strjoin("OLDPWD=", tmp);
	ft_export_single_arg(env, tmp2);
	free(tmp2);
	free(tmp);
	return (0);
}

static int	error_to_many_args(t_env *env)
{
	put_program_name(env);
	ft_putstr_fd(": cd: ", 2);
	ft_putstr_fd("too many arguments\n", 2);
	g_return_code = 1;
	return (-1);
}

static void	set_new_pwd(t_env *env)
{
	char	*tmp;
	char	*tmp2;

	tmp = getcwd(NULL, 0);
	tmp2 = ft_strjoin("PWD=", tmp);
	ft_export_single_arg(env, tmp2);
	free(tmp2);
}

int	ft_cd(t_command *command, t_env *env)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	tmp2 = NULL;
	g_return_code = 0;
	if (command == NULL || (command->args[1] != NULL
			&& command->args[2] != NULL))
		return (error_to_many_args(env));
	if (command->args[1] != NULL
		&& ft_strcmp(command->args[1], "-") == 0)
		return (old_cd(env));
	tmp2 = getcwd(NULL, 0);
	if (tmp2)
		tmp = ft_strjoin("OLDPWD=", tmp2);
	ft_export_single_arg(env, tmp);
	free(tmp2);
	free(tmp);
	if (other_case_cd(command, env) == 0)
		return (0);
	if (chdir(command->args[1]) != 0)
		return (error_cd(command->args[1], env, 0));
	set_new_pwd(env);
	return (0);
}
