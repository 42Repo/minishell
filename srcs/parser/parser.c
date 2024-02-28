/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/28 02:15:16 by mbuchs           ###   ########.fr       */
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

	command = malloc(sizeof(t_command));
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
	printf("file = %s\n", file);
	if (mode == 1)
		data->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		data->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	printf("fd_out = %d\n", data->fd_out);
	if (data->fd_out == -1)
		data->fd_out = 1;
}

void	get_redir(t_token *selected, t_data *data)
{
	if (selected->type == REDIR)
	{
		if (selected->next->type == WORD)
		{
			if (ft_strlen(selected->value) == 2 && selected->value[1] == '>')
				select_output(selected->next->value, data, 2);
			else if (selected->value[0] == '>')
				select_output(selected->next->value, data, 1);
		}
	}
}

void	parse_line(t_data *data, t_token *selected, t_command *command)
{
	while (selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		command->next = NULL;
		if (data->prompt_top->type == WORD)
			command->cmd = check_aliases(ft_strdup(selected->value), data);
		while (selected && selected->type == WORD)
		{
			command->args = join_tab(command->args, \
				check_aliases(ft_strdup(selected->value), data));
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
			get_redir(selected, data);
			selected = selected->next->next;
		}
		if (selected->type == END)
			return ;
	}
}

void	parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;

	if (data->command_top)
		free_command(data);
	data->command_top = init_command();
	selected = data->prompt_top;
	command = data->command_top;
	parse_line(data, selected, command);
	printf("fd_out = %d\n", data->fd_out);
	dup2(1, data->fd_out);
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