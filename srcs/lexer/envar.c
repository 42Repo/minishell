/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/11 01:01:27 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *special_case_envar(int len, char *str)
{
	if (len == -1)
		return (ft_strdup("$"));
	if (len == 1)
		return (ft_strdup(""));
	if ((ft_strlen(str) >= 2 && str[1] == '?'))
		return (ft_itoa(g_return_code));
	return (NULL);
}

char	*get_envar(char *str, int len, t_data *data)
{
	t_env	*tmp;
	char	*envar;
	int		bool;

	if (len == -1 || len == 1 || (ft_strlen(str) >= 2 && str[1] == '?'))
		return (special_case_envar(len, str));
	tmp = data->env;
	envar = NULL;
	bool = 0;
	while (tmp && !bool)
	{
		if (!ft_strncmp(&str[1], tmp->name, len - 1))
		{
			bool = 1;
			if ((int)ft_strlen(tmp->name) != len - 1)
				bool = 0;
		}
		if (!bool)
			tmp = tmp->next;
	}
	if (tmp)
		envar = ft_strdup(tmp->value);
	else
		envar = ft_strdup("");
	return (envar);
}

int	get_envar_len(char *str)
{
	int	i;

	i = 1;
	if (str[i] == '\0')
		return (-1);
	if (str[i] == '?')
		return (2);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*join_replaced(char **tab)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup("");
	while (tab[i])
	{
		tmp = ft_strjoin_free(tmp, tab[i]);
		free(tab[i]);
		i++;
	}
	return (tmp);
}

int	check_envar(t_token *selected, int i, int quote_state)
{
	return (selected->value[i]
		&& !((int)ft_strlen(selected->value) >= i + 1
			&& selected->value[i] == '$'
			&& (ft_isalpha(selected->value[i + 1])
				|| selected->value[i + 1] == '_'
				|| selected->value[i + 1] == '?'
				|| (selected->value[i + 1] == '"' && quote_state != 2)
				|| selected->value[i + 1] == '\'')
			&& quote_state != 1));
}

void	add_non_env(t_token *tmp, int i, int j, char ***tab)
{
	if (!tab[0])
		(*tab)[0] = ft_strndup(&tmp->value[i], j);
	else
		(*tab) = join_tab((*tab), ft_strndup(&tmp->value[i], j));
}

void skip_not_envar(t_token *tmp, int *i, int *j, int *quote_state)
{
	while (check_envar(tmp, (*i), (*quote_state)))
	{
		(*quote_state) = quote_management((*quote_state), tmp->value[(*i)]);
		(*i)++;
		(*j)++;
	}
}

void check_quote_state(t_token *tmp, int *quote_state, int *i, int *j)
{
	if ((((*quote_state) == 2 && tmp->value[(*i) + 1] == '"')
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

void	set_envar(t_data *data, t_token *selected)
{
	int		j;
	char	**tmp;

	tmp = ft_calloc(sizeof(char *), 1);
	tmp = ft_split_quote_state(replace_envar(data, selected, 0, &tmp), " \t\n");
	selected->value = tmp[0];
	j = 1;
	while (tmp[j])
	{
		add_token_next(selected, tmp[j], ft_strlen(tmp[j]), WORD);
		selected = selected->next;
		j++;
	}
}

char	*expander(t_data *data)
{
	t_token	*selected;
	t_token	*previous;
	int		quote_state;
	int		i;

	previous = NULL;
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
