/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/03/11 17:52:15 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	free_env(t_env *env)
{
	t_env	*tmp;

	while (env->next)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
	free(env->name);
	free(env->value);
	free(env);
	return (0);
}

void	ft_exit(t_data *data, t_env *env, char *exit_msg, int exit_code)
{
	free_token_lst(data);
	free_env(env);
	free_command(data);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
	if (ft_strlen(exit_msg))
		printf("%s (%d)\n", exit_msg, exit_code);
	exit (0);
}

void	*sig_handler(int num)
{
	if (num == SIGINT)
	{
		printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return (data_struct);
}
