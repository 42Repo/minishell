/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/21 01:24:36 by asuc             ###   ########.fr       */
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

void	ft_exit(t_data *data, t_env *env, char *exit_msg, int exit_code)
{
	int	i;
	
	i = 0;
	if (data->command_top->args[0] && data->command_top->args[1] && data->command_top->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_return_code = 1;
		return ;
	}
	if (data->command_top->args[0] && data->command_top->args[1])
	{
		if ((data->command_top->args[1][0] == '+' || data->command_top->args[1][0] == '-'))
			i++;
	}
	while(data->command_top->args[1] && data->command_top->args[1][i])
	{
		if(!ft_isdigit(data->command_top->args[1][i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(data->command_top->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_return_code = 2;
			return ;
		}
		i++;
	}
	if(data->command_top->args[1])
		exit_code = ft_atoi(data->command_top->args[1]);
	free_token_lst(data);
	free_env(env);
	free_command(data);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
	if (ft_strlen(exit_msg))
		printf("%s\n", exit_msg);
	if (exit_code >= 0 && exit_code <= 255)
		exit(exit_code);
	if (exit_code < 0)
		exit(256 + exit_code);
	if (exit_code > 255)
		exit(exit_code % 256);
	else
	{
		printf("minishell: exit: %d: numeric argument required\n", exit_code);
		exit(2);
	}
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
