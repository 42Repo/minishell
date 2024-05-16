/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:31:29 by asuc              #+#    #+#             */
/*   Updated: 2024/02/07 22:03:26 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	delete_env(t_env *env, char *name)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = env;
	prev = NULL;
	while (tmp)
	{
		if (tmp->name && !ft_strcmp(tmp->name, name))
		{
			if (prev)
				prev->next = tmp->next;
			else
				env = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	ft_unset(t_env *env, t_command *command, t_data *data)
{
	int		i;
	char	*line;

	i = 1;
	data->g_return_code = 0;
	while (command->args[i])
	{
		line = command->args[i];
		delete_env(env, line);
		i++;
	}
}
