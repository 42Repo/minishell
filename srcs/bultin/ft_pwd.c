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

#include "../../includes/minishell.h"

static void	put_program_name(t_env *env)
{
	char		*tmp;
	const char	*tmp_error = "minishell";
	int			i;

	tmp = get_env_value_string(env, "_");
	if (tmp == NULL)
		tmp = (char *)tmp_error;
	i = ft_strlen(tmp) - 1;
	while (i >= 0 && tmp[i] != '/')
		i--;
	ft_putstr_fd(tmp + i + 1, 2);
}

void	ft_pwd(t_env *env)
{
	char	*path;

	g_return_code = 0;
	path = getcwd(NULL, 0);
	if (!path)
	{
		put_program_name(env);
		put_error(": pwd: ", strerror(errno), "\n");
		g_return_code = 1;
		return ;
	}
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
	free(path);
	g_return_code = 0;
}
