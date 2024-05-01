/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/01 17:33:59 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	select_output(char *file, t_data *data, int mode, t_command *command)
{
	(void) data;
	if (command->fd_out != 1)
		close(command->fd_out);
	if (command->fd_out != 1)
		command->fd_out = 1;
	if (mode == 1)
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (command->fd_out == -1)
		command->fd_out = 1;
}
	// gerer erreurs

void	select_input(char *file, t_data *data, t_command *command)
{
	(void) data;
	if (command->fd_in != 0)
		close(command->fd_in);
	if (command->fd_in != 0)
		command->fd_in = 0;
	if (access(file, F_OK) == -1)
	{
		put_error("minishell: ", file, ": No such file or directory\n");
		command->cmd = NULL;
		return ;
	}
	if (access(file, R_OK) == -1)
	{
		put_error("minishell: ", file, ": Permission denied\n");
		command->cmd = NULL;
		return ;
	}
	command->fd_in = open(file, O_RDONLY);
	if (command->fd_in == -1)
		command->fd_in = 0;
}
	// gerer erreurs

void	get_redir(t_token *selected, t_data *data, t_command *command)
{
	if (selected->type == REDIR)
	{
		if (selected->next && selected->next->type == WORD)
		{
			if (ft_strlen(selected->value) == 2 && selected->value[1] == '>')
				select_output(selected->next->value, data, 2, command);
			else if (selected->value[0] == '>')
				select_output(selected->next->value, data, 1, command);
			else if (ft_strlen(selected->value) == 2 && \
					selected->value[1] == '<')
				heredoc(selected->next->value, data, command);
			else if (selected->value[0] == '<')
				select_input(selected->next->value, data, command);
		}
		else
		{
			ft_putstr_fd("minishell: syntax error near unexpected \
'HAHAHA'\n", 2);
			command->cmd = NULL;
		}
	}
}

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
		command->next = NULL;
		while (selected && selected->type != PIPE)
		{
			parser.error = parse_misc(&selected, data, command, &parser);
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

int	parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;
	char		*error;

	data->command_top = init_command();
	selected = data->prompt_top;
	command = data->command_top;
	clear_token_quotes(data);
	remove_empty_tokens(data);
	error = parse_line(data, selected, command);
	if (error)
	{
		put_error("minishell: syntax error near \
unexpected token `", error, "'\n");
		g_return_code = 2;
		return (-1);
	}
	command = data->command_top;
	return (0);
}
