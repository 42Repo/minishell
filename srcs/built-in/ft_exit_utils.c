/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 19:03:53 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 19:07:45 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exit_with_error(char *arg, char *message, int error_code)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(message, 2);
	g_return_code = error_code;
	exit(error_code);
}

static int	is_within_longlong_limits(const char *num, int is_negative)
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

static int	check_numeric_and_boundaries(char *arg)
{
	int	i;
	int	j;

	i = 0;
	if (arg[i] == '\0')
		return (0);
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
