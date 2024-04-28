/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:16:17 by asuc              #+#    #+#             */
/*   Updated: 2024/04/28 21:47:58 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	put_error(char *str1, char *str2, char *str3)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd(str3, 2);
}

int	check_exec_command(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
	{
		if (errno == ENOENT)
		{
			put_error("minishell: ", path, ": No such file or directory\n");
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

int	execve_path_env(char *cmd, char **args, t_env *env, t_data *data)
{
	char	**envp;
	char	*path;

	if (!cmd || !(*args) || !args || !env || !data)
		return (1);
	envp = env_to_tab(env);
	path = get_path(env);
	if (!path || !envp)
		return (1);
	path = find_cmd_path(cmd, path);
	g_return_code = check_exec_command(path);
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
