/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:31:29 by asuc              #+#    #+#             */
/*   Updated: 2024/02/07 00:14:56 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	delete_env(t_env *env, char *name)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	printf("name = %s\n", name);
	while (tmp->next)
	{
		if (!ft_strcmp(tmp->next->name, name))
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2->name);
			free(tmp2->value);
			free(tmp2);
			return ;
		}
		tmp = tmp->next;
	}
}

void	ft_unset(t_env *env, char *line)
{
	t_env	*tmp;

	tmp = get_env_value_ptr_by_name(env, line);
	if (tmp)
		delete_env(env, line);
}
