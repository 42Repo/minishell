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

#include "../includes/minishell.h"

void	ft_echo(t_data *data)
{
	int	i;
	int	mode;

	mode = 0;
	if (ft_tablen(data->command_top->args) > 1
		&& !ft_strcmp(data->command_top->args[1], "-n"))
	{
		mode = 1;
		i = 2;
	}
	else
		i = 1;
	if (ft_tablen(data->command_top->args) > 1)
	{
		while (data->command_top->args[i])
		{
			printf("%s", data->command_top->args[i]);
			if (data->command_top->args[i + 1])
				printf(" ");
			i++;
		}
	}
	if (!mode)
		printf("\n");
}
