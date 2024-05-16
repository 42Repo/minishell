/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/16 17:16:23 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*check_return_code(t_data *data)
{
	if (signal_received == SIGINT)
	{
		data->g_return_code = 128 + SIGINT;
	}
	return (ft_itoa(data->g_return_code));
}

char	*special_case_envar(int len, char *str, t_data *data) // TODO - fix return code
{
	if (len == -1)
		return (ft_strdup("$"));
	if (len == 1)
		return (ft_strdup(""));
	if ((ft_strlen(str) >= 2 && str[1] == '?'))
		return (check_return_code(data));
	return (NULL);
}

char	*get_envar(char *str, int len, t_data *data)
{
	t_env	*tmp;
	char	*envar;
	int		bool;

	if (len == -1 || len == 1 || (ft_strlen(str) >= 2 && str[1] == '?'))
		return (special_case_envar(len, str, data));
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
		i++;
	}
	free_tab(tab);
	return (tmp);
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
