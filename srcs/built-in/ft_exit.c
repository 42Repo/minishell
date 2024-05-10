/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:30:00 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/10 20:37:26 by asuc             ###   ########.fr       */
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

void	exit_with_error(char *arg, char *message, int error_code)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(message, 2);
	g_return_code = error_code;
	exit(error_code);
}

int	is_within_longlong_limits(const char *num, int is_negative)
{
	if (is_negative)
	{
		return (ft_strlen(num) < ft_strlen(LLONG_MIN_STR)
			|| (ft_strlen(num) == ft_strlen(LLONG_MIN_STR)
				&& ft_strcmp(num, LLONG_MIN_STR) <= 0));
	}
	else
	{
		return (ft_strlen(num) < ft_strlen(LLONG_MAX_STR)
			|| (ft_strlen(num) == ft_strlen(LLONG_MAX_STR)
				&& ft_strcmp(num, LLONG_MAX_STR) <= 0));
	}
}

int	check_numeric_and_boundaries(char *arg)
{
	int	i;
	int	j;

	i = 0;
	while (arg[i] && ft_isnamespace(arg[i]))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	j = i;
	while (arg[j])
	{
		if (!ft_isdigit(arg[j]))
			return (0);
		j++;
	}
	if (is_within_longlong_limits(arg + i, arg[i - 1] == '-'))
		return (1);
	return (0);
}

int	check_arg_exit(t_command *command)
{
	if (!check_numeric_and_boundaries(command->args[1]))
		exit_with_error(command->args[1], ": numeric argument required\n", 2);
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

void	free_resources(t_data *data, t_env *env)
{
	free_token_lst(data);
	free_env(env);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
}

void	ft_exit(t_command *command, t_data *data, t_env *env, char *exit_msg)
{
	if (!command)
	{
		free_resources(data, env);
		exit(g_return_code);
	}
	if (command->args && command->args[1])
	{
		if (check_arg_exit(command) == EXIT_FAILURE)
			return ;
	}
	free_resources(data, env);
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
