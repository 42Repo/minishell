/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_args_less.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:15:42 by asuc              #+#    #+#             */
/*   Updated: 2024/04/05 16:16:53 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	print_sorted_env(t_env *env)
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
