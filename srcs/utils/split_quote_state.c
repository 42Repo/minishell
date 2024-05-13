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

char	**ft_split_quote_state(char *str, const char *charset)
{
	char	**ret;
	int		i;
	int		j;
	int		in_qt;
	char	current_quote;

	if (!str || !charset)
		return (NULL);
	ret = malloc(sizeof(char *) * (count_words_quote(str, charset) + 1));
	if (ret == NULL)
		return (NULL);
	set_split_var(&i, &j, &in_qt, &current_quote);
	while (i < count_words_quote(str, charset))
	{
		while ((is_delimiter(str[j], charset) && !in_qt) && str[j] != '\0')
			j++;
		ret[i] = ft_strdup_split_quote(str + j, charset);
		while ((!is_delimiter(str[j++], charset) || in_qt) && str[j] != '\0')
			if_quote_case(&in_qt, &current_quote, str, j);
		i++;
	}
	free(str);
	ret[i] = NULL;
	return (ret);
}
