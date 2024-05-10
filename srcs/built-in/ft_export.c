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
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+')
	{
		if (str[i + 1] != '=')
			return (0);
	}
	return (1);
}

static int	ft_is_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	add_new_env_variable(t_env *env, char *arg)
{
	t_env	*new_env;

	if (!env)
		return ;
	if (!env->name)
	{
		env->name = ft_strndup(arg, ft_strchr(arg, '=') - arg);
		env->value = ft_strdup(ft_strchr(arg, '=') + 1);
		env->next = NULL;
		return ;
	}
	new_env = ft_calloc(sizeof(t_env), 1);
	if (!new_env)
		return ;
	if (!(ft_is_equal(arg)))
	{
		new_env->name = ft_strdup(arg);
		new_env->value = NULL;
		new_env->next = NULL;
		while (env->next)
			env = env->next;
		env->next = new_env;
		return ;
	}
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
	char	*value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		g_return_code = 1;
		return ;
	}
	tmp = get_env_value_ptr(env, arg);
	if (tmp)
	{
		if (ft_strchr(arg, '+') == ft_strchr(arg, '=') - 1)
		{
			value = ft_strjoin(tmp->value, ft_strchr(arg, '=') + 1);
			free(tmp->value);
			tmp->value = value;
			return ;
		}
		free(tmp->value);
		tmp->value = ft_strdup(ft_strchr(arg, '=') + 1);
	}
	else
		add_new_env_variable(env, arg);
}

int	ft_export(t_env *env, char **args)
{
	int	i;

	i = 1;
	g_return_code = 0;
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
