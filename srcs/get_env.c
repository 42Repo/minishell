/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:49:35 by asuc              #+#    #+#             */
/*   Updated: 2024/05/04 14:59:10 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_shlvl(t_env *env)
{
	int	i;

	while (env)
	{
		if (ft_strcmp(env->name, "SHLVL") == 0)
		{
			i = ft_atoi(env->value);
			i++;
			free(env->value);
			env->value = ft_itoa(i);
			return ;
		}
		env = env->next;
	}
}

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
	add_shlvl(env);
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
	tmp_name = ft_strchr(name, '=');
	if (!tmp_name)
		tmp_name = ft_strdup(name);
	else
	{
		if (*(tmp_name - 1) == '+')
			tmp_name = ft_strndup(name, tmp_name - name - 1);
		else
			tmp_name = ft_strndup(name, tmp_name - name);
	}
	while (tmp)
	{
		if (ft_strcmp(tmp->name, tmp_name) == 0)
		{
			free(tmp_name);
			return (tmp);
		}
		tmp = tmp->next;
	}
	free(tmp_name);
	return (NULL);
}
