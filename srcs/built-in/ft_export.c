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

static void	create_env_node(char *arg, int has_equal, t_env *env)
{
	t_env	*new_env;

	new_env = ft_calloc(sizeof(t_env), 1);
	if (!new_env)
		return ;
	if (has_equal == 0)
	{
		new_env->name = ft_strdup(arg);
		new_env->value = NULL;
		new_env->next = NULL;
	}
	else
	{
		new_env->name = ft_strndup(arg, ft_strchr(arg, '=') - arg);
		new_env->value = ft_strdup(ft_strchr(arg, '=') + 1);
		new_env->next = NULL;
	}
	while (env->next)
		env = env->next;
	env->next = new_env;
}

void	add_new_env_variable(t_env *env, char *arg)
{
	if (!env)
		return ;
	if (!env->name)
	{
		env->name = ft_strndup(arg, ft_strchr(arg, '=') - arg);
		env->value = ft_strdup(ft_strchr(arg, '=') + 1);
		env->next = NULL;
		return ;
	}
	if (ft_is_equal(arg) == 1)
		create_env_node(arg, 1, env);
	else
		create_env_node(arg, 0, env);
}

static void	process_arg(t_env *env, char *arg)
{
	t_env	*tmp;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		put_error("minishell: export: `", arg, "': not a valid identifier\n");
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
