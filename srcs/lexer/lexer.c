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

char	*remove_quotes(char *str, int mode)
{
	int		i;
	int		j;
	char	*new_str;
	int		local_quote;

	i = 0;
	j = 0;
	new_str = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	if (!str || new_str == NULL)
		return (NULL);
	local_quote = 0;
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
	if (mode == 1)
		free(str);
	return (new_str);
}

int	quote_management(int i, char c)
{
	if (i == 0 && c == '\'')
		return (1);
	if (i == 0 && c == '"')
		return (2);
	if ((i == 1 && c == '\'' ) || (i == 2 && c == '"'))
		return (0);
	return (i);
}

		// if (i > 1 && str[i] == '>' && str[i - 1] == '>')
		// 	i++;

// void printf_stack(t_data *data)
// {
// 	t_token	*token;

// 	token = data->prompt_top;
// 	while (token)
// 	{
// 		printf("type = %d, value = %s\n", token->type, token->value);
// 		token = token->next;
// 	}
// }

char	*remove_dollard_quote(char *str)
{
	int		i;
	int		j;
	char	*new_str;
	int		quote_state;
	
	quote_state = 0;
	
	i = 0;
	j = 0;

	new_str = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	if (!str || new_str == NULL)
		return (NULL);	
	while (str[i])
	{
		quote_state = quote_management(quote_state, str[i]);
		new_str[j] = str[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

int	lexer(char *str, t_data *data)
{
	int		i;
	int		j;
	t_token	*token;

	data->quote_state = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		data->quote_state = quote_management(data->quote_state, str[i]);
		if (ft_isnamespace(str[i]) && j < i && data->quote_state == 0)
			add_word_to_list(str, &i, &j, data);
		check_pipe_redir(str, &i, &j, data);
		i++;
	}
	if (data->quote_state != 0)
	{
		ft_putstr_fd("quote error\n", 2);
		free_token_lst(data);
		return (-1);
	}
	if (!ft_isnamespace(str[i]) && j < i)
		add_token_to_list(data, &str[j], i - j, WORD);
	add_token_to_list(data, ft_strdup("newline"), 7, END);
	token = data->prompt_top;
	while (token->type != END)
	{
		token->value = remove_dollard_quote(token->value);
		token = token->next;
	}
	// printf_stack(data);
	return (0);
}
