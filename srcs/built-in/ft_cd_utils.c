/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:54:05 by asuc              #+#    #+#             */
/*   Updated: 2024/05/20 21:18:00 by asuc             ###   ########.fr       */
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

int	error_to_many_args(t_env *env, t_data *data)
{
	put_program_name(env);
	ft_putstr_fd(": cd: ", 2);
	ft_putstr_fd("too many arguments\n", 2);
	data->g_return_code = 1;
	return (-1);
}

int	error_cd(char *path, t_env *env, int mode, t_data *data)
{
	put_program_name(env);
	ft_putstr_fd(": cd: ", 2);
	if (errno == 14)
	{
		ft_putstr_fd("HOME not set\n", 2);
		data->g_return_code = 1;
		return (0);
	}
	if (path != NULL)
		ft_putstr_fd(path, 2);
	if (mode == 1)
		free(path);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	data->g_return_code = 1;
	return (0);
}

void	update_env(t_env *env, char *new_value)
{
	char	*tmp2;

	tmp2 = ft_strjoin("OLDPWD=", new_value);
	free(new_value);
	ft_export_single_arg(env, tmp2);
	free(tmp2);
}

char	*ft_export_single_arg(t_env *env, char *name)
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
		while (!tmp->name && tmp->next)
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
