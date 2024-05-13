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
	char	*tmp1;
	char	*tmp2;

	tmp1 = set_token_str(str, len);
	tmp2 = ft_strtrim(tmp1, " \t\n\v\f\r");
	free(tmp1);
	if (ft_strlen(tmp2) == 0)
	{
		free(tmp2);
		return ;
	}
	ms_lstadd_back(&data->prompt_top,
		ms_lstnew(type, tmp2), data);
}

void	add_token_next(t_token *token, char *str, int len, t_token_type type)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = set_token_str(str, len);
	tmp2 = ft_strtrim(tmp1, " \t\n\v\f\r");
	free(tmp1);
	if (ft_strlen(tmp2) == 0)
	{
		free(tmp2);
		return ;
	}
	ms_lstadd_next(token,
		ms_lstnew(type, tmp2));
}

t_token	*ms_lstadd_next(t_token *token, t_token *new)
{
	t_token	*tmp;

	if (!token || !new)
		return (NULL);
	tmp = token->next;
	token->next = new;
	new->next = tmp;
	return (new);
}

void	redir_outfile(char *str, int *i, int *j, t_data *data)
{
	if (str[*i] == '<' && str[*i + 1] == '<')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, "<<", 2, REDIR);
		*i += 1;
	}
	else if (str[*i] == '<')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, &str[*i], 1, REDIR);
	}
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
	else if (str[*i] == '>')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, &str[*i], 1, REDIR);
	}
	else
		redir_outfile(str, i, j, data);
	*j = *i + 1;
}
