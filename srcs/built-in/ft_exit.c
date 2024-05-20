/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/20 21:29:02 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	free_resources(t_data *data, t_env *env)
{
	free_token_lst(data);
	free_env(env);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	if (data->fd_in > 2)
		close(data->fd_in);
	if (data->fd_out > 2)
		close(data->fd_out);
	free(data->term);
	free_command(data);
	free(data);
	close(0);
	close(1);
	close(2);
}

// static void	display_message(char *exit_msg)
// {
// 	if (ft_strlen(exit_msg))
// 	{
// 		ft_putstr_fd(exit_msg, 2);
// 		ft_putstr_fd("\n", 2);
// 	}
// }

void	ft_exit(t_command *command, t_data *data, char *exit_msg, int check_arg)
{
	int	ret;

	(void)exit_msg;
	ret = data->g_return_code;
	if (!command)
	{
		// display_message(exit_msg);
		free_resources(data, data->env);
		if (g_signal_received == SIGINT)
			ret = 128 + SIGINT;
		exit(ret);
	}
	if (command->args && command->args[1] && check_arg == 0
		&& ft_strcmp(command->cmd, "exit") == 0)
	{
		if (check_arg_exit(command, data) == EXIT_FAILURE)
			return ;
	}
	ret = data->g_return_code;
	// display_message(exit_msg);
	free_resources(data, data->env);
	if (g_signal_received == SIGINT)
		ret = 128 + SIGINT;
	exit(ret & 255);
}
