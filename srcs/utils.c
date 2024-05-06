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

#include "../includes/minishell.h"

int	max_len(char *str, size_t nb)
{
	if (ft_strlen(str) > nb)
		return (ft_strlen(str));
	return (nb);
}

char	*ft_strtrim_free(char *s1, char *set)
{
	char *line;

	line = ft_strtrim(s1, set);
	if (s1)
		free(s1);
	return (line);
}

static int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static char	*ft_strdup_split_quote(const char *src, char charset)
{
	char	*tab;
	int		i;
	int		in_quote;
	char	current_quote;

	i = 0;
	in_quote = 0;
	current_quote = 0;
	while (src[i] && (src[i] != charset || in_quote))
	{
		if (is_quote(src[i]))
		{
			if (!in_quote)
			{
				in_quote = 1;
				current_quote = src[i];
			}
			else if (src[i] == current_quote)
			{
				in_quote = 0;
			}
		}
		i++;
	}
	tab = malloc(sizeof(char) * (i + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	in_quote = 0;
	while (src[i] && (src[i] != charset || in_quote))
	{
		if (is_quote(src[i]))
		{
			if (!in_quote)
			{
				in_quote = 1;
				current_quote = src[i];
			}
			else if (src[i] == current_quote)
			{
				in_quote = 0;
			}
		}
		tab[i] = src[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

static int	count_words_quote(const char *str, char charset)
{
	int		count;
	int		in_quote;
	int		bol;
	char	current_quote;

	count = 0;
	in_quote = 0;
	bol = 1;
	current_quote = 0;
	while (*str)
	{
		if (is_quote(*str))
		{
			if (!in_quote)
			{
				in_quote = 1;
				current_quote = *str;
			}
			else if (*str == current_quote)
			{
				in_quote = 0;
			}
		}
		if (*str != charset || in_quote)
		{
			if (bol == 1)
			{
				count++;
			}
			bol = 0;
		}
		else
		{
			bol = 1;
		}
		str++;
	}
	return (count);
}

char	**ft_split_quote_state(const char *str, char charset)
{
	int		words;
	char	**ret;
	int		i;
	int		j;
	int		in_quote;
	char	current_quote;

	i = 0;
	j = 0;
	in_quote = 0;
	current_quote = 0;
	words = count_words_quote(str, charset);
	ret = malloc(sizeof(char *) * (words + 1));
	if (!str)
		return (NULL);
	if (ret == NULL)
		return (NULL);
	while (i < words)
	{
		while ((str[j] == charset && !in_quote) && str[j] != '\0')
		{
			j++;
		}
		ret[i] = ft_strdup_split_quote(str + j, charset);
		while ((str[j] != charset || in_quote) && str[j] != '\0')
		{
			if (is_quote(str[j]))
			{
				if (!in_quote)
				{
					in_quote = 1;
					current_quote = str[j];
				}
				else if (str[j] == current_quote)
				{
					in_quote = 0;
				}
			}
			j++;
		}
		i++;
	}
	ret[i] = NULL;
	return (ret);
}