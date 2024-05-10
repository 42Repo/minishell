/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/09 23:21:24 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_dir(char *file, t_command *command)
{
	int			i;
	char		*end;
	char		*dir;
	struct stat	sb;

	i = 0;
	stat(file, &sb);
	if (!access(file, F_OK))
	{
		if (S_ISDIR(sb.st_mode))
		{
			put_error("minishell: ", file, ": Is a directory\n");
			g_return_code = 1;
			command->fd_out = -1;
			command->cmd = NULL;
			return (1);
		}
		if (access(file, R_OK))
		{
			put_error("minishell: ", file, ": Permission denied\n");
			g_return_code = 1;
			command->fd_out = -1;
			command->cmd = NULL;
			return (1);
		}
	}
	end = ft_strrchr(file, '/');
	if (!end)
		return (0);
	while (&file[i] != end)
		i++;
	dir = ft_strndup(&file[0], i);
	stat(dir, &sb);
	if (access(dir, F_OK))
	{
		put_error("minishell: ",  dir, ": No such file or directory\n");
		g_return_code = 1;
		command->fd_out = -1;
		command->cmd = NULL;
		free(dir);
		return (1);
	}
	if (!(S_ISDIR(sb.st_mode)))
	{
		put_error("minishell: ", dir, ": Is a directory\n");
		g_return_code = 1;
		command->fd_out = -1;
		command->cmd = NULL;
		free(dir);
		return (1);
	}
	free(dir);
	return (0);
}


void	select_output(char *file, int mode, t_command *command)
{
	if (command->fd_out != 1)
		close(command->fd_out);
	if (command->fd_out != 1)
		command->fd_out = 1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
	{
		put_error("minishell: ", file, ": Permission denied\n");
		command->fd_out = -1;
		g_return_code = 1;
		command->cmd = NULL;
		return ;
	}
	if (check_dir(file, command))
		return ;
	if (mode == 1)
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

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
		command->fd_out = -1;
		g_return_code = 1;
		return ;
	}
	if (access(file, R_OK) == -1)
	{
		put_error("minishell: ", file, ": Permission denied\n");
		g_return_code = 126;
		command->fd_out = -1;
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
				select_output(selected->next->value, 2, command);
			else if (selected->value[0] == '>')
				select_output(selected->next->value, 1, command);
			else if (ft_strcmp(selected->value, "<<") == 0)
			{
				return ;
			}
			else if (selected->value[0] == '<')
			{
				select_input(selected->next->value, data, command);
			}
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
			if (command->fd_out != -1 && command->fd_in != -1 && command->fd_heredoc != -2)
				parser.error = parse_misc(&selected, data, command, &parser);
			else
			{
				selected = selected->next;
				parser.error = NULL;
			}
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

void	open_heredoc(t_data *data)
{
	t_token	*selected;
	t_token	*tmp;
	t_command	*command;
	
	command = data->command_top;
	selected = data->prompt_top;
	while (selected)
	{
		if (selected->type == REDIR && selected->next && \
			ft_strlen(selected->value) == 2 && selected->value[1] == '<')
		{
			if (selected->type == PIPE)
				command = command->next;
			tmp = selected->next;
			if (tmp->type == WORD)
			{
				heredoc(tmp->value, data, command);
				tmp = tmp->next;
			}
		}
		selected = selected->next;
	}
}

void	set_fd_in(t_command *command, t_token *token)
{
	t_token	*tmp;
	t_token	*last_redir;
	t_command *selected;
	
	selected = command;
	tmp = token;
	last_redir = NULL;
	while (selected)
	{	
		while (tmp->type != END && tmp->type != PIPE)
		{
			if (tmp->type == REDIR)
				last_redir = tmp;
				
			tmp = tmp->next;
		}
		if(last_redir && !ft_strcmp(last_redir->value, "<<") && selected->fd_heredoc != -1)
			selected->fd_in = selected->fd_heredoc;
		if (tmp->type != END)
			tmp = tmp->next;
		// printf("fd_in : %d\n", selected->fd_in);
		selected= selected->next;
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
	open_heredoc(data);
	expander(data);
	clear_token_quotes(data);
	// remove_empty_tokens(data);
	error = parse_line(data, selected, command);
	if (error)
	{
		put_error("minishell: syntax error near \
unexpected token `", error, "'\n");
		g_return_code = 2;
		return (-1);
	}
	command = data->command_top;
	
	set_fd_in(data->command_top, data->prompt_top);
	// printf("fd_in : %d\n", data->command_top->fd_in);
	// printf("fd_out : %d\n", data->command_top->fd_out);

	return (0);
	// ! mettre le bon fd , soit garder fdin, soit close fdin et mettre fdheredoc a la place
}
