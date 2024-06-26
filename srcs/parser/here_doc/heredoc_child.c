/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:34:54 by asuc              #+#    #+#             */
/*   Updated: 2024/06/01 15:45:22 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	close_other_fd_heredoc(t_command *command, int fd)
{
	while (command)
	{
		if (command->fd_heredoc > 2)
		{
			close(command->fd_heredoc);
			command->fd_heredoc = -1;
		}
		if (command->fd_in > 2)
		{
			close(command->fd_in);
			command->fd_in = 0;
		}
		if (command->fd_out > 2)
		{
			close(command->fd_out);
			command->fd_out = 1;
		}
		command = command->next;
	}
	close(fd);
}

void	handle_eof(char *eof, int fd, t_command *command, t_data *data)
{
	ft_putstr_fd("minishell: warning: here-document delimited \
by end-of-file (wanted `", 1);
	ft_putstr_fd(eof, 1);
	ft_putstr_fd("')\n", 1);
	data->g_return_code = 1;
	free(eof);
	close_other_fd_heredoc(data->command_top, fd);
	ft_exit(command, data, ft_strdup(""), 1);
}

void	heredoc_signals(t_data *data)
{
	signal(SIGINT, sig_child_handler);
	signal(SIGQUIT, sig_child_handler);
	data->g_return_code = 0;
}

void	handle_child_process(int fd, char *eof, t_command *command,
		t_data *data)
{
	char	*line;

	heredoc_signals(data);
	line = readline("> ");
	if (line == NULL && g_signal_received != 2)
		handle_eof(eof, fd, command, data);
	while (line && ft_strcmp(line, eof) != 0 && g_signal_received != 2)
	{
		line = expand_heredoc(line, data);
		if (test_open(command) == -1)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = NULL;
		line = readline("> ");
		if (line == NULL && g_signal_received != 2)
			handle_eof(eof, fd, command, data);
	}
	free(line);
	close_other_fd_heredoc(data->command_top, fd);
	free(eof);
	ft_exit(command, data, ft_strdup(""), 1);
	exit(data->g_return_code);
}
