/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/23 23:11:27 by mbuchs           ###   ########.fr       */
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
	t_token *selected;

	data->command_top = malloc(sizeof(t_command));
	data->command_top->args = ft_calloc(sizeof(char **), 1);
	if (data->prompt_top->type == WORD)
		data->command_top->cmd = ft_strdup(data->prompt_top->value);
	selected = data->prompt_top->next;
	while (selected->type == WORD)
	{
		data->command_top->args = join_tab(data->command_top->args, ft_strdup(selected->value));
		selected = selected->next;
	}

	printf("cmd = %s\n", data->command_top->cmd);
	printf("arg 1 = %s\n", data->command_top->args[0]);
	printf("arg 2 = %s\n", data->command_top->args[1]);


// if ->top node == word
// 	command_top == word
// while -> node->type == WORD
// 	join_tab to command->top->args
	// 
// if << -> heredoc

// if < -> if next == word
// 	cat "word" in args

// if pipe
// 	pipex

// if >> OR >
// 	change output to file
}