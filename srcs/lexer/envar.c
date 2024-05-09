/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:02:13 by mbuchs            #+#    #+#             */
/*   Updated: 2024/05/09 16:16:33 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_envar(char *str, int len, t_data *data)
{
	t_env	*tmp;
	char	*envar;
	int		bool;
	str = remove_quotes(str, 0);
	if (len == 1)
		return (ft_strdup("$"));
	// if (str[1] == '"' || str[1] == '\'')
	if ((ft_strlen(str) >= 2 && str[1] == '?'))
		return (ft_itoa(g_return_code));
	tmp = data->env;
	envar = NULL;
	// printf("&str[1] = %s\n", &str[1]);
	// printf("len = %d\n", len);
	bool = 0;
	while (tmp && !bool)
	{
		// tmp = tmp->next;
		if (!ft_strncmp(&str[1], tmp->name, len - 1))
		{
			bool = 1;
			// printf("FOUND %s\n", tmp->name);
			// printf("tmp->name = %s\n", tmp->name);

			if ((int)ft_strlen(tmp->name) != len - 1)
			{
				bool = 0;
				// printf("INCORRECT %s\n", tmp->name);
			}
		}
		if (tmp)
			// printf("tmp->name = %s\n", tmp->name);
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
	if (str[i] == '?')
		return (2);
	// if (str[i] == '"' || str[i] == '\'')
		// return (2);
	while (str[i] && ft_isalnum(str[i]) && str[i] != '_')
		i++;
	return (i);
}

// char	*replace_envar(t_data *data, t_token *selected, int i)
// {
// 	char	*str[3];
// 	int		k;

// 	k = 0;
// 	str[0] = ft_calloc(sizeof(char), i + 1);
// 	printf("str[0] = %s\n", str[0]);
// 	ft_strlcpy(str[0], selected->value, i + 1);
// 	// str[1] = get_envar(remove_quotes(&selected->value[i], 0), \
// 		// get_envar_len(remove_quotes(&selected->value[i], 0)), data);
// 	// i += get_envar_len(remove_quotes(&selected->value[i], 0));
// 	str[1] = get_envar(&selected->value[i], \
// 		get_envar_len(&selected->value[i]), data);
// 	i += get_envar_len(&selected->value[i]);
// 	str[2] = ft_calloc(sizeof(char), ft_strlen(&selected->value[i]) + 1);
// 	while (selected->value[i])
// 	{
// 		str[2][k] = selected->value[i];
// 		k++;
// 		i++;
// 	}
// 	str[0] = ft_strjoin_free(str[0], str[1]);
// 	free(str[1]);
// 	str[0] = ft_strjoin_free(str[0], str[2]);
// 	free(str[2]);
// 	return (str[0]);
// }

char	*replace_envar(t_data *data, t_token *selected, int i)
{
	(void) data;
	// char	*tmp;
	char	**tab = ft_calloc(sizeof(char *), 2);
	int		j = 0;
	int z = 0;  // ! DEBUG
	// int 	bol = 0;
	int		quote_state = 0;
	// int		k = 0;
	// tmp = ft_strdup(selected->value);
	// tab = NULL;
	
	while(selected->value[i])
	{
		// quote_state = quote_management(quote_state, selected->value[i]);
		while (selected->value[i] && !((int)ft_strlen(selected->value) >= i + 1 && \
				selected->value[i] == '$' \
				&& (ft_isalpha(selected->value[i + 1]) || selected->value[i + 1] == '?' \
				|| selected->value[i + 1] == '"' || selected->value[i + 1] == '\'') && \
				 quote_state != 1))
		{
			quote_state = quote_management(quote_state, selected->value[i]);
			i++;
			j++;
		}
		if (j > 1)
		{
			if (!tab[0])
				tab[0] = ft_strndup(&selected->value[i-j], j);
			else
				tab = join_tab(tab, ft_strndup(&selected->value[i - j],  j));
			// if (!selected->value[i])
			// 	printf("jme casse\n");
			if (!selected->value[i])
				j = -1;
			// j = 0;
			// printf("hors envar de len %d\n", j);
		}
		if (j < 0)
			break ;
		if (selected->value[i] == '$')
		{
			// printf("selected->value[%d] = %c\n", i, selected->value[i]);
			tab = join_tab(tab, get_envar(&selected->value[i], get_envar_len(&selected->value[i]), data));
			i+= get_envar_len(&selected->value[i]);
			j = 0;
		}
		// if (!selected->value[i])
		// 	break ;

		
			// if (bol == 0)
			// {
			// 	while (selected->value[i + j]&& selected->value[i + j]  != '$' && selected->value[i + j] != '"' && selected->value[i + j] != '\'')
			// 		j++;
			// 	tab = join_tab(tab, ft_strndup(&selected->value[i], j));
			// 	i += j;
			// }
		// if (j > 1)
		// {
		// 	bol = 1;
		// 	printf("JE CHIE DANS TA BOUCHE\n");
		// 	printf("i = %d, selected->value = %s\n", i, selected->value);
		// 	if (!tab[0])
		// 		tab[0] = ft_strndup(&selected->value[i-j], j);
		// 	else
		// 		tab = join_tab(tab, remove_quotes(ft_strndup(&selected->value[i], j), 1));
		// 	tab[k] =remove_quotes(tab[k], 1);
		// 	printf("LE PIPI\n");
		// 	j = 0;
		// 	// i--;
		// 	if (selected->value[i + 1] == '$')
		// 		bol = 1;
		// 	k++;
		// }
			// else if ((((int)ft_strlen(selected->value) >= i + 1 && \
			// 	selected->value[i] == '$' \
			// 	&& (ft_isalpha(selected->value[i + 1]) || selected->value[i + 1] == '?' \
			// 	|| selected->value[i + 1] == '"' || selected->value[i + 1] == '\'') && \
			// 	 quote_state != 1)))
			// {
				// if (!tab[0])
				// {
				// 	tab[0] = ft_strdup(get_envar(&selected->value[i], get_envar_len(&selected->value[i]), data));
				// 	i += get_envar_len(&selected->value[i])  - 1;;
				// 	// if (selected->value[i] == '$')
				// 		// i--;
				// 	printf("selected->value[%d] = %c\n", i, selected->value[i]);
					
				// 	// i++;
				// }
				// else
				// {
				// 	tab = join_tab(tab, get_envar(&selected->value[i], get_envar_len(&selected->value[i]), data));
				// 	printf("i = %d\n", i);
				// 	i += get_envar_len(&selected->value[i]) - 1;
				// 	printf("i = %d\n", i);
				// 	// i++;
				// }
				// if ( selected->value[i] == '$')
					// i--;
				// if (selected->value[i + 1] == '$')
				// 	bol = 1;
				// j = 0;
				// k++;
			// }
		// i++;
		// bol = 0;
		// j++;
	}
	
	// printf("&selected->value[i-j] %s\n", &selected->value[i]);
	// if (selected->value[i-j+1] && !tab[0][0])
	// 	tab[0] = ft_strndup(&selected->value[i-j], j);
	// else if (selected->value[i-j])
	// 	tab = join_tab(tab, ft_strndup(&selected->value[i - j + 1], j));
	// printf("j = %d\n", j);
	// printf("i = %d\n", i);
	char *tmp = ft_strdup("");
	while(tab[z])
	{
		tmp = ft_strjoin_free(tmp, tab[z]);
		// printf("tab[%d] = %s\n", z, tab[z]);
		free(tab[z]);
		z++;
	}
	// printf("tmp = %s\n", tmp);
	return tmp;
}


char	*expander(t_data *data)
{
	t_token	*selected;
	t_token	*previous;
	int		quote_state;
	int		i;
	// int		k;
	char	**tmp;
	
	previous = NULL;
	selected = data->prompt_top;
	quote_state = 0;
	while (selected->type != END)
	{
		i = 0;
		while(selected->value[i])
		{
			quote_state = quote_management(quote_state, selected->value[i]);
			if (((int)ft_strlen(selected->value) >= i + 1 && \
				selected->value[i] == '$' \
				&& (ft_isalpha(selected->value[i + 1]) || selected->value[i + 1] == '?' \
				|| selected->value[i + 1] == '"' || selected->value[i + 1] == '\'') && \
				 quote_state != 1) && !(previous && previous->type == REDIR && previous->value[1] == '<'))
				 {
					tmp = ft_split_quote_state(replace_envar(data, selected, 0), " \t\n");
					selected->value = tmp[0];
					int j = 1;
					while (tmp[j])
					{
						add_token_next(selected, tmp[j], ft_strlen(tmp[j]), WORD);
						// printf("selected->value = %s\n", selected->value);
						selected = selected->next;
						// printf("tmp[%d] = %s\n", j, tmp[j]);
						j++;
					}
					break ;
				 }
			i++;
		}
		selected = selected->next;
	}
	return (NULL);
}	
//TODO [Xkkkjnfknfkjhfkjhfkjhfkjhfkjhfkjhfkjhfkjhfdkjhfkjhfkjhfkjhfkjhfkjhfkjhfkjhfkjhfjfkjfjjfkfjfkfjjfkfjjfkjfkjfkjfkjfkjfk] - NIQUER SA MERE

// ! caac
// char	*expander(t_data *data)
// {
// 	t_token	*selected;
// 	t_token	*previous;
// 	int		quote_state;
// 	int		i;
// 	int		k;
// 	char	**tmp;
// 	// char	*before;
// 	// char	*after;
	
// 	tmp = NULL;
// 	quote_state = 0;
// 	if (!data->prompt_top)
// 		return (NULL);
// 	selected = data->prompt_top;
// 	previous = NULL;
// 	while (selected->type != END)
// 	{
// 		i = 0;
// 		quote_state = 0;
// 		k = 0;
// 		while (i < (int)ft_strlen(selected->value) && selected->value[i])
// 		{
// 			k++;
// 			//TODO[] - Ajouter un strjoin pour avant et apres l'envar;
// 			quote_state = quote_management(quote_state, selected->value[i]);
// 			if (((int)ft_strlen(selected->value) >= i + 1 && \
// 				selected->value[i] == '$' \
// 				&& (ft_isalpha(selected->value[i + 1]) || selected->value[i + 1] == '?' \
// 				|| selected->value[i + 1] == '"' || selected->value[i + 1] == '\'') && \
// 				 quote_state != 1) && !(previous && previous->type == REDIR && previous->value[1] == '<'))
// 				 {
// 					tmp = ft_split(replace_envar(data, selected, i), ' ');
// 					printf("caca = %s\n", tmp[0]);
// 					// selected->value = ft_strjoin_free(ft_strndup(selected->value, k - 1), tmp[0]);
// 					selected->value = tmp[0];
// 					int j = 1;
// 					while (tmp[j])
// 					{
// 						printf("strdup = %s\n", ft_strndup(selected->value, k));
// 						add_token_next(selected, tmp[j], ft_strlen(tmp[j]), WORD);
// 						printf("selected->value = %s\n", selected->value);
// 						selected = selected->next;
// 						printf("tmp[%d] = %s\n", j, tmp[j]);
// 						j++;
// 						k = 0;
// 					}
// 					// if (ft_strlen(tmp) < 1)
// 						// printf("%zu\n", ft_strlen(tmp));
// 					if (ft_strlen(tmp[0]) < 1)
// 						i--;
// 					free(tmp);
// 					// if (selected->value[i] == '$')
// 						// printf("C'EST NON SALE BATARD DE MERDE %c%c\n", selected->value[i], selected->value[i+1]);
// 					// if (selected->value[i] == '$')
// 						// i--;
// 					// i++;
// 				 }
// 			i++;
// 		}
// 		previous = selected;
// 		selected = selected->next;
// 	}
// 	return (NULL);
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
