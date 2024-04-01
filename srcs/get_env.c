/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:49:35 by asuc              #+#    #+#             */
/*   Updated: 2024/03/27 22:54:15 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_env(t_env *env, char **envp)
{
	int		i;
	t_env	*tmp;

	i = 0;
	if (!envp[i])
		return ;
	env->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
	env->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
	env->next = NULL;
	tmp = env;
	while (envp[++i])
	{
		tmp->next = ft_calloc(sizeof(t_env), 1);
		if (!tmp->next)
			return ;
		tmp = tmp->next;
		tmp->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = NULL;
	}
}

char	*get_env_value_string(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*get_env_value_ptr_by_name(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*get_env_value_ptr(t_env *env, char *name)
{
	t_env	*tmp;
	char	*tmp_name;

	tmp = env;
	while (tmp)
	{
		tmp_name = ft_strndup(name, ft_strchr(name, '=') - name);
		if (ft_strcmp(tmp->name, tmp_name) == 0)
		{
			free(tmp_name);
			return (tmp);
		}
		tmp = tmp->next;
		free(tmp_name);
	}
	return (NULL);
}
