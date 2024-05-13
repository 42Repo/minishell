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

	token = ft_calloc(sizeof(char), len + 1);
	if (token == NULL)
		return (NULL);
	ft_strlcpy(token, str, len + 1);
	return (token);
}

int	get_quote_type(char *str)
{
	int		i;
	int		type;

	i = 0;
	type = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			type = 1;
		if (str[i] == '"')
			type = 2;
		if (str[i] == '\'' || str[i] == '"')
			break ;
		i++;
	}
	return (type);
}

int	if_remove_quote(char c, int local_quote)
{
	return (!((c == '\'' && (local_quote == 0 || local_quote == 1)) \
				|| (c == '"' && (local_quote == 0 || local_quote == 2))));
}

void	remover_loop(char c, char **new_str, int *j, int *local_quote)
{
	if (if_remove_quote(c, (*local_quote)))
		(*new_str)[(*j)] = c;
	if (if_remove_quote(c, (*local_quote)))
		(*j)++;
	(*local_quote) = quote_management((*local_quote), c);
}

char	*remove_quotes(char *str, int mode)
{
	int		i;
	int		j;
	char	*new_str;
	int		local_quote;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	if (!*str)
		return (str);
	new_str = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	local_quote = 0;
	while (str[i])
	{
		remover_loop(str[i], &new_str, &j, &local_quote);
		i++;
	}
	if (mode == 1)
		free(str);
	return (new_str);
}
