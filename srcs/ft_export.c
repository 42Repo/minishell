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

#include "../includes/minishell.h"

static void	free_env(t_env *env)
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

static t_env	*create_new_node(t_env *tmp)
{
	t_env	*new_node;

	new_node = ft_calloc(sizeof(t_env), 1);
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdup(tmp->name);
	new_node->value = ft_strdup(tmp->value);
	if ((!new_node->name || !new_node->value) && errno == ENOMEM)
	{
		free(new_node->name);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

static void	insert_node(t_env **sorted_env, t_env *new_node)
{
	t_env	*prev;
	t_env	*curr;

	if (!*sorted_env || strcmp(new_node->name, (*sorted_env)->name) < 0)
	{
		new_node->next = *sorted_env;
		*sorted_env = new_node;
	}
	else
	{
		prev = *sorted_env;
		curr = (*sorted_env)->next;
		while (curr && strcmp(new_node->name, curr->name) > 0)
		{
			prev = curr;
			curr = curr->next;
		}
		new_node->next = curr;
		prev->next = new_node;
	}
}

static t_env	*sort_env(t_env *env)
{
	t_env	*sorted_env;
	t_env	*tmp;
	t_env	*new_node;

	sorted_env = NULL;
	tmp = env;
	while (tmp)
	{
		new_node = create_new_node(tmp);
		if (!new_node)
		{
			free_env(sorted_env);
			return (NULL);
		}
		insert_node(&sorted_env, new_node);
		tmp = tmp->next;
	}
	return (sorted_env);
}

static void	print_sorted_env(t_env *env)
{
	t_env	*tmp;
	t_env	*sorted_env;

	sorted_env = sort_env(env);
	if (!sorted_env)
		return ;
	tmp = sorted_env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->name, 1);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(tmp->value, 1);
		ft_putstr_fd("\"\n", 1);
		tmp = tmp->next;
	}
	free_env(sorted_env);
}

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

void	print_error_message(char *arg)
{
	ft_putstr_fd("bash: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	update_env_value(t_env *env, char *arg)
{
	free(env->value);
	env->value = ft_strdup(ft_strchr(arg, '=') + 1);
}

void	add_new_env_variable(t_env *env, char *arg)
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

void	process_arg(t_env *env, char *arg)
{
	t_env	*tmp;

	if (!is_valid_identifier(arg))
	{
		print_error_message(arg);
		return ;
	}
	tmp = get_env_value_ptr(env, arg);
	if (tmp)
		update_env_value(tmp, arg);
	else
		add_new_env_variable(env, arg);
}

void	ft_export(t_env *env, char **args)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		print_sorted_env(env);
		return ;
	}
	while (args[i])
	{
		process_arg(env, args[i]);
		i++;
	}
}
