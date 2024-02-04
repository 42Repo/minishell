/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/04 11:57:49 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_data(t_data *data)
{
	data->prompt_top = NULL;
	data->old_cd = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	int		i;
	char	*line;
	t_data	data;
	char	*input;

	(void)argc;
	(void)argv;
	put_header();
	init_data(&data);
	while (1)
	{
		input = ft_strjoin_free(getcwd(NULL, 0), " $> ");
		line = readline(input);
		free(input);
		add_history(line);
		ft_cd(&data, line);
		if (ft_strncmp(line, "env", max_len(line, 3)) == 0)
		{
			i = 0;
			while (envp[i])
			{
				printf("%s\n", envp[i]);
				i++;
			}
			free(line);
			free(data.old_cd);
			return (0);
		}
		if (line != NULL)
			free(line);
	}
	// lexer(NULL, &data);
	return (0);
}
