/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:18:39 by asuc              #+#    #+#             */
/*   Updated: 2024/05/16 17:43:38 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	sig_child_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "^C", 3);
		rl_replace_line("", 0);
		signal_received = SIGINT;
		close(0);
	}
}

void	sig_heredoc_handler(int sig)
{
	(void)sig;
}

void	set_signals(void (*handler))
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
}
