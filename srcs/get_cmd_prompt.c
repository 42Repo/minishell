/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 20:18:51 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 19:28:00 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_free(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

static char	*get_cwd(t_env *env)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
	{
		perror("getcwd failed");
		return (NULL);
	}
	if (get_env_value_string(env, "HOME") != NULL
		&& ft_strncmp(tmp, get_env_value_string(env, "HOME"),
			ft_strlen(get_env_value_string(env, "HOME"))) == 0)
	{
		tmp = ft_strjoin_free(ft_strdup("~"),
				tmp + ft_strlen(get_env_value_string(env, "HOME")));
		if (tmp == NULL)
			return (NULL);
	}
	return (tmp);
}

static int	set_prompt_top(t_data *data, char *hostname, t_env *env)
{
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, "@");
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, hostname);
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, "\033[0m:");
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, "\033[1;34m");
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, get_cwd(env));
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, "\033[0m$ ");
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	return (EXIT_SUCCESS);
}

int	get_cmd_prompt(t_data *data, t_env *env)
{
	int		fd;
	ssize_t	bytes_read;
	char	hostname[1024];

	data->cmd_prompt = ft_strjoin_free(ft_strdup("\033[1;32m"),
			get_env_value_string(env, "USER"));
	if (data->cmd_prompt == NULL)
		data->cmd_prompt = ft_strdup("minishell");
	fd = open("/etc/hostname", O_RDONLY);
	if (fd == -1)
		return (error_free("open /etc/hostname"));
	bytes_read = read(fd, hostname, 1024 - 1);
	if (bytes_read == -1)
	{
		perror("read /etc/hostname");
		close(fd);
		return (EXIT_FAILURE);
	}
	hostname[bytes_read - 1] = '\0';
	if (set_prompt_top(data, hostname, env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}
