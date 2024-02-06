/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:31:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 01:05:18 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	i;

	new = malloc(n + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*ft_export(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = get_env_value(env, name);
	printf("name = %s\n", name);
	printf("tmp->name = %s\n", tmp->name);
	printf("tmp->value = %s\n", tmp->value);
	if (tmp)
	{
		free(tmp->value);
		tmp->value = ft_strchr(name, '=') + 1;
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
		tmp->next->value = ft_strchr(name, '=') + 1;
		tmp->next->next = NULL;
		return (tmp->next->value);
	}
	env = tmp;
	return (NULL);
}
