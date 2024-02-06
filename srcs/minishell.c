/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/06 22:52:22 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_data(t_data *data)
{
	data->prompt_top = NULL;
}

static int	ft_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

static int	free_env(t_env *env)
{
	t_env	*tmp;

	while (env->next)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
	free(env->name);
	free(env->value);
	free(env);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	data;
	t_env	*env;
	char	*line_tmp;

	(void)argc;
	(void)argv;
	env = malloc(sizeof(t_env));
	put_header();
	init_data(&data);
	get_env(env, envp);
	while (1)
	{
		get_cmd_prompt(&data, env);
		line = readline(data.cmd_prompt);
		free(data.cmd_prompt);
		add_history(line);
		if (ft_strncmp(line, "exit", max_len(line, 4)) == 0)
		{
			free(line);
			free_env(env);
			rl_clear_history();
			return (0);
		}
		else if (ft_strncmp(line, "cd ", 3) == 0)
		{
			line_tmp = line + 3;
			printf("line_tmp = %s\n", line_tmp);
			ft_cd(line_tmp, env);
		}
		else if (ft_strncmp(line, "export ", 7) == 0)
		{
			line_tmp = line + 7;
			printf("line_tmp = %s\n", line_tmp);
			ft_export(env, line_tmp);
		}
		else if (ft_strncmp(line, "env", 3) == 0)
		{
			ft_env(env);
		}
		if (line != NULL)
			free(line);
	}
	// lexer(NULL, &data);
	return (0);
}
