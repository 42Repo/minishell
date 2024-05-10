/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/10 20:04:25 by asuc             ###   ########.fr       */
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

void	handle_exit_error(char *arg, char *message)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(message, 2);
}

int	check_arg_exit(t_command *command)
{
	int	i;
	int	j;

	i = 0;
	while (command->args[1][i] && ft_isnamespace(command->args[1][i]) == 1)
		i++;
	j = i;
	while (command->args[1][j])
	{
		if (!ft_isdigit(command->args[1][j]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(command->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_return_code = 2;
			exit(2);
		}
		j++;
	}
	if (command->args[1][i] == '-')
	{
		i++;
		if (ft_strlen(command->args[1] + i) > ft_strlen(LLONG_MAX_STR)
			|| (ft_strlen(command->args[1] + i) == ft_strlen(LLONG_MIN_STR)
				&& ft_strcmp(command->args[1] + i, LLONG_MIN_STR) > 0))
		{
			handle_exit_error(command->args[1], ": numeric argument required\n");
			g_return_code = 2;
			exit(2);
		}
	}
	else
	{
		if (command->args[1][i] == '+')
			i++;
		if ((command && command->args && command->args[0] && command->args[1] && command->args[1] + i && ft_strlen(command->args[1] + i) > ft_strlen(LLONG_MAX_STR))
			|| (ft_strlen(command->args[1] + i) == ft_strlen(LLONG_MAX_STR)
				&& ft_strcmp(command->args[1] + i, LLONG_MAX_STR) > 0))
		{
			handle_exit_error(command->args[1], ": numeric argument required\n");
			g_return_code = 2;
			exit(2);
		}
	}
	if (command->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_return_code = 1;
		return (EXIT_FAILURE);
	}
	if (command->args[1])
		g_return_code = ft_atoi(command->args[1]);
	return (EXIT_SUCCESS);
}

void	ft_exit(t_command *command, t_data *data, t_env *env, char *exit_msg) // TODO : A clean + changer la logique
{
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
	if (command->args && command->args[1])
	{
		if (check_arg_exit(command) == EXIT_FAILURE)
			return ;
	}
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
	exit(EXIT_SUCCESS);
}

