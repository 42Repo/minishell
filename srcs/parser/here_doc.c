/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:30:08 by asuc              #+#    #+#             */
/*   Updated: 2024/05/08 16:41:06 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	random_init(t_command *command)
{
	int			urandom_fd;
	int			bytes_read;
	char		buffer[2];
	int			i;

	i = 1;
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		return ;
	while (i < 20)
	{
		bytes_read = read(urandom_fd, buffer, 1);
		if (bytes_read == -1)
			return ;
		while (ft_isalnum(buffer[0]) == 0)
			buffer[0] = (buffer[0] % 93) + 33;
		command->random_name[i] = buffer[0];
		i++;
	}
	command->random_name[0] = '.';
	command->random_name[i] = '\0';
	close(urandom_fd);
}

static int	test_open(t_command *command)
{
	if (access(command->random_name, F_OK) == -1)
	{
		perror("Error tmp file");
		return (-1);
	}
	if (access(command->random_name, W_OK) == -1)
	{
		perror("Error tmp file");
		return (-1);
	}
	return (0);
}

void	read_heredoc(int fd, char *eof, t_command *command)
{
	char	*line;

	line = readline("> ");
	while (line && ft_strcmp(line, eof) != 0)
	{
		if (test_open(command) == -1)
			return ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = NULL;
		line = readline("> ");
	}
}

// // quitte le heredoc et revient au prompt
// void	sig_heredoc(int sig)
// {
	

void	heredoc(char *eof, t_data *data, t_command *command)
{
	int		fd;

	(void)data;
	if (eof == NULL)
		return ;
	if (command->fd_in != 0)
		close(command->fd_in);
	if (command->random_name[0] == '\0')
		random_init(command);
	fd = open(command->random_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", command->random_name, strerror(errno));
		eof = NULL;
		return ;
	}
	signal(SIGINT, SIG_IGN);
	read_heredoc(fd, eof, command);
	close(fd);
	command->fd_in = open(command->random_name, O_RDONLY);
}
