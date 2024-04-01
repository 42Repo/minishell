/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:08:03 by asuc              #+#    #+#             */
/*   Updated: 2024/04/01 17:01:05 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_return_code = 0;

void	init_data(t_data *data)
{
	rl_catch_signals = 0;
	data->prompt_top = NULL;
	data->cmd_prompt = NULL;
	data->quote_state = 0;
	data->command_top = NULL;
	data->fd_out = 1;
	data->fd_in = 0;

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

void	execute_command(t_command *command, t_env *env, t_data *data,
		int input_fd, int output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		if (input_fd && input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd && output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		if (ft_strcmp(command->cmd, "exit") == 0)
			ft_exit(data, env, "exit", 0);
		else if (ft_strcmp(command->cmd, "cd") == 0)
			ft_cd(data, env);
		else if (ft_strcmp(command->cmd, "export") == 0)
			ft_export(env, command->args[1]);
		else if (ft_strcmp(command->cmd, "env") == 0)
			ft_env(env);
		else if (ft_strcmp(command->cmd, "unset") == 0)
			ft_unset(env, data);
		else if (ft_strcmp(command->cmd, "echo") == 0)
			ft_echo(data);
		else if (ft_strcmp(command->cmd, "pwd") == 0)
			ft_pwd(env);
		else
			g_return_code = execve_path_env(command->cmd, command->args, env,
					data);
		exit(g_return_code);
	}
	else
	{
		waitpid(pid, &g_return_code, 0);
	}
}

void	choose_case(t_env *env, t_data *data)
{
	t_command	*command;
	int			pipe_fd[2];
	int			prev_fd;

	command = data->command_top;
	prev_fd = STDIN_FILENO;
	while (command)
	{
		if (ft_strcmp(command->cmd, "exit") == 0)
			ft_exit(data, env, "exit", 0);
		if (command->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		execute_command(command, env, data, prev_fd, pipe_fd[1]);
		if (prev_fd != STDIN_FILENO)
		{
			printf("close prev_fd\n");
			close(prev_fd);
		}
		if (command->next)
		{
			close(pipe_fd[1]);
			prev_fd = dup(pipe_fd[0]);
			close(pipe_fd[0]);
		}
		command = command->next;
	}
}

int	wait_cmd_prompt(t_data *data, t_env *env)
{
	char	*line;

	while (1)
	{
		if (data->prompt_top)
			free_token_lst(data);
		if (data->command_top)
			free_command(data);
		get_cmd_prompt(data, env);
		if (data->cmd_prompt == NULL)
			return (-1);
		line = readline(data->cmd_prompt);
		if (line == NULL)
			ft_exit(data, env, "exit", 0);
		add_history(line);
		lexer(line, data);
		parser(data);
		choose_case(env, data);
		if (line != NULL)
			free(line);
		if (data->fd_out != 1)
		{
			close(data->fd_out);
			data->fd_out = 1;
		}
	}
}

int	main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)),
		char **envp)
{
	t_data	*data;
	t_env	*env;

	signal(SIGINT, (void (*)(int))sig_handler);
	signal(SIGQUIT, (void (*)(int))sig_handler);
	data = ft_calloc(sizeof(t_data), 1);
	env = ft_calloc(sizeof(t_env), 1);
	data->env = env;
	// printf("\033c");
	init_data(data);
	get_env(env, envp);
	wait_cmd_prompt(data, env);
	return (0);
}
