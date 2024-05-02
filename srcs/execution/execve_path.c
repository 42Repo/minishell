/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:16:17 by asuc              #+#    #+#             */
/*   Updated: 2024/05/01 21:19:49 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	put_error(char *str1, char *str2, char *str3)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd(str3, 2);
}

int	check_exec_command_path(char *path)
{
	struct stat	buf;
	int test;

	test = stat(path, &buf);
	if (test == -1)
	{
		if (errno == ENOENT)
		{
			put_error("", path, ": No such file or directory\n");
			return (127);
		}
		else
			return (execve_error(path));
	}
	if (S_ISDIR(buf.st_mode))
	{
		put_error("minishell: ", path, ": Is a directory\n");
		return (126);
	}
	if (!(buf.st_mode & S_IXUSR))
	{
		put_error("minishell: ", path, ": Permission denied\n");
		return (126);
	}
	return (127);
}

int	check_exec_command(char *path)
{
	struct stat	buf;
	int test;

	test = stat(path, &buf);
	if (test == -1)
	{
		if (errno == ENOENT)
		{
			put_error("minishell: ", path, ": command not found\n");
			return (127);
		}
		else
			return (execve_error(path));
	}
	if (S_ISDIR(buf.st_mode))
	{
		put_error("minishell: ", path, ": Is a directory\n");
		return (126);
	}
	if (!(buf.st_mode & S_IXUSR))
	{
		put_error("minishell: ", path, ": Permission denied\n");
		return (126);
	}
	return (0);
}

int	has_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	execve_path_env(char *cmd, char **args, t_env *env, t_data *data)
{
	char	**envp;
	char	*path;

	if (!cmd || !(*args) || !args || !env || !data)
		return (1);
	g_return_code = 0;
	envp = env_to_tab(env);
	path = get_path(env);
	if (!path || !envp)
		return (1);
	if (has_slash(cmd) != 1)
	{
		path = find_cmd_path(cmd, path);
		if (path == NULL)
		{
			if (errno == EACCES)
			{
				put_error("minishell: ", cmd, ": Permission denied\n");
				g_return_code = 126;
			}
			else
			{
				put_error("minishell: ", cmd, ": command not found\n");
				g_return_code = 127;
			}
		}
		// g_return_code = check_exec_command(path);
	}
	else
	{
		path = ft_strdup(cmd);
		g_return_code = check_exec_command_path(path);
	}
	if (g_return_code != 0)
	{
		free_tab(envp);
		free(path);
		free_token_lst(data);
		free_env(env);
		free_command(data);
		if (data->cmd_prompt)
			free(data->cmd_prompt);
		free(data);
		return (g_return_code);
	}
	close(data->fd_in);
	close(data->fd_out);
	execve(path, args, envp);
	perror("minishell");
	free_tab(envp);
	free(path);
	return (127);
}

char	*get_path(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PATH"))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
