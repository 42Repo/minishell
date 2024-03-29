/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:39:33 by asuc            #+#    #+#             */
/*   Updated: 2023/09/26 19:07:21 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if (!c)
		return (0);
	if (!(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z'))
		return (0);
	return (1);
}
