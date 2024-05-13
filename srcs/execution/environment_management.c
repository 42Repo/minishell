/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:43:50 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 18:47:56 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
