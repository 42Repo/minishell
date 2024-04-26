/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/26 16:33:58 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	free_command(t_data *data)
{
	t_command	*selected;
	t_command	*old;

	selected = data->command_top;
	while (selected)
	{
		if (selected->cmd)
		{
			free(selected->cmd);
			selected->cmd = NULL;
		}
		if (selected->args)
		{
			free_tab(selected->args);
			selected->args = NULL;
		}
		old = selected;
		selected = selected->next;
		if (old)
		{
			free(old);
			old = NULL;
		}
		data->command_top = selected;
	}
}

t_command	*init_command(void)
{
	t_command	*command;

	command = ft_calloc(sizeof(t_command), 1);
	command->cmd = NULL;
	command->args = NULL;
	command->next = NULL;
	return (command);
}

void select_output(char *file, t_data *data, int mode, t_command *command)
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
	// gerer erreurs
}

void	select_input(char *file, t_data *data, t_command *command)
{
	(void) data;
	if (command->fd_in != 0)
		close(command->fd_in);
	if (command->fd_in != 0)
		command->fd_in = 0;
	command->fd_in = open(file, O_RDONLY);
	if (command->fd_in == -1)
		command->fd_in = 0;
	// gerer erreurs
}

void	heredoc(char *file, t_data *data, t_command *command)
{
	char	*line;
	int		fd;

	(void) data;
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", file, strerror(errno));
		command->cmd = NULL;
		return ;
	}
	line = readline("> ");
	while (line && ft_strcmp(line, file) != 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(fd);
	printf("pranked ca marche pas\n");
}

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
			else if (ft_strlen(selected->value) == 2 && selected->value[1] == '<')
				heredoc(selected->next->value, data, command);
			else if (selected->value[0] == '<')
				select_input(selected->next->value, data, command);
			else
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				command->cmd = NULL;
			}
		}
		else
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			command->cmd = NULL;
		}
	}
}

void	parse_line(t_data *data, t_token *selected, t_command *command)
{
	char	**tmp;
	char *tmp2;
	t_token *token;
	
	token = data->prompt_top;
	while(token->type != END)
	{
		if(token->type == WORD)
			token->value = remove_quotes(token->value, data);
		token = token->next;
	}
	selected = data->prompt_top;
	while (selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		command->next = NULL;
		if (data->prompt_top->type == WORD)
		{
			selected->value =remove_quotes(selected->value, data);
			tmp2 = ft_strdup(selected->value);
			tmp = ft_split(tmp2, ' ');
			command->cmd = ft_strdup(tmp[0]);
			
			if(ft_tablen(tmp) > 2)
				command->args = tmp;
			if(ft_tablen(tmp) > 2)
				selected = selected->next;
		}
		while (selected && selected->type == WORD)
		{
			command->args = join_tab(command->args, ft_strdup(selected->value));
			selected = selected->next;
		}
		if (selected->type == PIPE)
		{
			command->next = init_command();
			command = command->next;
			selected = selected->next;
		}
		if (selected->type == REDIR)
		{
			get_redir(selected, data, command);
			selected = selected->next->next;
		}
		if (selected && selected->type == END)
			return ;
	}
}

void	parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;

	data->command_top = init_command();
	selected = data->prompt_top;
	command = data->command_top;
	parse_line(data, selected, command);
	command = data->command_top;
	while (command->next)
	{
		command->cmd = remove_quotes(command->cmd, data);
		int i = 0;
		while(command->args[i])
		{
			command->args[i] = remove_quotes(command->args[i], data);
			i++;
		}
		command = command->next;
	}
}

// if ->top node == word
// 	command_top == word
// while -> node->type == WORD
// 	join_tab to command->top->args
	//
// if << -> heredoc

// if < -> if next == word
// 	dup2 "word" in args

// if pipe
// 	pipex

// if >> OR >
// 	change output to file
