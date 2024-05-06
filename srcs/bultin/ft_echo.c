/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 00:07:39 by asuc              #+#    #+#             */
/*   Updated: 2024/02/09 23:57:07 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_arg_option(char *arg)
{
	int	i;

	if (arg && arg[0] == '-')
	{
		if (!arg[1])
			return (0);
		i = 1;
		while (arg[i] && arg[i] == 'n')
			i++;
		if (!arg[i])
			return (1);
	}
	return (0);
}

void	ft_echo(t_command *command)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	g_return_code = 0;

	while (command->args[i] && is_arg_option(command->args[i]))
		i++;
	newline = (i == 1);
	while (command->args[i])
	{
		ft_putstr_fd(command->args[i], STDOUT_FILENO);
		if (command->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
