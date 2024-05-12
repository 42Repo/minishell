/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:22:03 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/13 00:03:18 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_word(t_token *selected, t_command *command)
{

	// selected->value = remove_quotes(selected->value);
	command->cmd = ft_strdup(selected->value);
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
	if ((*selected)->next->type != WORD && (*selected)->next->type != REDIR)
		return ((*selected)->value);
	// (*command)->next = init_command();
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
		if (token->type == WORD && !(token == data->prompt_top && (!ft_strcmp(token->value, "\"\"") || !ft_strcmp(token->value, "''"))))
		{
			// printf("je remove les quotes de %s\n", token->value);
			token->value = remove_quotes(token->value, 1);
		}
		// printf("token->value: %s\n", token->value);
		// printf("&token->value: %p\n\n", &token->value);
		token = token->next;
	}
}

char	*parse_misc(t_token **selected, t_data *data, \
		t_command *command, t_parser *parser)
{
	if ((*selected)->type == WORD && !command->cmd)
		parse_word((*selected), command);
	if ((*selected)->type == WORD)
		command->args = join_tab(command->args, ft_strdup((*selected)->value));
	else if ((*selected)->type == REDIR)
		parser->error = parse_redir(&(*selected), data, command);
	if (parser->error && (*selected)->type == REDIR)
		return (parser->error);
	(*selected) = (*selected)->next;
	return (NULL);
}
