/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:16:17 by asuc              #+#    #+#             */
/*   Updated: 2024/02/24 00:17:25 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		execve_error(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int		execve_path_env(char *path, char **args, t_env *env)
{
	char	**envp;
	int		i;

	envp = env_to_tab(env);
	if (!envp)
		return (1);
	i = execve_path(path, args, envp);
	free_tab(envp);
	return (i);
}

char *get_path(t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PATH"))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}


