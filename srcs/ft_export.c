/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:31:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 18:52:40 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*get_env_value(t_env *env, char *name)
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

char	*ft_export(t_env *env, char *name)
{
	t_env	*tmp;

	name += 7;
	tmp = get_env_value(env, name);
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
		tmp->next = malloc(sizeof(t_env));
		if (!tmp->next)
			return (NULL);
		tmp->next->name = ft_strndup(name, ft_strchr(name, '=') - name);
		tmp->next->value = ft_strdup(ft_strchr(name, '=') + 1);
		tmp->next->next = NULL;
		return (tmp->next->value);
	}
	env = tmp;
	return (NULL);
}
