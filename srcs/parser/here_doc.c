/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:30:08 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 17:31:27 by mbuchs           ###   ########.fr       */
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

void	sig_child_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		g_return_code = 130;
		close(0);
	}
}

void	sig_heredoc_handler(int sig)
{
	(void)sig;
}

char	*expand_heredoc(char *str, t_data *data)
{
	int		i;
	int		j;
	char	**tab;

	// if (!str)
	// 	return (NULL);
	i = 0;
	j = 0;
	tab = ft_calloc(sizeof(char *), 2);
	while (str[i])
	{
		while (check_envar(str, i, 0))
		{
			i++;
			j++;
		}
		if (j > 0)
		{
			if (!tab[0])
				tab[0] = ft_strndup(&str[i-j], j);
			else
				tab = join_tab(tab, ft_strndup(&str[i-j], j));
			j = 0;
			if (!str[i])
				j = -1;
		}
		if (j < 0)
			break ;
		if (str[i] == '$')
		{
			tab = join_tab(tab, get_envar(&str[i],
						get_envar_len(&str[i]), data));
			i += get_envar_len(&str[i]);
			j = 0;
		}
	}
	return (join_replaced(tab));
}

void	read_heredoc(int fd, char *eof, t_command *command, t_data *data)
{
	char	*line;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		signal(SIGINT, sig_child_handler);
		signal(SIGQUIT, sig_child_handler);
		rl_catch_signals = 0;
		line = expand_heredoc(readline("> "), data);
		if (line == NULL && g_return_code != 130)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 1);
			ft_putstr_fd(eof, 1);
			ft_putstr_fd("')\n", 1);
			close(fd);
			ft_exit(command, data, data->env, "");
			exit(130);
		}
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
			{
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 1);
				ft_putstr_fd(eof, 1);
				ft_putstr_fd("')\n", 1);
				close(fd);
				exit(130);
			}
		}
		free(line);
		close(fd);
		free(eof);
		ft_exit(command, data, data->env, "");
		exit(g_return_code);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		g_return_code = 128 + WTERMSIG(status);
	if (WIFEXITED(status))
		g_return_code = WEXITSTATUS(status);
}

void	heredoc(char *eof, t_data *data, t_command *command)
{
	int		fd;
	int		fd2;

	(void)data;
	eof = remove_quotes(eof, 0);
	if (eof == NULL)
		return ;
	if (command->fd_heredoc > 2)
		close(command->fd_heredoc);
	if (command->random_name[0] == '\0')
		random_init(command);
	signal(SIGINT, sig_heredoc_handler);
	signal(SIGQUIT, sig_heredoc_handler);
	fd = open(command->random_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", command->random_name, strerror(errno));
		eof = NULL;
		return ;
	}
	read_heredoc(fd, eof, command, data);
	free(eof);
	fd2 = open(command->random_name, O_RDONLY);
	unlink(command->random_name);
	if (fd > 2)
		close(fd);
	tcsetattr(0, TCSANOW, data->term);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	command->fd_heredoc = fd2; // TODO - Fix this
	if (g_return_code == 130)
	{
		command->random_name[0] = '\0';
		if (command->fd_heredoc > 2)
			close(command->fd_heredoc);
		command->fd_heredoc = -1;
	}
}
