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

void	ft_echo(t_data *data)
{
	int	i;
	int	newline;

	g_return_code = 0;
	newline = 1;
	i = 1;
	if (data->command_top->args[i]
		&& ft_strcmp(data->command_top->args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (data->command_top->args[i])
	{
		ft_putstr_fd(data->command_top->args[i], 1);
		if (data->command_top->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
}
