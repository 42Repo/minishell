/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:34:54 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 22:09:23 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	handle_eof(char *eof, int fd, t_command *command, t_data *data)
{
	ft_putstr_fd("minishell: warning: here-document delimited \
by end-of-file (wanted `", 1);
	ft_putstr_fd(eof, 1);
	ft_putstr_fd("')\n", 1);
	g_return_code = 1;
	close(fd);
	free(eof);
	ft_exit(command, data, "", 1);
}

void	handle_child_process(int fd, char *eof,
			t_command *command, t_data *data)
{
	char	*line;

	signal(SIGINT, sig_child_handler);
	signal(SIGQUIT, sig_child_handler);
	g_return_code = 0;
	line = expand_heredoc(readline("> "), data);
	if (line == NULL && g_return_code != 130)
		handle_eof(eof, fd, command, data);
	while (line && ft_strcmp(line, eof) != 0 && g_return_code != 130)
	{
		if (test_open(command) == -1)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = NULL;
		line = expand_heredoc(readline("> "), data);
		if (line == NULL && g_return_code != 130)
			handle_eof(eof, fd, command, data);
	}
	free(line);
	close(fd);
	free(eof);
	ft_exit(command, data, "", 1);
	exit(g_return_code);
}
