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

char	*set_token_str(char *str, int len)
{
	char	*token;

	token = malloc(sizeof(char) * len + 1);
	if (token == NULL)
		return (NULL);
	ft_strlcpy(token, str, len + 1);

	return (token);
}

void free_token_lst(t_data *data)
{
	t_token	*tmp;

	while (data->prompt_top)
	{
		tmp = data->prompt_top;
		data->prompt_top = data->prompt_top->next;
		free(tmp->value);
		free(tmp);
	}
	data->prompt_top = NULL;
}

t_token	*lexer(char *str, t_data *data)
{
	int		i = 0;
	int		j = 0;
	char	*token;

	(void)str;
	if(data->prompt_top)
		free_token_lst(data);
	while (str[i])
	{
		if (ft_isnamespace(str[i]) && i > j)
		{
			token = set_token_str(&str[j], i - j + 1);
			ft_printf("token = %s\n", token);
			ms_lstadd_back(&data->prompt_top, ms_lstnew(0, token), data);
			// free(token);
			j = i+1;
		}
		i++;
	}
	ms_lstadd_back(&data->prompt_top, ms_lstnew(0, "test1"), data);
	ms_lstadd_back(&data->prompt_top, ms_lstnew(0, "test2"), data);
	print_stack(data->prompt_top);
	return (NULL);
}

void	print_stack(t_token *node)
{
	while (node->next)
	{
		printf("Le node est de type %d et \
ipi contient : %s\n", node->type, node->value);
		node = node->next;
	}
	printf ("Le node est de type %d et \
contient : %s\n", node->type, node->value);
}

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