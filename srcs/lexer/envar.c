/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/19 14:41:15 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_return_code;

char	*get_envar(char *str, int len, t_data *data)
// get the $text, returns what it should be replaced by
{
	t_env	*tmp;
	char	*envar;
	
	if (len == 1)
		return(ft_strdup("$"));
	if (ft_strlen(str) == 2 && str[1] == '?')
		return (ft_itoa(g_return_code));
	tmp = data->env;
	envar = NULL;
	while (tmp && ft_strncmp(&str[1], tmp->name, len - 1))
		tmp = tmp->next;
	if (tmp)
		envar = ft_strdup(tmp->value);
	else
		envar = ft_strdup("");
	return (envar);
}

int	get_envar_len(char *str)
{
	int i = 1;
	if(str[i] == '?')
		return (2);
	while (str[i] && ft_isalnum(str[i]))
		i++;
	return (i);
}

char	*replace_envar(t_data *data, t_token *selected, int i)
{
	char *str[3];
	int j = 0;
	int k = 0;
	
	(void) data;
	str[0] = ft_calloc(sizeof(char), i + 1);
	while (j < i)
	{
		str[0][j] = selected->value[j];
		j++;
	}
	str[1] = get_envar(&selected->value[i], get_envar_len(&selected->value[i]), data);
	i += get_envar_len(&selected->value[i]);
	str[2] = ft_calloc(sizeof(char), ft_strlen(&selected->value[i]) + 1);
	while (selected->value[i])
	{
		str[2][k] = selected->value[i];
		k++;
		i++;
	}
	str[0] = ft_strjoin_free(str[0], str[1]);
	free(str[1]);
	str[0] = ft_strjoin_free(str[0], str[2]);
	free(str[2]);
	return (str[0]);
}

char	*expander(t_data *data)
{
	t_token *selected;
	int	quote_state = 0;

	if (!data->prompt_top)
		return(NULL);
	selected = data->prompt_top;
	while (selected->type != END)
	{
		int i = 0;
		quote_state = 0;
		// printf("%s\n", selected->value);
		while (i < (int) ft_strlen(selected->value) && selected->value[i])
		{
			quote_state = quote_management(quote_state, selected->value[i]);
			if ((int)ft_strlen(selected->value) >= i + 1 && selected->value[i] == '$' \
				&& !ft_isdigit(selected->value[i + 1]) && selected->value[i + 1] != '_' && quote_state != 1)
				selected->value = replace_envar(data, selected, i);
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
