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

static int	do_tilde(t_env *env, t_data *data, t_command *command)
{
	char	*tmp;

	tmp = ft_strjoin(get_env_value_string(env, "HOME"), command->args[1] + 1);
	if (!tmp || chdir(tmp) != 0)
	{
		errno = 14;
		free(tmp);
		return (error_cd(tmp, env, 1, data));
	}
	free(tmp);
	return (0);
}

static int	other_case_cd(t_command *command, t_env *env, t_data *data)
{
	char	*tmp;

	tmp = NULL;
	if (command->args[1] == NULL)
	{
		tmp = get_env_value_string(env, "HOME");
		if (!tmp || chdir(tmp) != 0)
		{
			errno = 14;
			free(tmp);
			return (error_cd(get_env_value_string(env, "HOME"), env, 0, data));
		}
		return (0);
	}
	if (command->args[1][0] == '/')
	{
		if (chdir(command->args[1]) != 0)
			return (error_cd(command->args[1], env, 0, data));
		return (0);
	}
	if (command->args[1][0] == '~')
		return (do_tilde(env, data, command));
	return (1);
}

static int	old_cd(t_env *env, t_data *data)
{
	char	*tmp;

	if (get_env_value_string(env, "OLDPWD") == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		data->g_return_code = 1;
		return (-1);
	}
	tmp = getcwd(NULL, 0);
	if (chdir(get_env_value_string(env, "OLDPWD")) != 0)
	{
		update_env(env, tmp);
		printf("%s\n", get_env_value_string(env, "OLDPWD"));
		data->g_return_code = 1;
		return (0);
	}
	printf("%s\n", get_env_value_string(env, "OLDPWD"));
	update_env(env, tmp);
	return (0);
}

static void	set_new_pwd(t_env *env)
{
	char	*tmp;
	char	*tmp2;

	tmp = getcwd(NULL, 0);
	tmp2 = ft_strjoin("PWD=", tmp);
	free(tmp);
	ft_export_single_arg(env, tmp2);
	free(tmp2);
}

int	ft_cd(t_command *command, t_env *env, t_data *data)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	tmp2 = NULL;
	data->g_return_code = 0;
	if (command == NULL || (command->args[1] != NULL
			&& command->args[2] != NULL))
		return (error_to_many_args(env, data));
	if (command->args[1] != NULL
		&& ft_strcmp(command->args[1], "-") == 0)
		return (old_cd(env, data));
	tmp2 = getcwd(NULL, 0);
	if (tmp2)
		tmp = ft_strjoin("OLDPWD=", tmp2);
	ft_export_single_arg(env, tmp);
	free(tmp2);
	free(tmp);
	if (other_case_cd(command, env, data) == 0)
		return (0);
	if (chdir(command->args[1]) != 0)
		return (error_cd(command->args[1], env, 0, data));
	set_new_pwd(env);
	return (0);
}
