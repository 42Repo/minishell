/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/02 17:36:54 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_return_code;

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

void	ft_exit(t_command *command, t_data *data, t_env *env, char *exit_msg) // TODO : A clean + changer la logique
{
	int	i;

	i = 0;
	if (command && command->args && command->args[0]
		&& command->args[1] && command->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_return_code = 1;
		return ;
	}
	if (command && command->args && command->args[0] && command->args[1]
		&& (command->args[1][0] == '+' || command->args[1][0] == '-'))
		i++;
	if (command && command->args && command->args[1])
	{
		while (command->args[1][i])
		{
			if (!ft_isdigit(command->args[1][i]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(command->args[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				g_return_code = 2;
				exit(2);
			}
			i++;
		}
		g_return_code = ft_atoi(command->args[1]);
	}
	free_token_lst(data);
	free_env(env);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
	if (ft_strlen(exit_msg))
		printf("%s\n", exit_msg);
	if (g_return_code >= 0 && g_return_code <= 255)
		exit(g_return_code);
	if (g_return_code < 0)
		exit(256 + g_return_code);
	if (g_return_code > 255)
		exit(g_return_code % 256);
	exit(1);
}

void	sig_handler(int num)
{
	if (num == SIGINT)
	{
		printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_return_code = 130;
	}
}
