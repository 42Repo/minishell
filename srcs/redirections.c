/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:41:19 by asuc              #+#    #+#             */
/*   Updated: 2024/05/20 21:34:12 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_loop(t_data *data)
{
	setup_signals(&sig_handler);
	if (data->prompt_top)
		free_token_lst(data);
	if (data->command_top)
		free_command(data);
	get_cmd_prompt(data, data->env);
}

void	setup_redirections(int *input_fd, int *output_fd)
{
	if ((*input_fd) != STDIN_FILENO && (*input_fd) > 2)
	{
		dup2((*input_fd), STDIN_FILENO);
		close((*input_fd));
	}
	if ((*output_fd) != STDOUT_FILENO && (*output_fd) > 2)
	{
		dup2((*output_fd), STDOUT_FILENO);
		close((*output_fd));
	}
}

void	choose_case(t_data *data)
{
	t_command	*command;
	int			prev_fd;

	setup_signals(&useless);
	command = data->command_top;
	prev_fd = STDIN_FILENO;
	if (command && command->next == NULL)
	{
		prev_fd = command->fd_in;
		execute_command(command, data, prev_fd, command->fd_out);
		return ;
	}
	while (command)
	{
		setup_command_execution(command, &prev_fd);
		execute_pipes(command, data, prev_fd);
		close_and_dup_pipes(command, &prev_fd);
		command = command->next;
	}
	wait_for_commands(command, data);
}
