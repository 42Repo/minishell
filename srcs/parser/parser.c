/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/20 16:56:26 by mbuchs           ###   ########.fr       */
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
			free(selected->cmd);
		if (selected->args)
			free_tab(selected->args);
		old = selected;
		selected = selected->next;
		if (old)
			free(old);
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

void select_output(char *file, t_data *data, int mode)
{
	if (data->fd_out != 1)
		close(data->fd_out);
	if (data->fd_out != 1)
		data->fd_out = 1;
	if (mode == 1)
		data->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		data->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->fd_out == -1)
		data->fd_out = 1;
}

void	select_input(char *file, t_data *data, t_command *command)
{
	char *line;
	char *tmp = ft_calloc(sizeof(char), 256);

	line = ft_strdup("");
	if (data->fd_in != 0)
		close(data->fd_in);
	data->fd_in = open(file, O_RDONLY);
	if (data->fd_in == -1)
	{
		printf("minishell: %s: No such file or directory\n", file);
		command->cmd = NULL;
	}
	while (read(data->fd_in, tmp, 256))
		line = ft_strjoin(line, tmp);
	command->args = join_tab(command->args, line);
	close(data->fd_in);
	data->fd_in = 0;
}

void	get_redir(t_token *selected, t_data *data, t_command *command)
{
	if (selected->type == REDIR)
	{
		if (selected->next && selected->next->type == WORD)
		{
			if (ft_strlen(selected->value) == 2 && selected->value[1] == '>')
				select_output(selected->next->value, data, 2);
			else if (selected->value[0] == '>')
				select_output(selected->next->value, data, 1);
			else if (ft_strlen(selected->value) == 2 && selected->value[1] == '<')
				printf("faut ouvrir le heredoc mais flemme\n");
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
	t_token *token = data->prompt_top;
	// printf("a\n") ;
	while(token->type != END)
	{
		// printf("token->value before = %s\n", token->value);
		if(token->type == WORD)
			token->value = remove_quotes(token->value, data);
		// printf("token->value after = %s\n", token->value);
		token = token->next;
	}
	
	while (selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		command->next = NULL;
		if (data->prompt_top->type == WORD)
		{
			tmp2 = remove_quotes(selected->value, data);
			tmp = ft_split(tmp2, ' ');
			command->cmd = ft_strdup(tmp[0]);
			if(ft_tablen(tmp) > 1)
				command->args = tmp;
			if(ft_tablen(tmp) > 1)
				selected = selected->next;
		}
		while (selected && selected->type == WORD)
		{
			// printf("selected->value = %s\n", selected->value);
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
		// printf("cmd = %s\n", command->cmd);
		int i = 0;
		while(command->args[i])
		{
			command->args[i] = remove_quotes(command->args[i], data);
			// printf("cmd = %s\n", command->args[i]);
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
