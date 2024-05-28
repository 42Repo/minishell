/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:27:31 by asuc              #+#    #+#             */
/*   Updated: 2024/05/28 15:29:25 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_all(char **path, char ***envp, t_data *data)
{
	close(0);
	close(1);
	close(2);
	free_tab((*envp));
	free((*path));
	ft_exit(data->command_top, data, (*path), 1);
}
