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
	int		i;

	i = 0;
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
		if (ft_strchr(arg, '+') == ft_strchr(arg, '=') - 1)
			i = 1;
		new_env->name = ft_strndup(arg, ft_strchr(arg, '=') - arg - i);
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
		if (ft_strchr(arg, '+') == ft_strchr(arg, '=') - 1)
		{
			env->name = ft_strndup(arg, ft_strchr(arg, '=') - arg - 1);
			env->value = ft_strdup(ft_strchr(arg, '=') + 1);
			env->next = NULL;
			return ;
		}
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

void	set_env_variable(t_env *tmp, char **value, char *arg)
{
	if (ft_strchr(arg, '+') == ft_strchr(arg, '=') - 1)
	{
		(*value) = ft_strjoin(tmp->value, ft_strchr(arg, '=') + 1);
		if (!(*value))
			(*value) = ft_strdup(ft_strchr(arg, '=') + 1);
		free(tmp->value);
		tmp->value = (*value);
		return ;
	}
	(*value) = ft_strchr(arg, '=');
	if ((*value))
	{
		free(tmp->value);
		tmp->value = ft_strdup((*value) + 1);
	}
}

static void	process_arg(t_env *env, char *arg, t_data *data)
{
	t_env	*tmp;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		put_error("minishell: export: `", arg, "': not a valid identifier\n");
		data->g_return_code = 1;
		return ;
	}
	tmp = get_env_value_ptr(env, arg);
	if (tmp)
		set_env_variable(tmp, &value, arg);
	else
		add_new_env_variable(env, arg);
}

int	ft_export(t_env *env, char **args, t_data *data)
{
	int	i;

	i = 1;
	data->g_return_code = 0;
	if (!args[i])
	{
		print_sorted_env(env);
		return (0);
	}
	while (args[i])
	{
		process_arg(env, args[i], data);
		i++;
	}
	return (0);
}
