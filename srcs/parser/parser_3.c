/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:52 by asuc              #+#    #+#             */
/*   Updated: 2024/05/15 14:48:48 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	do_redir(t_command **command, t_token **last_redir)
{
	if ((*last_redir) && ft_strcmp((*last_redir)->value, "<<") == 0)
	{
		if ((*command)->fd_in > 2)
			close((*command)->fd_in);
		(*command)->fd_in = (*command)->fd_heredoc;
		(*last_redir) = NULL;
	}
	else
	{
		if ((*command)->fd_heredoc > 2)
			close ((*command)->fd_heredoc);
		(*command)->fd_heredoc = -1;
	}
	(*command) = (*command)->next;
}

void	set_fd_in(t_command *command, t_token *selected)
{
	t_token	*last_redir;

	last_redir = NULL;
	while (selected)
	{
		if (selected->type == REDIR && selected->value[0] == '<')
			last_redir = selected;
		if (selected->type == PIPE)
			do_redir(&command, &last_redir);
		selected = selected->next;
	}
	if (last_redir && ft_strcmp(last_redir->value, "<<") == 0)
	{
		if (command->fd_in > 2)
			close(command->fd_in);
		command->fd_in = command->fd_heredoc;
		command->fd_heredoc = -1;
	}
	else if (command->fd_heredoc != -1 && command->fd_heredoc != command->fd_in)
		close(command->fd_heredoc);
	command->fd_heredoc = -1;
}
