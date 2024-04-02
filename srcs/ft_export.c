# /* ************************************************************************** */
# /*                                                                            */
# /*                                                        :::      ::::::::   */
# /*   ft_export.c                                        :+:      :+:    :+:   */
# /*                                                    +:+ +:+         +:+     */
# /*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
# /*                                                +#+#+#+#+#+   +#+           */
# /*   Created: 2024/02/05 19:31:03 by asuc              #+#    #+#             */
# /*   Updated: 2024/02/07 00:21:43 by asuc             ###   ########.fr       */
# /*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_export(t_env *env, char **args)
{
	t_env	*tmp;
	int		i;

	i = 1;
	while (args[i])
	{
		tmp = get_env_value_ptr(env, args[i]);
		if (tmp)
		{
			free(tmp->value);
			tmp->value = ft_strdup(ft_strchr(args[i], '=') + 1);
		}
		else
		{
			tmp = env;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = ft_calloc(sizeof(t_env), 1);
			if (!tmp->next)
				return ;
			tmp->next->name = ft_strndup(args[i], ft_strchr(args[i], '=') - args[i]);
			tmp->next->value = ft_strdup(ft_strchr(args[i], '=') + 1);
			tmp->next->next = NULL;
		}
		args++;
	}
}
