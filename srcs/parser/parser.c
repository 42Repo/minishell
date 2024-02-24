/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/24 21:04:21 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_tablen(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char **join_tab(char **tab, char *line)
{
	char **new_tab;
	int i;
	int len;

	len = ft_tablen(tab);
	new_tab = ft_calloc(sizeof(char *), (len + 2));
	if (new_tab == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_tab[i] = tab[i];
		i++;
	}
	new_tab[i] = line;
	new_tab[i + 1] = NULL;
	// free(tab);
	return (new_tab);
}


void parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;
	data->command_top = malloc(sizeof(t_command));
	selected = data->prompt_top;
	
	command = data->command_top;
	while(selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		if (data->prompt_top->type == WORD)
			command->cmd = ft_strdup(selected->value);
		// selected = selected->next;
		while (selected && selected->type == WORD)
		{
			command->args = join_tab(command->args, ft_strdup(selected->value));
			selected = selected->next;
		}
		if (selected->type == PIPE)
		{
			command->next = malloc(sizeof(t_command *));
			command = command->next;
			selected = selected->next;
		}
		// printf("cmd = %s\n", data->command_top->cmd);
		// printf("arg 1 = %s\n", data->command_top->args[0]);
		// printf("arg 2 = %s\n", data->command_top->args[1]);
		
		if (selected->type == END)
			return ;
	}
// printf("cmd = %s\n", data->command_top->next->cmd);
// 		printf("arg 1 = %s\n", data->command_top->next->args[0]);
// 		printf("arg 2 = %s\n", data->command_top->next->args[1]);
		
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
}