/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:55:16 by asuc              #+#    #+#             */
/*   Updated: 2024/04/05 15:55:16 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	add_new_env_variable(t_env *env, char *arg)
{
	t_env	*new_env;

	new_env = ft_calloc(sizeof(t_env), 1);
	if (!new_env)
		return ;
	new_env->name = ft_strndup(arg, ft_strchr(arg, '=') - arg);
	new_env->value = ft_strdup(ft_strchr(arg, '=') + 1);
	new_env->next = NULL;
	while (env->next)
		env = env->next;
	env->next = new_env;
}

static void	process_arg(t_env *env, char *arg)
{
	t_env	*tmp;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return ;
	}
	tmp = get_env_value_ptr(env, arg);
	if (tmp)
	{
		free(env->value);
		env->value = ft_strdup(ft_strchr(arg, '=') + 1);
	}
	else
		add_new_env_variable(env, arg);
}

int	ft_export(t_env *env, char **args)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		print_sorted_env(env);
		return (0);
	}
	while (args[i])
	{
		process_arg(env, args[i]);
		i++;
	}
	return (0);
}