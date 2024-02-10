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

char	*remove_quotes(char *str, t_data *data)
{
	int		i;
	int		j;
	char	*new_str;
	int		local_quote;

	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	local_quote = data->quote_state;
	while (str[i])
	{
		if (!((str[i] == '\'' && (local_quote == 0 || local_quote == 1)) \
				|| (str[i] == '"' && (local_quote == 0 || local_quote == 2))))
			new_str[j] = str[i];
		if (!((str[i] == '\'' && (local_quote == 0 || local_quote == 1)) \
			|| (str[i] == '"' && (local_quote == 0 || local_quote == 2))))
			j++;
		local_quote = quote_management(local_quote, str[i]);
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

int	quote_management(int i, char c)
{
	if (i == 0 && c == '\'')
		printf("simple quote open\n");
	if (i == 0 && c == '\'')
		return (1);
	if (i == 0 && c == '"')
		printf("double quote open\n");
	if (i == 0 && c == '"')
		return (2);
	if ((i == 1 && c == '\'' ) || (i == 2 && c == '"'))
		printf("quote close\n");
	if ((i == 1 && c == '\'' ) || (i == 2 && c == '"'))
		return (0);
	return (i);
}

//use quote management pour erase quote

void	lexer(char *str, t_data *data)
{
	int		i;
	int		j;
	int		is_ok;

	i = 0;
	j = 0;
	is_ok = 1;
	if (data->prompt_top)
		free_token_lst(data);
	while (str[i])
	{
		if (i > 1 && str[i] == '>' && str[i - 1] == '>')
			i++;
		if (ft_isnamespace(str[i]) && j < i && data->quote_state == 0)
			is_ok = add_word_to_list(str, &i, &j, data);
		else if (!ft_isnamespace(str[i]))
			is_ok = 0;
		check_pipe_redir(str, &i, &j, data);
		data->quote_state = quote_management(data->quote_state, str[i]);
		i++;
	}
	if (!ft_isnamespace(str[i]) && j < i)
		add_token_to_list(data, &str[j], i - j, WORD);
	add_token_to_list(data, "", 0, END);
}

	// print_stack(data->prompt_top);
	// if quote != 0
	// 	syntax error

// void	print_stack(t_token *node)
// {
// 	while (node->next)
// 	{
// 		printf("Type = %d and value = %s\n", node->type, node->value);
// 		node = node->next;
// 	}
// 	printf("Type = %d and value = %s\n", node->type, node->value);
// }
