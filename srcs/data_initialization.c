/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_initialization.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:27:42 by asuc              #+#    #+#             */
/*   Updated: 2024/05/16 17:49:11 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_data(t_data *data)
{
	rl_catch_signals = 0;
	data->prompt_top = NULL;
	data->cmd_prompt = NULL;
	data->quote_state = 0;
	data->command_top = NULL;
	data->fd_out = dup(STDOUT_FILENO);
	data->fd_in = dup(STDIN_FILENO);
	data->term = ft_calloc(sizeof(struct termios), 1);
	tcgetattr(STDIN_FILENO, data->term);
	data->g_return_code = 0;
}
