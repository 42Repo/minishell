/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:16:17 by asuc              #+#    #+#             */
/*   Updated: 2024/04/10 17:51:03 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execve_error(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (126);
}

char	**env_to_tab(t_env *env)
{
	t_env	*tmp;
	int		i;
	char	**envp;

	tmp = env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = ft_calloc(sizeof(char *), (i + 1));
	if (envp == NULL)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		envp[i] = ft_strjoin(tmp->name, "=");
		envp[i] = ft_strjoin_free(envp[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*find_cmd_path(char *cmd, char *path_env)
{
	char	**path_tab;
	int		i;
	char	*path;

	path_tab = ft_split(path_env, ':');
	if (!path_tab)
		return (NULL);
	i = 0;
	while (path_tab[i])
	{
		path = ft_strjoin(path_tab[i], "/");
		path = ft_strjoin_free(path, cmd);
		if (!access(path, F_OK))
		{
			if (!access(path, X_OK))
			{
				free_tab(path_tab);
				return (path);
			}
		}
		free(path);
		i++;
	}
	free_tab(path_tab);
	return (ft_strdup(cmd));
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

int	check_exec_command(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
	{
		if (errno == ENOENT)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": command not found\n", 2);
			return (127);
		}
		else
			return (execve_error(path));
	}
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (126);
	}
	if (!(buf.st_mode & S_IXUSR))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
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
