/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:18:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/13 23:12:28 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

void	free_command_loop(t_command **selected, t_command **old)
{
	if ((*selected)->cmd)
	{
		free((*selected)->cmd);
		(*selected)->cmd = NULL;
	}
	if ((*selected)->args)
	{
		free_tab((*selected)->args);
		(*selected)->args = NULL;
	}
	if ((*selected)->fd_in > 2)
		close((*selected)->fd_in);
	if ((*selected)->fd_out > 2)
		close((*selected)->fd_out);
	(*old) = (*selected);
	(*selected) = (*selected)->next;
	if ((*old))
	{
		free((*old));
		(*old) = NULL;
	}
}

void	free_command(t_data *data)
{
	t_command	*selected;
	t_command	*old;

	old = NULL;
	selected = data->command_top;
	while (selected)
	{
		free_command_loop(&selected, &old);
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
	command->fd_heredoc = -1;
	command->random_name[0] = '\0';
	command->pid = 0;
	return (command);
}

void	get_redir(t_token *selected, t_command *command)
{
	if (selected->type == REDIR)
	{
		if (selected->next && selected->next->type == WORD)
		{
			if (ft_strlen(selected->value) == 2 && selected->value[1] == '>')
				select_output(selected->next->value, 2, command);
			else if (selected->value[0] == '>')
				select_output(selected->next->value, 1, command);
			else if (ft_strcmp(selected->value, "<<") == 0)
				return ;
			else if (selected->value[0] == '<')
				select_input(selected->next->value, command);
		}
		else
		{
			ft_putstr_fd("minishell: syntax error near unexpected \
'HAHAHA'\n", 2);
			if (command->cmd)
				free (command->cmd);
			command->cmd = NULL;
		}
	}
}
