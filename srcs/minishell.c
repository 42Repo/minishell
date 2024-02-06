/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 00:54:40 by asuc             ###   ########.fr       */
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
	t_env	*env;

	(void)argc;
	(void)argv;
	put_header();
	init_data(&data);
	get_env(&env, envp);
	printf("env = \n");
	ft_export(env, "PATH=caca");
	while (env->next)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	while (1)
	{
		get_cmd_prompt(&data);
		line = readline(data.cmd_prompt);
		add_history(line);
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
