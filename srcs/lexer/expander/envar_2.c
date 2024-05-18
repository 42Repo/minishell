/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/18 23:58:18 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	add_non_env(t_token *tmp, int i, int j, char ***tab)
{
	if (!tab[0])
		(*tab)[0] = ft_strndup(&tmp->value[i], j);
	else
		(*tab) = join_tab((*tab), ft_strndup(&tmp->value[i], j));
}

void	skip_not_envar(t_token *tmp, int *i, int *j, int *quote_state)
{
	while (check_envar(tmp->value, (*i), (*quote_state)))
	{
		(*quote_state) = quote_management((*quote_state), tmp->value[(*i)]);
		(*i)++;
		(*j)++;
	}
}

void	check_quote_state(t_token *tmp, int *quote_state, int *i, int *j)
{
	if (tmp && tmp->value && tmp->value[(*i)]
		&& tmp->value[(*i)] + 1 && (((*quote_state) == 2
				&& tmp->value[(*i) + 1] == '"')
			|| ((*quote_state) == 1 && tmp->value[(*i) + 1] == '\'')))
	{
		(*quote_state) = 0;
		(*j)++;
		(*i)++;
	}
}

char	*replace_envar(t_data *data, t_token *tmp, int i, char ***tab)
{
	int		j;
	int		quote_state;

	j = 0;
	quote_state = 0;
	while (tmp->value[i])
	{
		skip_not_envar(tmp, &i, &j, &quote_state);
		check_quote_state(tmp, &quote_state, &i, &j);
		if (j > 0)
			add_non_env(tmp, i - j, j, tab);
		if (j > 0)
			if (!tmp->value[i])
				j = -1;
		if (j < 0)
			break ;
		if (tmp->value[i] == '$')
		{
			(*tab) = join_tab((*tab), get_envar(&tmp->value[i],
						get_envar_len(&tmp->value[i]), data));
			i += get_envar_len(&tmp->value[i]);
			j = 0;
		}
	}
	return (join_replaced((*tab)));
}

void	set_envar(t_data *data, t_token **selected, t_token *previous)
{
	int		j;
	char	**tmp;
	char	*str;

	tmp = ft_calloc(sizeof(char *), 1);
	str = replace_envar(data, (*selected), 0, &tmp);
	if (str && previous && previous->type == REDIR
		&& (is_space_inside(str) || ft_strlen(str) == 0))
		is_ambiguous(selected, previous, str);
	tmp = ft_split_quote_state(str, " \t\n");
	free((*selected)->value);
	(*selected)->value = tmp[0];
	j = 1;
	while (tmp && *tmp && tmp[j])
	{
		add_token_next((*selected), tmp[j], ft_strlen(tmp[j]), WORD);
		free (tmp[j]);
		(*selected) = (*selected)->next;
		j++;
	}
	free(tmp);
	tmp = NULL;
}
