/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/26 12:24:08 by mbuchs           ###   ########.fr       */
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
	free(tab);
	return (new_tab);
}


char *get_alias(char *str, int *i, t_data *data)
{
	(void) str;
	(void) i;
	(void) data; 
	//get the $text, returns what it should be replaced by
	return (NULL);
}

char *replace_alias(char *str, int *i, char *alias, t_data *data)
{
	char *new_str;
	int	dollar_len;
	int j;
	int	k;
	
	(void) data;
	dollar_len = 1;

	while (str[*i + dollar_len] && ft_isalnum(str[*i + dollar_len]))
		dollar_len++;

	// printf("dollar_len = %d\n", dollar_len);
	new_str = ft_calloc(sizeof(char), (ft_strlen(str) + ft_strlen(alias) - dollar_len + 1));
	j = 0;
	while(str[j] && j < *i)
	{
		new_str[j] = str[j];
		j++;
	}
	k = j;
	j = 0;
	while(alias[j])
	{
		new_str[k] = alias[j];
		j++;
		k++;
	}
	j = *i + dollar_len;
	while((int)ft_strlen(str) >= j && str[j])
	{
		new_str[k] = str[j];
		j++;
		k++;
	}
	(*i) += ft_strlen(alias);
	free(str);
	return (new_str);
}

char *check_aliases(char *str, t_data *data)
{
	int	i;
	char *alias;
	
	data->quote_state = 0;
	i = 0;
	while (str[i])
	{
		data->quote_state = quote_management(data->quote_state, str[i]);
		if (data->quote_state != 1 && str[i] == '$')
		{
			alias = get_alias(str, &i, data);
			(void) alias;
			str = replace_alias(str, &i, "replaced", data);
		}
		i++;
	}
	return (remove_quotes(str, data));
}

void free_command(t_data *data)
{
	t_command *selected;
	t_command *old;
	
	selected = data->command_top;

	while(selected)
	{
		if(selected->cmd)
			free(selected->cmd);
		if(selected->args)
			free_tab(selected->args);
		old = selected;
		selected = selected->next;
		if (old)
			free(old);
		printf("cleared cmd\n");
	}
}

t_command *init_command()
{
	t_command *command;

	command = malloc(sizeof(t_command));
	command->cmd = NULL;	
	command->args = NULL;
	command->next = NULL;
	return (command);
}

void parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;

	if (data->command_top)
		free_command(data);
	data->command_top = init_command();
	selected = data->prompt_top;
	command = data->command_top;
	while(selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		command->next = NULL;
		if (data->prompt_top->type == WORD)
			command->cmd = check_aliases(ft_strdup(selected->value), data);
		// selected = selected->next;
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
		if (selected->type == END)
			return ;
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
}
