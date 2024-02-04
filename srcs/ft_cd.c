/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:46:32 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 15:46:39 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// on change la variables path si il y a des points dans cd_arg par exemple : ../../../ si le path est /home/asuc/Documents/42/minishell ca deviendra /home/asuc
// de plus on doit aussi gerer le cas ou il y a  ../../../srcs/caca par exemple donc on doit arreter le ft_check_point a chaque / et si il y a un . on doit remonter d'un dossier
int	check_point(char *path, char *arg_cd)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (arg_cd[i])
	{

	}
}

int	ft_cd(t_data *data, char *path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		if (path[i] == '\n')
			path[i] = '\0';
		i++;
	}
	check_point(path);
	data->actual_path = ft_strjoin_free(data->actual_path, "/");
	data->actual_path = ft_strjoin_free(data->actual_path, path);
	printf("path to change to %s\n", data->actual_path);
	if (chdir(data->actual_path) == 0)
	{
		printf("path changed to %s\n", getcwd(NULL, 0));
	}
	else
	{
		printf("path not changed\n");
		return (1);
	}
	return (0);
}
