/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 11:54:59 by asuc              #+#    #+#             */
/*   Updated: 2024/02/04 11:59:08 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_split_var(int *i, int *j, int *in_qt, char *current_quote)
{
	(*i) = 0;
	(*j) = 0;
	(*in_qt) = 0;
	(*current_quote) = 0;
}

char	*ft_strdup_split_quote(const char *src, const char *charset)
{
	char	*tab;
	int		i;
	int		in_quote;
	char	cur_qt;

	set_split_var(&i, &i, &in_quote, &cur_qt);
	tab = ft_calloc(sizeof(char), (get_strdup_len(src, charset, &cur_qt)));
	if (tab == NULL)
		return (NULL);
	while (src[i] && (!is_delimiter(src[i], charset) || in_quote))
	{
		if (is_quote(src[i]))
		{
			if (!in_quote)
			{
				in_quote = 1;
				cur_qt = src[i];
			}
			else if (src[i] == cur_qt)
				in_quote = 0;
		}
		tab[i] = src[i];
		i++;
	}
	return (tab);
}

void	if_case_quote(int *in_quote, char *current_quote, char str)
{
	if (!(*in_quote))
	{
		(*in_quote) = 1;
		(*current_quote) = str;
	}
	else if (str == (*current_quote))
		(*in_quote) = 0;
}

int	count_words_quote(const char *str, const char *charset)
{
	int		count;
	int		in_quote;
	int		bol;
	char	current_quote;

	set_split_var(&count, &in_quote, &in_quote, &current_quote);
	bol = 1;
	while (*str)
	{
		if (is_quote(*str))
			if_case_quote(&in_quote, &current_quote, *str);
		if (!is_delimiter(*str, charset) || in_quote)
		{
			if (bol == 1)
				count++;
			bol = 0;
		}
		else
			bol = 1;
		str++;
	}
	return (count);
}

void	if_quote_case(int *in_quote, char *current_quote, char *str, int j)
{
	if (is_quote(str[j]))
	{
		if (!in_quote)
		{
			(*in_quote) = 1;
			(*current_quote) = str[j];
		}
		else if (str[j] == (*current_quote))
			(*in_quote) = 0;
	}
}
