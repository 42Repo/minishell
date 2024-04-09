/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:59:39 by asuc              #+#    #+#             */
/*   Updated: 2024/04/09 14:45:14 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_return_code = 0;

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

int	execute_bultin(t_command *command, t_env *env, t_data *data)
{
	if (data->prompt_top->type == END)
		return (0);
	if (ft_strcmp(command->cmd, "exit") == 0)
		ft_exit(data, env, "exit", g_return_code);
	else if (ft_strcmp(command->cmd, "cd") == 0)
		ft_cd(data, env);
	else if (ft_strcmp(command->cmd, "export") == 0)
		ft_export(env, command->args);
	else if (ft_strcmp(command->cmd, "env") == 0)
		ft_env(env);
	else if (ft_strcmp(command->cmd, "unset") == 0)
		ft_unset(env, data);
	else if (ft_strcmp(command->cmd, "echo") == 0)
		ft_echo(data);
	else if (ft_strcmp(command->cmd, "pwd") == 0)
		ft_pwd(env);
	else
		return (0);
	return (1);
}

void	execute_command(t_command *command, t_data *data, int input_fd, int output_fd)
{
	if (execute_bultin(command, data->env, data) == 1)
		return ;
	command->pid = fork();
	if (command->pid == -1)
	{
		perror("fork");
		return ;
	}
	if (command->pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		g_return_code = execve_path_env(command->cmd, command->args, data->env, data);
		exit(g_return_code);
	}
	else
	{
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		if (output_fd != STDOUT_FILENO)
			close(output_fd);
	}
}

void	choose_case(t_data *data)
{
	t_command	*command;
	int			pipe_fd[2];
	int			prev_fd;
	int			status;

	command = data->command_top;
	prev_fd = STDIN_FILENO;
	while (command)
	{
		if (command->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return ;
		}
		if (command->next)
			execute_command(command, data, prev_fd, pipe_fd[1]);
		else
			execute_command(command, data, prev_fd, data->fd_out);
		if (prev_fd != STDIN_FILENO)
			close(prev_fd);
		if (command->next)
			prev_fd = pipe_fd[0];
		command = command->next;
	}
	command = data->command_top;
	while (command)
	{
		waitpid(command->pid, &status, 0);
		if (WIFEXITED(status))
			g_return_code = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			g_return_code = 128 + WTERMSIG(status);
		command = command->next;
	}
}


int	wait_cmd_prompt(t_data *data)
{
	char	*line;

	g_return_code = 0;
	while (1)
	{
		if (data->prompt_top)
			free_token_lst(data);
		if (data->command_top)
			free_command(data);
		get_cmd_prompt(data, data->env);
		if (data->cmd_prompt == NULL)
			return (-1);
		line = readline(data->cmd_prompt);
		if (line == NULL)
			ft_exit(data, data->env, "exit", g_return_code);
		if (ft_strlen(line) > 0)
			add_history(line);
		lexer(line, data);
		parser(data);
		choose_case(data);
		if (line != NULL)
			free(line);
	}
}

int	main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)),
		char **envp)
{
	t_data	*data;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	data = ft_calloc(sizeof(t_data), 1);
	data->env = ft_calloc(sizeof(t_env), 1);
	// printf("\033c");
	init_data(data);
	get_env(data->env, envp);
	wait_cmd_prompt(data);
	return (0);
}

