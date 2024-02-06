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

char *remove_quotes(char *str)
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
		if (str[i] != '\'' && str[i] != '"')
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

t_token	*lexer(char *str, t_data *data)
{
	int		i;
	int		j;
	int		is_ok;
	int		quote;

	i = 0;
	j = 0;
	quote = 0;
	is_ok = 1;
	(void)str;
	if (data->prompt_top)
		free_token_lst(data);
	while (str[i])
	{
		if (i > 1 && str[i] == '>' && str[i - 1] == '>')
			i++;
		if (ft_isnamespace(str[i]) && j < i && quote == 0)
		{
			if (!is_ok)
			{
				ms_lstadd_back(&data->prompt_top, ms_lstnew(0, \
				ft_strtrim(remove_quotes(set_token_str(&str[j], i - j)), " ")), data);
				j = i + 1;
			}
			is_ok = 1;
		}
		else if (!ft_isnamespace(str[i]))
			is_ok = 0;
		quote = quote_management(quote, str[i]);
		i++;
	}
	if (!ft_isnamespace(str[i]) && j < i)
		ms_lstadd_back(&data->prompt_top, ms_lstnew(0, \
		ft_strtrim(set_token_str(&str[j], i - j), " ")), data);

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