/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/02/13 00:56:22 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_data(t_data *data)
{
	data->prompt_top = NULL;
	data->cmd_prompt = NULL;
	data->quote_state = 0;
	data->command_top = NULL;
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

char	**env_to_tab(t_env *env)
{
	t_env	*tmp;
	int		i;
	char	**envp;

	tmp = env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (i + 1));
	if (envp == NULL)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		envp[i] = ft_strjoin(tmp->name, "=");
		envp[i] = ft_strjoin(envp[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	*data;
	t_env	*env;
	char	*line_tmp;

	printf("\e[1;1H\e[2J");
	(void)argc;
	(void)argv;
	env = malloc(sizeof(t_env));

	data = malloc(sizeof(t_data));
	put_header();
	init_data(data);
	get_env(env, envp);
	while (1)
	{
		get_cmd_prompt(data, env);
		if (data->cmd_prompt == NULL)
			return (-1);
		line = readline(data->cmd_prompt);
		if (line == NULL)
			return (-1);
		// printf("test\n");
		free(data->cmd_prompt);
		add_history(line);
		lexer(line, data);
		parser(data);
		if (ft_strncmp(line, "exit", max_len(line, 4)) == 0)
		{
			free_token_lst(data);
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
		else if (ft_strncmp(line, "unset ", 6) == 0)
		{
			line_tmp = line + 6;
			printf("line_tmp = %s\n", line_tmp);
			ft_unset(env, line_tmp);
		}
		else if (ft_strncmp(line, "echo ", 5) == 0)
		{
			line_tmp = line + 5;
			printf("line_tmp = %s\n", line_tmp);
			ft_echo(line_tmp, 0);
		}
		else if (ft_strncmp(line, "pwd", 3) == 0)
		{
			ft_pwd(env);
		}
		else
		{
			char **env_test;
			pid_t pid;
			data->command_top->cmd = ft_strjoin("/usr/bin/", data->command_top->cmd);
			env_test = env_to_tab(env);
			pid = fork();
			if (pid == -1)
			{
				printf("fork failed\n");
				exit(1);
			}
			if (pid == 0)
			{
				execve(data->command_top->cmd, data->command_top->args, env_test);
				exit(0);
			}
			else
			{
				waitpid(pid, NULL, 0);
			}
			free(env_test);
		}
		if (line != NULL)
			free(line);

	}
	return (0);
}
