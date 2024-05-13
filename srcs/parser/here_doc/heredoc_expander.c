/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:26:52 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 21:27:56 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	add_non_env_text(char ***tab, char *str, int start, int *length)
{
	if ((*length) > 0)
	{
		if (!(*tab)[0])
			(*tab)[0] = ft_strndup(&str[start - (*length)], (*length));
		else
			(*tab) = join_tab((*tab),
					ft_strndup(&str[start - (*length)], (*length)));
		(*length) = 0;
		if (!str[start])
			(*length) = -1;
	}
}

void	init_var(int *i, int *j, char ***tab)
{
	*i = 0;
	*j = 0;
	*tab = ft_calloc(sizeof(char *), 2);
}

void	skip_envar(char *str, int *i, int *j)
{
	while (check_envar(str, *i, 0))
	{
		(*i)++;
		(*j)++;
	}
}

char	*expand_heredoc(char *str, t_data *data)
{
	int		i;
	int		j;
	char	**tab;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	tab = ft_calloc(sizeof(char *), 2);
	while (str[i])
	{
		skip_envar(str, &i, &j);
		add_non_env_text(&tab, str, i, &j);
		if (j < 0)
			break ;
		if (str[i] == '$')
		{
			tab = join_tab(tab, get_envar(&str[i],
						get_envar_len(&str[i]), data));
			i += get_envar_len(&str[i]);
			j = 0;
		}
	}
	return (join_replaced(tab));
}
