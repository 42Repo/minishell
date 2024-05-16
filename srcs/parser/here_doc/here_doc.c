/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:30:08 by asuc              #+#    #+#             */
/*   Updated: 2024/05/16 17:44:13 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	sigint_here_doc(t_command *command)
{
	command->random_name[0] = '\0';
	if (command->fd_heredoc > 2)
		close(command->fd_heredoc);
	command->fd_heredoc = 0;
	return (-1);
}

void	read_heredoc(int fd, char *eof, t_command *command, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
		handle_child_process(fd, eof, command, data);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->g_return_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		data->g_return_code = 128 + WTERMSIG(status);
}

int	open_heredoc_file(t_command *command)
{
	int	fd;

	fd = open(command->random_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", command->random_name, strerror(errno));
	}
	return (fd);
}

int	prepare_heredoc(t_command *command)
{
	if (command->fd_heredoc > 2)
		close(command->fd_heredoc);
	if (command->random_name[0] == '\0')
		random_init(command);
	set_signals(&sig_heredoc_handler);
	return (open_heredoc_file(command));
}

int	heredoc(char *eof, t_data *data, t_command *command)
{
	int		fd;
	int		fd2;

	eof = remove_quotes(eof, 0);
	if (eof == NULL)
		return (0);
	fd = prepare_heredoc(command);
	if (fd == -1)
	{
		free(eof);
		return (-1);
	}
	read_heredoc(fd, eof, command, data);
	free(eof);
	fd2 = open(command->random_name, O_RDONLY);
	unlink(command->random_name);
	if (fd > 2)
		close(fd);
	tcsetattr(0, TCSANOW, data->term);
	set_signals(&sig_handler);
	command->fd_heredoc = fd2;
	if (data->g_return_code == 130)
		return (sigint_here_doc(command));
	return (0);
}
