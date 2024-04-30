/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:22:03 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/30 15:24:00 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_word(t_token *selected, t_command *command, \
					char *tmp2, char **tmp)
{
	selected->value = remove_quotes(selected->value);
	tmp2 = ft_strdup(selected->value);
	tmp = ft_split(tmp2, ' ');
	command->cmd = ft_strdup(tmp[0]);
	if (ft_tablen(tmp) > 2)
		command->args = tmp;
	if (ft_tablen(tmp) > 2)
		selected = selected->next;
}

char	*parse_redir(t_token **selected, t_data *data, t_command *command)
{
	if ((*selected)->next && (*selected)->next->type == END)
		return ((*selected)->next->value);
	if (!(*selected)->next || ((*selected)->next && \
		(*selected)->next->type != WORD))
		return ((*selected)->value);
	get_redir((*selected), data, command);
	(*selected) = (*selected)->next;
	return (NULL);
}

char	*parse_pipe(t_token **selected, t_command **command)
{
	if ((*selected)->next && (*selected)->next->type == REDIR && \
		(*selected)->next->value[0] == '>')
		return ((*selected)->next->value);
	if ((*selected)->next->type != WORD)
		return ((*selected)->value);
	(*command)->next = init_command();
	(*command) = (*command)->next;
	(*selected) = (*selected)->next;
	return (NULL);
}

void	clear_token_quotes(t_data *data)
{
	t_token	*token;

	token = data->prompt_top;
	while (token->type != END)
	{
		if (token->type == WORD)
			token->value = remove_quotes(token->value);
		token = token->next;
	}
}

char	*parse_misc(t_token **selected, t_data *data, \
		t_command *command, t_parser *parser)
{
	if ((*selected)->type == WORD && !command->cmd)
		parse_word((*selected), command, parser->tmp2, parser->tmp);
	if ((*selected)->type == WORD)
		command->args = join_tab(command->args, ft_strdup((*selected)->value));
	else if ((*selected)->type == REDIR)
		parser->error = parse_redir(&(*selected), data, command);
	if (parser->error && (*selected)->type == REDIR)
		return (parser->error);
	(*selected) = (*selected)->next;
	return (NULL);
}
