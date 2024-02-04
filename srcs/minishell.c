/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/04 08:27:46 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	// int i;
	// char *line;
	t_data data;
	(void)envp;
	(void)data;
	// put_header();
	// while (1)
	// {
	// 	data.actual_path = getcwd(NULL, 0);
	// 	ft_putstr_fd(data.actual_path, 1);
	// 	line = readline(" $> ");
	// 	ft_cd(&data, line);
	// 	if (ft_strncmp(line, "env\n", 4) == 0)
	// 	{
	// 		i = 0;
	// 		while (envp[i])
	// 		{
	// 			printf("%s\n", envp[i]);
	// 			i++;
	// 		}
	// 	}
	// 	if (line != NULL)
	// 		free(line);

	// }
	lexer(NULL, &data);
	return (0);
}
