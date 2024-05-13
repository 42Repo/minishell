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
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

int	lexer_error(t_data *data)
{
	ft_putstr_fd("minishell: syntax error\n", 2);
	free_token_lst(data);
	return (-1);
}

void	lexer_loop(char *str, int *i, int *j, t_data *data)
{
	data->quote_state = quote_management(data->quote_state, str[(*i)]);
	if (ft_isnamespace(str[(*i)]) && (*j) < (*i) && data->quote_state == 0)
		add_word_to_list(str, i, j, data);
	check_pipe_redir(str, i, j, data);
	(*i)++;
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
		lexer_loop(str, &i, &j, data);
	if (data->quote_state != 0)
		return (lexer_error(data));
	if (!ft_isnamespace(str[i]) && j < i)
		add_token_to_list(data, &str[j], i - j, WORD);
	add_token_to_list(data, "newline", 7, END);
	token = data->prompt_top;
	while (token->type != END)
	{
		token->value = remove_dollard_quote(token->value);
		token = token->next;
	}
	return (0);
}
