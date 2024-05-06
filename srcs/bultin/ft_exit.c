/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/06 18:43:12 by asuc             ###   ########.fr       */
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
	if (!command)
	{
		free_token_lst(data);
		free_env(env);
		rl_clear_history();
		if (data->cmd_prompt)
			free(data->cmd_prompt);
		free(data);
		exit(g_return_code);
	}
	while (command && command->args && command->args[0]
		&& command->args[1] && (command->args[1][i] == '\t'
		|| command->args[1][i] == '\n' || command->args[1][i] == '\v'
		|| command->args[1][i] == '\f' || command->args[1][i] == '\r'))
		i++;
	if (command && command->args && command->args[0] && command->args[1]
		&& (command->args[1][i] == '+' || command->args[1][i] == '-'))
		i++;
	while (command && command->args && command->args[0] && command->args[1] && command->args[1][i])
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
	i = 0;
	while (command->args && command->args[0] && command->args[1]
		&& command->args[1][i] && (command->args[1][i] == '\t'
		|| command->args[1][i] == '\n' || command->args[1][i] == '\v'
		|| command->args[1][i] == '\f' || command->args[1][i] == '\r'))
		i++;
	if (command && command->args && command->args[0] && command->args[1]
		&& command->args[1][i] == '-')
	{
		i++;
		if (ft_strlen(command->args[1]) > ft_strlen(LLONG_MAX_STR)
			|| (ft_strlen(command->args[1] + i) == ft_strlen(LLONG_MIN_STR)
				&& ft_strcmp(command->args[1] + i, LLONG_MIN_STR) > 0))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(command->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_return_code = 2;
			exit(2);
		}
	}
	else
	{
		if (command && command->args && command->args[0] && command->args[1]
			&& command->args[1][i] == '+')
			i++;
		if (ft_strlen(command->args[1]) > ft_strlen(LLONG_MAX_STR)
			|| (ft_strlen(command->args[1] + i) == ft_strlen(LLONG_MAX_STR)
				&& ft_strcmp(command->args[1] + i, LLONG_MAX_STR) > 0))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(command->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_return_code = 2;
			exit(2);
		}
	}
	if (command && command->args && command->args[0]
		&& command->args[1] && command->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_return_code = 1;
		return ;
	}
	if (command && command->args && command->args[1])
		g_return_code = ft_atoi(command->args[1]);
	free_token_lst(data);
	free_env(env);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
	if (ft_strlen(exit_msg))
	{
		ft_putstr_fd(exit_msg, 2);
		ft_putstr_fd("\n", 2);
	}
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
