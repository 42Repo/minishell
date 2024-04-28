/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:30:08 by asuc              #+#    #+#             */
/*   Updated: 2024/04/28 17:28:15 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	random_init(t_data *data)
{
	int			urandom_fd;
	int			bytes_read;
	char		buffer[2];
	t_command	*command;
	int			i;

	i = 1;
	command = data->command_top;
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

void	heredoc(char *eof, t_data *data, t_command *command)
{
	char	*line;
	int		fd;

	if (command->cmd[0] == '\0' || command->cmd[0] == '\n' || command->cmd[0] == ' ')
	{
		printf("minishell: %s: here-document delimited by end-of-file (wanted `%s')\n", eof, eof);
		command->cmd = NULL;
		return ;
	}
	if (command->random_name[0] == '\0')
		random_init(data);
	fd = open(command->random_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", command->random_name, strerror(errno));
		command->cmd = NULL;
		return ;
	}
	line = readline("> ");
	while (line && ft_strcmp(line, command->random_name) != 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(fd);
	printf("pranked ca marche pas\n");
}