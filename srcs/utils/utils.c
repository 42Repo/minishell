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

char	*ft_strtrim_free(char *s1, char *set)
{
	char	*line;

	line = ft_strtrim(s1, set);
	if (s1)
		free(s1);
	return (line);
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	is_delimiter(char c, const char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

int	get_strdup_len(const char *src, const char *charset, char *current_quote)
{
	int	i;
	int	in_quote;

	in_quote = 0;
	i = 0;
	while (src[i] && (!is_delimiter(src[i], charset) || in_quote))
	{
		if (is_quote(src[i]))
		{
			if (!in_quote)
			{
				in_quote = 1;
				(*current_quote) = src[i];
			}
			else if (src[i] == (*current_quote))
				in_quote = 0;
		}
		i++;
	}
	return (i + 1);
}
