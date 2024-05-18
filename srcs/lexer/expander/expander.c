/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/19 00:20:27 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	add_word_to_list(char *str, int *i, int *j, t_data *data)
{
	if (*j < *i)
		add_token_to_list(data, &str[*j], *i - *j, WORD);
	*j = *i + 1;
	return (1);
}

void	check_pipe_redir(char *str, int *i, int *j, t_data *data)
{
	if (data->quote_state != 0)
		return ;
	if (str[*i] == '>' || str[*i] == '<')
		add_redir(str, i, j, data);
	else if (str[*i] == '|')
	{
		if (*j < *i)
			add_token_to_list(data, &str[*j], *i - *j, WORD);
		add_token_to_list(data, &str[*i], 1, PIPE);
		*j = *i + 1;
	}
}

char	**join_tab(char **tab, char *line)
{
	char	**new_tab;
	int		i;
	int		len;

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

int	check_envar(char *str, int i, int quote_state)
{
	return (str[i]
		&& !((int)ft_strlen(str) >= i + 1
			&& str[i] == '$'
			&& (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_'
				|| str[i + 1] == '?'
				|| (str[i + 1] == '"' && quote_state != 2)
				|| str[i + 1] == '\'')
			&& quote_state != 1));
}

char	*expander(t_data *data)
{
	t_token	*selected;
	t_token	*previous;
	int		quote_state;
	int		i;

	selected = data->prompt_top;
	previous = NULL;
	quote_state = 0;
	while (selected->type != END)
	{
		i = 0;
		while (selected->value[i])
		{
			quote_state = quote_management(quote_state, selected->value[i]);
			if (!check_envar(selected->value, i, quote_state))
			{
				set_envar(data, &selected, previous);
				break ;
			}
			i++;
		}
		previous = selected;
		selected = selected->next;
	}
	return (NULL);
}
