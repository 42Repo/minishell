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

	int i = 0;

	while (data->prompt_top != NULL)
	{
		printf("i = %d\n", i++);
		tmp = data->prompt_top;
		data->prompt_top = data->prompt_top->next;
		printf("value = %s\n", tmp->value);
		free(tmp->value);
		free(tmp);
	}
	data->prompt_top = NULL;
}

char *remove_quotes(char *str, int type)
/*
	1 = simple quote
	2 = double quote
*/ 
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	while (str[i])
	{
		if ((str[i] != '\'' && type == 1) || (str[i] != '"' && type == 2))
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

int	quote_management(int i, char c)
{
	if ((i == 1 && c == '\'' ) || (i == 2 && c == '"'))
		return (0);
	if (i == 0 && c == '\'')
		return (1);
	if (i == 0 && c == '"')
		return (2);
	return (i);
}

void 	add_token_to_list(t_data *data, char *str, int len, int last_quote)
{
	ms_lstadd_back(&data->prompt_top, ms_lstnew(0,ft_strtrim(set_token_str(str, len), " ")), data);
				if(last_quote == 1)
					remove_quotes(ms_lstlast(data->prompt_top)->value, 1);
				else if(last_quote == 2)
					remove_quotes(ms_lstlast(data->prompt_top)->value, 2);
}

t_token	*lexer(char *str, t_data *data)
{
	int		i;
	int		j;
	int		is_ok;
	int	last_quote;

	last_quote = 0;
	i = 0;
	j = 0;
	is_ok = 1;
	if (data->prompt_top)
		free_token_lst(data);
	while (str[i])
	{
		if (i > 1 && str[i] == '>' && str[i - 1] == '>')
			i++;
		if (ft_isnamespace(str[i]) && j < i && data->quote_state== 0)
		{
			if (!is_ok)
				add_token_to_list(data, &str[j], i - j, last_quote);
			if (!is_ok)
				j = i + 1;
			is_ok = 1;
		}
		else if (!ft_isnamespace(str[i]))
			is_ok = 0;
		if (quote_management(data->quote_state, str[i]) != data->quote_state && data->quote_state != 0)
			last_quote = data->quote_state;
		data->quote_state= quote_management(data->quote_state, str[i]);
		i++;
	}
	if (!ft_isnamespace(str[i]) && j < i)
		add_token_to_list(data, &str[j], i - j, last_quote);
	print_stack(data->prompt_top);
	// if quote != 0
	// 	changer prompt par "squote" ou "dquote"
	// 	et attendre la fin de la quote
	return (NULL);
}

void	print_stack(t_token *node)
{
	while (node->next)
	{
		printf("Type = %d and value = %s\n", node->type, node->value);
		node = node->next;
	}
	printf("Type = %d and value = %s\n", node->type, node->value);
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

t_token		*ms_lstnew(t_token_type type, char *value)
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