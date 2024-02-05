/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:49:35 by asuc              #+#    #+#             */
/*   Updated: 2024/02/05 19:30:33 by asuc             ###   ########.fr       */
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
		new->value = ft_strchr(new->name, '=') + 1;
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

char	*get_env_value(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	while (env)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name)) == 0)
			return (env->value);
		env = env->next;
	}
	env = tmp;
	return (NULL);
}