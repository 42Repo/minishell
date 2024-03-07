/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/03/07 10:39:03 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_alias(char *str, int *i, t_data *data)
// get the $text, returns what it should be replaced by
{
	t_env	*tmp;
	char	*alias;
	int		dollar_len;

	tmp = data->env;
	dollar_len = 1;
	while (str[*i + dollar_len] && ft_isalnum(str[*i + dollar_len]))
		dollar_len++;
	alias = NULL;
	while (tmp && ft_strncmp(tmp->name, &str[*i + 1], dollar_len))
		tmp = tmp->next;
	if (tmp)
		alias = ft_strdup(tmp->value);
	else
		alias = ft_strdup("");
	return (alias);
}

char	*alias_remover(char *str, char *alias, char *new_str, int *i)
{
	int	j;
	int	k;

	j = 0;
	while (str[j] && j < *i)
	{
		new_str[j] = str[j];
		j++;
	}
	k = j;
	j = 0;
	while (alias[j])
	{
		new_str[k] = alias[j];
		j++;
		k++;
	}
	while ((int)ft_strlen(str) >= j && str[j])
	{
		new_str[k] = str[j];
		j++;
		k++;
	}
	return (new_str);
}

char	*replace_alias(char *str, int *i, char *alias, t_data *data)
{
	char	*new_str;
	int		dollar_len;

	(void)data;
	dollar_len = 1;
	while (str[*i + dollar_len] && ft_isalnum(str[*i + dollar_len]))
		dollar_len++;
	new_str = ft_calloc(sizeof(char), (ft_strlen(str) + ft_strlen(alias)
				- dollar_len + 1));
	new_str = alias_remover(str, alias, new_str, i);
	(*i) += ft_strlen(alias) + dollar_len;
	free(str);
	return (new_str);
}

char	*check_aliases(char *str, t_data *data)
{
	int		i;
	char	*alias;

	data->quote_state = 0;
	i = 0;
	while (str[i])
	{
		data->quote_state = quote_management(data->quote_state, str[i]);
		if (data->quote_state != 1 && str[i] == '$')
		{
			alias = get_alias(str, &i, data);
			str = replace_alias(str, &i, alias, data);
			if (alias)
				free(alias);
		}
		i++;
	}
	return (remove_quotes(str, data));
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
