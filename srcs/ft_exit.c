/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/28 02:04:12 by mbuchs           ###   ########.fr       */
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
	free(data);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	if (ft_strlen(exit_msg))
		printf("%s (%d)\n", exit_msg, exit_code);
	exit (0);
}

void	*sig_handler(int num)
{
	static t_data	*data_struct = NULL;

	if (data_struct == NULL)
		data_struct = malloc(sizeof(t_data));
	if (num == SIGINT)
		printf("^C\n");
	if (num == SIGINT)
		write(1, data_struct->cmd_prompt, ft_strlen(data_struct->cmd_prompt));
	return (data_struct);
}
