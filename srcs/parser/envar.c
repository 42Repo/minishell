/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/05 16:54:55 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_return_code;

char *expander(t_data *data)
{
	t_token *selected;
	int	quote_state = 0;

	if (!data->prompt_top)
		return(NULL);
	selected = data->prompt_top;
	while(selected->type != END)
	{
		int i = 0;
		quote_state = 0;
		
		while(selected->value[i])
		{
			quote_management(quote_state, selected->value[i]);
			if ((int)ft_strlen(&selected->value[i]) >= i + 1 && selected->value[i] == '$' && !ft_isdigit(selected->value[i + 1]) && selected->value[i + 1] != '_' && quote_state != 1)
				printf("la faut replace\n");
			i++;
		}
		// printf("%s\n", selected->value);
		selected = selected->next;
	}
	return (NULL);
}

// char	*get_envar(char *str, int *i, t_data *data)
// // get the $text, returns what it should be replaced by
// {
// 	t_env	*tmp;
// 	char	*envar;
// 	int		dollar_len;

// 	if (ft_strlen(str) == 2 && str[*i + 1] == '?')
// 	{
// 		(*i)++;
// 		return (ft_itoa(g_return_code));
// 	}
// 	tmp = data->env;
// 	dollar_len = 1;
// 	while (str[*i + dollar_len] && ft_isalnum(str[*i + dollar_len]))
// 		dollar_len++;
// 	envar = NULL;
// 	while (tmp && ft_strncmp(&str[*i + 1], tmp->name, dollar_len - 1))
// 	{
// 		// printf("tmp->value = %s\n", tmp->value);
// 		tmp = tmp->next;
// 	}
// 	if (tmp)
// 		envar = ft_strdup(tmp->value);
// 	else
// 		envar = ft_strdup("");
// 	printf("envar = %s\n", envar);
// 	(*i) += dollar_len;
// 	return (envar);
// }

// char	*envar_remover(char *str, char *envar, char *new_str, int *i)
// {
// 	int		j;
// 	int		k;

// 	j = 0;
// 	k = 0;
// 	while (str[j] && j < *i)
// 	{
// 		new_str[j] = str[j];
// 		j++;
// 	}
// 	while (envar[k])
// 	{
// 		new_str[j] = envar[k];
// 		j++;
// 		k++;
// 	}
// 	while (str[*i])
// 	{
// 		new_str[j] = str[*i];
// 		j++;
// 		(*i)++;
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }

// char	*replace_envar(char *str, int *i, char *envar, t_data *data)
// {
// 	char	*new_str;
// 	int		dollar_len;

// 	(void)data;
// 	dollar_len = 1;
// 	while (str[*i + dollar_len] && ft_isalnum(str[*i + dollar_len]))
// 		dollar_len++;
// 	new_str = ft_calloc(sizeof(char), (ft_strlen(str) + ft_strlen(envar)
// 				- dollar_len + 1));
// 	new_str = envar_remover(str, envar, new_str, i);
// 	(*i) += ft_strlen(envar) + dollar_len;
// 	free(str);
// 	// printf("new_str = %s\n", new_str);
// 	return (new_str);
// }



// char	*check_envar(char *str, t_data *data)
// {
// 	int		i;
// 	char	*envar;
// 	char *start;
// 	char *mid;
// 	char *end;

// 	data->quote_state = 0;
// 	i = 0;

// 	while(str[i])
// 	{
// 		if(str[i] == '$' && str[i + 1] == '?')
// 			envar_interogation(str, &i, data);
// 		while(str[i + 1] && str[i] != '$' && ft_isdigit(str[i] + 1))
// 			i++;
		
// r	}
// 	// while (str[i])
// 	// {
// 	// 	data->quote_state = quote_management(data->quote_state, str[i]);
// 	// 		// printf("str = %s\n", str);
// 	// 		// printf("str[i] = %c\n", str[i]);
// 	// 	if (data->quote_state != 1 && str[i] == '$')
// 	// 	{
// 	// 		printf("cheh batard\n");
// 	// 		envar = get_envar(str, &i, data);
// 	// 		str = replace_envar(str, &i, envar, data);
// 	// 		printf("str = %s\n", str);
// 	// 		if (envar)
// 	// 			free(envar);
// 	// 	}
// 	// 	i++;
// 	// }
// 	return (remove_quotes(str, data));
// }

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
