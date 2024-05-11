/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/11 01:44:25 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

char	*expander(t_data *data)
{
	t_token	*selected;
	int		quote_state;
	int		i;

	selected = data->prompt_top;
	quote_state = 0;
	while (selected->type != END)
	{
		i = 0;
		while (selected->value[i])
		{
			quote_state = quote_management(quote_state, selected->value[i]);
			if (check_envar(selected, i, quote_state))
			{
				set_envar(data, selected);
				break ;
			}
			i++;
		}
		selected = selected->next;
	}
	return (NULL);
}
