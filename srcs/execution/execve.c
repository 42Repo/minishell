/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:16:17 by asuc              #+#    #+#             */
/*   Updated: 2024/05/11 18:49:22 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

static int	check_executable(const char *path, struct stat *buf)
{
	if (!access(path, F_OK))
	{
		stat(path, buf);
		if (!access(path, X_OK) && !S_ISDIR(buf->st_mode))
			return (1);
		else
		{
			if (S_ISDIR(buf->st_mode))
				errno = EISDIR;
			else
				errno = EACCES;
			return (0);
		}
	}
	return (0);
}

char	*find_cmd_path(char *cmd, char *path_env)
{
	char		**path_tab;
	int			i;
	char		*path;
	struct stat	buf;

	buf.st_mode = 0;
	path_tab = ft_split(path_env, ':');
	if (!path_tab)
		return (NULL);
	i = 0;
	while (path_tab[i])
	{
		path = ft_strjoin(path_tab[i], "/");
		path = ft_strjoin_free(path, cmd);
		if (check_executable(path, &buf))
		{
			free_tab(path_tab);
			errno = 0;
			return (path);
		}
		free(path);
		i++;
	}
	free_tab(path_tab);
	return (NULL);
}
