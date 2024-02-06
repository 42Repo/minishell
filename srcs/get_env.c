/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 18:49:35 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 18:52:31 by asuc             ###   ########.fr       */
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
		tmp->next = malloc(sizeof(t_env));
		if (!tmp->next)
			return ;
		tmp = tmp->next;
		tmp->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = NULL;
	}
}


