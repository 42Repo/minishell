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

void	ft_echo(char *line, int mode)
{
	if (line)
	{
		while (*line)
		{
			write(1, line, 1);
			line++;
		}
	}
	if (!mode)
		write(1, "\n", 1);
}
