/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:49:35 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 01:06:00 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_env(t_env **env, char **envp)
{
	int		i;
	t_env	*new;
	t_env	*tmp;

	i = 0;
	*env = NULL;
	while (envp[i])
	{
		new = malloc(sizeof(t_env));
		new->name = ft_strdup(envp[i]);
		new->value = ft_strdup(ft_strchr(new->name, '=') + 1);
		*(ft_strchr(new->name, '=')) = '\0';
		new->next = NULL;
		if (*env == NULL)
			*env = new;
		else
		{
			tmp = *env;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		i++;
	}
}

// ft_strcmp
static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

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

t_env	*get_env_value(t_env *env, char *name)
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
