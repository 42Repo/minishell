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

int	ft_cd(t_data *data, char *path)
{
	int i;
	
	i = 0;
	while (path[i])
	{
		if (path[i] == '\n')
			path[i] = '\0';
		i++;
	}
	while (ft_strncmp(path, "..", 2) == 0)
	{
		if (ft_strncmp(data->actual_path, "/", 1) == 0)
			return (0);
		i = ft_strlen(data->actual_path) - 1;
		while (data->actual_path[i] != '/')
			i--;
		data->actual_path[i] = '\0';
		path += 2;
	}
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