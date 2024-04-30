/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 11:54:59 by asuc              #+#    #+#             */
/*   Updated: 2024/02/04 11:59:08 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	max_len(char *str, size_t nb)
{
	if (ft_strlen(str) > nb)
		return (ft_strlen(str));
	return (nb);
}

char	*ft_strtrim_free(char *s1, char *set)
{
	char *line;

	line = ft_strtrim(s1, set);
	if (s1)
		free(s1);
	return (line);
}