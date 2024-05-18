/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:52 by asuc              #+#    #+#             */
/*   Updated: 2024/05/18 16:47:03 by mbuchs           ###   ########.fr       */
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

int	check_dir(char *file, t_command *command, t_data *data)
{
	int			i;
	char		*dir;
	struct stat	sb;

	i = 0;
	stat(file, &sb);
	if (check_permission(file, command, data, sb))
		return (check_permission(file, command, data, sb));
	if (!ft_strrchr(file, '/'))
		return (0);
	while (&file[i] != ft_strrchr(file, '/'))
		i++;
	dir = ft_strndup(&file[0], i);
	stat(dir, &sb);
	if (access(dir, F_OK))
		put_error("minishell: ", dir, ": No such file or directory\n");
	if (access(dir, F_OK))
		return (error_output(dir, command, 1, data));
	if (!(S_ISDIR(sb.st_mode)))
		put_error("minishell: ", dir, ": Is a directory\n");
	if (!(S_ISDIR(sb.st_mode)))
		return (error_output(dir, command, 1, data));
	free(dir);
	return (0);
}
