/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:42:49 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 23:43:11 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	useless(int sig)
{
	(void)sig;
}

void	setup_signals(void *handler)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
}
