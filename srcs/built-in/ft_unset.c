/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:07:20 by asuc              #+#    #+#             */
/*   Updated: 2024/05/20 20:07:20 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	create_env(t_data *data)
{
	data->env = (t_env *)malloc(sizeof(t_env));
	data->env->name = NULL;
	data->env->value = NULL;
	data->env->next = NULL;
}

static void	delete_env(t_data *data, char *name)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = data->env;
	prev = NULL;
	while (tmp)
	{
		if (tmp->name && !ft_strcmp(tmp->name, name))
		{
			if (prev)
				prev->next = tmp->next;
			else
				data->env = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	if (!data->env)
		create_env(data);
}

void	ft_unset(t_command *command, t_data *data)
{
	int		i;
	char	*line;

	i = 1;
	data->g_return_code = 0;
	while (command->args[i])
	{
		line = command->args[i];
		delete_env(data, line);
		i++;
	}
}
