/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:52 by asuc              #+#    #+#             */
/*   Updated: 2024/05/19 00:20:16 by asuc             ###   ########.fr       */
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

void	set_fd_in(t_command *cmd, t_token *selected)
{
	t_token	*last_redir;

	last_redir = NULL;
	if ((cmd->fd_in == -1 || cmd->fd_out == -1) && cmd->fd_heredoc != -1)
		close(cmd->fd_heredoc);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		return ;
	while (selected)
	{
		if (selected->type == REDIR && selected->value[0] == '<')
			last_redir = selected;
		if (selected->type == PIPE)
			do_redir(&cmd, &last_redir);
		selected = selected->next;
	}
	if (last_redir && ft_strcmp(last_redir->value, "<<") == 0)
	{
		if (cmd->fd_in > 2)
			close(cmd->fd_in);
		cmd->fd_in = cmd->fd_heredoc;
		cmd->fd_heredoc = -1;
	}
	else if (cmd->fd_heredoc != -1 && cmd->fd_heredoc != cmd->fd_in)
		close(cmd->fd_heredoc);
	cmd->fd_heredoc = -1;
}

int	check_dir(char *file, t_command *command, t_data *data)
{
	int			i;
	char		*dir;
	struct stat	sb;

	i = 0;
	stat(file, &sb);
	i = check_permission(file, command, data, sb);
	if (i)
		return (i);
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

void	is_ambiguous(t_token **selected, t_token *previous, char *str)
{
	if (ft_strlen(str) == 0)
		put_error("minishell: ",
			(*selected)->value, ": ambiguous redirection\n");
	else
		put_error("minishell: ", str, ": ambiguous redirection\n");
	free(str);
	while ((*selected)->next->type != END
		&& (*selected)->next->type != PIPE)
		(*selected) = (*selected)->next;
	free(previous->value);
	previous->value = NULL;
	return ;
}
