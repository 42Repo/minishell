/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:28:24 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/03 14:50:20 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *lexer(char *str, t_data *data)
{
	(void)str;
	int i = 0;
	int j = 0;

	while(str[i])
	{
		if(str[i] == ' ')
			ms_lstadd_back(&data->prompt_top, ms_lstnew(0, &str[i]));
		i++;
	}
	print_stack(data->prompt_top);
	return (NULL);
}

void	print_stack(t_token *node)
{
	while(node->next)
	{
		printf("Le node est de type %d et contient : %s\n", node->type, node->value);
		node = node->next;
	}
		printf("Le node est de type %d et contient : %s\n", node->type, node->value);
}

void	ms_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*actual;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	actual = ms_lstlast(*lst);
	actual -> next = new;
}

void	ms_lstadd_front(t_token **lst, t_token *new)
{
	if (!lst || !new)
		return ;
	new -> next = *lst;
	*lst = new;
}

t_token	*ms_lstnew(int type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (new == NULL)
		return (NULL);
	new -> type = type;
	new -> value = value;
	new -> next = NULL;
	return (new);
}

t_token	*ms_lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst -> next)
		lst = lst -> next;
	return (lst);
}