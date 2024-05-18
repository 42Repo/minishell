/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/19 00:16:22 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*parse_line(t_data *data, t_token *selected, t_command *command)
{
	t_parser	parser;

	parser = (t_parser){.tmp = NULL, .tmp2 = NULL, .error = NULL};
	selected = data->prompt_top;
	if (selected->type == PIPE)
		return (selected->value);
	while (selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		while (selected && selected->type != PIPE)
		{
			parser.error = skip_pipe(&command, &selected, &parser, data);
			if (parser.error)
				return (parser.error);
		}
		if (selected && selected->type == PIPE)
			parser.error = parse_pipe(&selected, &command);
		if (parser.error && selected->type == PIPE)
			return (parser.error);
		if (selected && selected->type == END)
			return (NULL);
	}
	return (NULL);
}

int	open_heredoc(t_data *data)
{
	t_token		*selected;
	t_command	*command;

	command = data->command_top;
	selected = data->prompt_top;
	while (selected && selected->type != END)
	{
		if (selected->type == PIPE)
		{
			command->next = init_command();
			command = command->next;
		}
		else if (selected->type == REDIR
			&& selected->next && selected->next->type == WORD
			&& !ft_strcmp(selected->value, "<<"))
		{
			if (heredoc(selected->next->value, data, command) == -1)
				return (-1);
		}
		selected = selected->next;
	}
	return (0);
}

void	count_heredoc(t_data *data)
{
	t_token		*selected;
	int			count;

	count = 0;
	selected = data->prompt_top;
	while (selected)
	{
		if (selected->type == REDIR && selected->next
			&& selected->next->type == WORD
			&& !ft_strcmp(selected->value, "<<"))
			count++;
		selected = selected->next;
	}
	if (count >= 16)
	{
		data->g_return_code = 2;
		free(data->line);
		ft_exit(data->command_top, data,
			"minishell: error: too many here documents", 1);
	}
}

int	parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;
	char		*error;

	data->command_top = init_command();
	selected = data->prompt_top;
	command = data->command_top;
	if (open_heredoc(data) == -1)
		return (-1);
	expander(data);
	clear_token_quotes(data);
	error = parse_line(data, selected, command);
	if (error)
	{
		put_error("minishell: syntax error near \
unexpected token `", error, "'\n");
		data->g_return_code = 2;
		return (-1);
	}
	command = data->command_top;
	set_fd_in(data->command_top, data->prompt_top);
	return (0);
}
