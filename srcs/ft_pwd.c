/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:40:59 by asuc              #+#    #+#             */
/*   Updated: 2024/02/07 22:35:05 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	put_program_name(t_env *env)
{
	char		*tmp;
	const char	*tmp_error = "minishell";
	int			i;

	tmp = get_env_value_string(env, "_");
	if (tmp == NULL)
	{
		tmp = (char *)tmp_error;
	}
	i = ft_strlen(tmp) - 1;
	while (i >= 0 && tmp[i] != '/')
		i--;
	ft_putstr_fd(tmp + i + 1, 2);
	return (0);
}

void	ft_pwd(t_env *env)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		put_program_name(env);
		ft_putstr_fd(": pwd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	else
	{
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
		free(path);
	}
}
