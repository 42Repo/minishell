/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 08:45:19 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/13 22:46:02 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_lstadd_back(t_token **lst, t_token *new, t_data *data)
{
	t_token	*actual;

	if (!data->prompt_top)
	{
		data->prompt_top = new;
		return ;
	}
	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	actual = ms_lstlast(*lst);
	actual -> next = new;
	new->next = NULL;
}

t_token	*ms_lstnew(t_token_type type, char *value)
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

void	free_token_lst(t_data *data)
{
	// t_token *selected;
	// t_token *old;
	
	// selected = data->prompt_top;

	// while(selected)
	// {
	// 	if(selected->value)
	// 		free(selected->value);
	// 	old = selected;
	// 	selected = selected->next;
	// 	if (old)
	// 		free(old);
	// 	printf("cleared cmd\n");
	// }

	t_token	*tmp;


	while (data->prompt_top != NULL)
	{
		tmp = data->prompt_top;
		data->prompt_top = data->prompt_top->next;
		free(tmp->value);
		free(tmp);
	}
	data->prompt_top = NULL;
}
