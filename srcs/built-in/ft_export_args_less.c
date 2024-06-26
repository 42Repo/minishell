/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_args_less.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:15:42 by asuc              #+#    #+#             */
/*   Updated: 2024/05/10 22:20:59 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

	if (!*sorted_env || (!new_node->name && !(*sorted_env)->name
			&& ft_strcmp(new_node->name, (*sorted_env)->name) < 0))
	{
		new_node->next = *sorted_env;
		*sorted_env = new_node;
	}
	else
	{
		prev = *sorted_env;
		curr = (*sorted_env)->next;
		while (curr && ft_strcmp(new_node->name, curr->name) > 0)
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
		if ((tmp->name && tmp->name[0] == '_'
				&& tmp->name[1] == '\0') || (!tmp->name))
		{
			tmp = tmp->next;
			continue ;
		}
		printf("declare -x %s", tmp->name);
		if (tmp->value)
			printf("=\"%s\"", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
	free_env(sorted_env);
}
