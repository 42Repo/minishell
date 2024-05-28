/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 19:03:53 by asuc              #+#    #+#             */
/*   Updated: 2024/05/28 16:59:11 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exit_with_error(char *arg, int error_code,
		t_data *data)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	data->g_return_code = error_code;
	free_resources(data, data->env);
	exit(error_code);
}

static int	is_within_longlong_limits(const char *num, int is_negative)
{
	size_t	num_len;

	num_len = strlen(num);
	if (is_negative)
	{
		if (num_len > strlen(LLONG_MIN_STR))
			return (1);
		else if (num_len == strlen(LLONG_MIN_STR))
		{
			if (strcmp(num, LLONG_MIN_STR) > 0)
				return (1);
		}
	}
	else
	{
		if (num_len > strlen(LLONG_MAX_STR))
			return (1);
		else if (num_len == strlen(LLONG_MAX_STR))
		{
			if (strcmp(num, LLONG_MAX_STR) > 0)
				return (1);
		}
	}
	return (0);
}

static int	check_numeric_and_boundaries(char *arg)
{
	int	i;
	int	j;
	int	is_negative;

	i = 0;
	is_negative = 0;
	while (arg[i] && ft_isnamespace(arg[i]))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			is_negative = 1;
		i++;
	}
	j = i;
	while (arg[j])
	{
		if (!ft_isdigit(arg[j]))
			return (1);
		j++;
	}
	if (is_within_longlong_limits(arg + i, is_negative) == 1 || arg[i] == '\0')
		return (1);
	return (0);
}

int	check_arg_exit(t_command *command, t_data *data, char *exit_msg)
{
	if (check_numeric_and_boundaries(command->args[1]))
	{
		if (exit_msg)
			free(exit_msg);
		exit_with_error(command->args[1], 2, data);
	}
	if (command->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->g_return_code = 1;
		if (exit_msg)
			free(exit_msg);
		return (EXIT_FAILURE);
	}
	if (command->args[1])
		data->g_return_code = ft_atoi(command->args[1]);
	return (EXIT_SUCCESS);
}
