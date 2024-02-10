/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 08:47:29 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/10 16:14:12 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_token_to_list(t_data *data, char *str, int len, t_token_type type)
{
	ms_lstadd_back(&data->prompt_top,
		ms_lstnew(type, ft_strtrim(set_token_str(str, len), " ")), data);
	ms_lstlast(data->prompt_top)->value = remove_quotes
	(ms_lstlast(data->prompt_top)->value, data);
}

void	add_redir(char *str, int *i, int *j, t_data *data)
{
	if (str[*i] == '>' && str[*i + 1] == '>')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, ">>", 2, REDIR);
		*i += 1;
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, "<<", 2, REDIR);
		*i += 1;
	}
	else if (str[*i] == '>')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, &str[*i], 1, REDIR);
	}
	*j = *i + 1;
}

int	add_word_to_list(char *str, int *i, int *j, t_data *data)
{
	if (*j < *i)
		add_token_to_list(data, &str[*j], *i - *j, WORD);
	*j = *i + 1;
	return (1);
}

void	check_pipe_redir(char *str, int *i, int *j, t_data *data)
{
	if (str[*i] == '>' || str[*i] == '<')
		add_redir(str, i, j, data);
	else if (str[*i] == '|')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, &str[*i], 1, PIPE);
		*j = *i + 1;
	}
}
