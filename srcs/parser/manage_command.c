/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:18:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/30 15:29:26 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	free_command(t_data *data)
{
	t_command	*selected;
	t_command	*old;

	selected = data->command_top;
	while (selected)
	{
		if (selected->cmd)
		{
			free(selected->cmd);
			selected->cmd = NULL;
		}
		if (selected->args)
		{
			free_tab(selected->args);
			selected->args = NULL;
		}
		old = selected;
		selected = selected->next;
		if (old)
		{
			free(old);
			old = NULL;
		}
		data->command_top = selected;
	}
}

t_command	*init_command(void)
{
	t_command	*command;

	command = ft_calloc(sizeof(t_command), 1);
	command->cmd = NULL;
	command->args = NULL;
	command->next = NULL;
	command->fd_in = 0;
	command->fd_out = 1;
	command->random_name[0] = '\0';
	command->pid = 0;
	return (command);
}

void	remove_empty_tokens(t_data *data)
{
	t_token	*selected;
	t_token	*old;

	old = NULL;
	selected = data->prompt_top;
	while (selected)
	{
		if (selected->type == WORD && ft_strlen(selected->value) == 0)
		{
			if (old)
				old->next = selected->next;
			else
				data->prompt_top = selected->next;
			if (selected->value)
				free(selected->value);
			if (selected)
				free(selected);
			if (old)
				selected = old;
		}
		old = selected;
		selected = selected->next;
	}
}
