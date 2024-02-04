/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 20:18:51 by asuc              #+#    #+#             */
/*   Updated: 2024/02/04 21:27:44 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_strjoin_free(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

static int	set_prompt_top(t_data *data, char *hostname)
{
	data->prompt_top = ft_strjoin_free(data->prompt_top, "@");
	if (data->prompt_top == NULL)
		return (error_strjoin_free("ft_strjoin_free failed"));
	data->prompt_top = ft_strjoin_free(data->prompt_top, hostname);
	if (data->prompt_top == NULL)
		return (error_strjoin_free("ft_strjoin_free failed"));
	data->prompt_top = ft_strjoin_free(data->prompt_top, "\033[0m:");
	if (data->prompt_top == NULL)
		return (error_strjoin_free("ft_strjoin_free failed"));
	data->prompt_top = ft_strjoin_free(data->prompt_top, "\033[1;34m");
	if (data->prompt_top == NULL)
		return (error_strjoin_free("ft_strjoin_free failed"));
	data->prompt_top = ft_strjoin_free(data->prompt_top, getcwd(NULL, 0));
	if (data->prompt_top == NULL)
		return (error_strjoin_free("ft_strjoin_free failed"));
	data->prompt_top = ft_strjoin_free(data->prompt_top, "$\033[0m ");
	if (data->prompt_top == NULL)
		return (error_strjoin_free("ft_strjoin_free failed"));
	return (EXIT_SUCCESS);
}

int	get_cmd_prompt(t_data *data)
{
	int		fd;
	ssize_t	bytes_read;
	char	hostname[1024];

	data->prompt_top = ft_strjoin_free(ft_strdup("\033[1;32m"), getenv("USER"));
	if (data->prompt_top == NULL)
		data->prompt_top = ft_strdup("minishell");
	fd = open("/etc/hostname", O_RDONLY);
	if (fd == -1)
	{
		perror("open /etc/hostname failed");
		return (EXIT_FAILURE);
	}
	bytes_read = read(fd, hostname, 1024 - 1);
	if (bytes_read == -1)
	{
		perror("read /etc/hostname failed");
		close(fd);
		return (EXIT_FAILURE);
	}
	hostname[bytes_read - 1] = '\0';
	if (set_prompt_top(data, hostname) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}