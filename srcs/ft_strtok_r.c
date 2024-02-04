/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 09:01:57 by asuc              #+#    #+#             */
/*   Updated: 2024/02/04 09:32:44 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

static size_t	ft_strcspn(const char *str, const char *reject)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (reject[j] != '\0')
		{
			if (str[i] == reject[j])
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

static size_t	ft_strspn(const char *str, const char *accept)
{
	size_t	i;
	size_t	j;
	int		found;

	i = 0;
	while (str[i] != '\0')
	{
		found = 0;
		j = 0;
		while (accept[j] != '\0')
		{
			if (str[i] == accept[j])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (!found)
			break ;
		i++;
	}
	return (i);
}

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*end;

	if (str == NULL)
		str = *saveptr;
	if (*str == '\0')
	{
		*saveptr = str;
		return (NULL);
	}
	str += ft_strspn(str, delim);
	if (*str == '\0')
	{
		*saveptr = str;
		return (NULL);
	}
	end = str + ft_strcspn(str, delim);
	if (*end == '\0')
	{
		*saveptr = end;
		return (str);
	}
	*end = '\0';
	*saveptr = end + 1;
	return (str);
}
