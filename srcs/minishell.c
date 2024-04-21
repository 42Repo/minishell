/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:59:39 by asuc              #+#    #+#             */
/*   Updated: 2024/04/21 06:23:08 by asuc             ###   ########.fr       */
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

void	ft_exit_fork(t_data *data, t_env *env, int exit_code)
{
	int	i;
	
	i = 0;
	if (data->command_top->args[0] && data->command_top->args[1] && data->command_top->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_return_code = 1;
		return ;
	}
	if (data->command_top->args[0] && data->command_top->args[1])
	{
		if ((data->command_top->args[1][0] == '+' || data->command_top->args[1][0] == '-'))
			i++;
	}
	while(data->command_top->args[0] && data->command_top->args[1] && data->command_top->args[1][i])
	{
		i++;
	}
	if(data->command_top->args[1])
		exit_code = ft_atoi(data->command_top->args[1]);
	free_token_lst(data);
	free_env(env);
	free_command(data);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
	if (exit_code >= 0 && exit_code <= 255)
		exit(exit_code);
	if (exit_code < 0)
		exit(256 + exit_code);
	if (exit_code > 255)
		exit(exit_code % 256);
	else
	{
		printf("minishell: exit: %d: numeric argument required\n", exit_code);
		exit(2);
	}
}

int	execute_bultin(t_command *command, t_env *env, t_data *data)
{
	if (data->prompt_top->type == END)
		return (0);
	if (ft_strcmp(command->cmd, "exit") == 0)
		ft_exit_fork(data, env, g_return_code);
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

void	execute_command_pipe(t_command *command, t_data *data, int input_fd, int output_fd)
{
	if (data->prompt_top->type == END)
		return ;
	if (ft_strcmp(command->cmd, "exit") == 0)
		ft_exit(data, data->env, "exit", g_return_code);
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
		if (execute_bultin(command, data->env, data) == 1)
			ft_exit_fork(data, data->env, g_return_code);
		g_return_code = execve_path_env(command->cmd, command->args, data->env, data);
		exit(g_return_code);
	}
}

void	execute_command(t_command *command, t_data *data, int input_fd, int output_fd)
{
	if (data->prompt_top->type == END)
		return ;
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
	if (ft_strcmp(command->cmd, "exit") == 0)
		ft_exit(data, data->env, "exit", g_return_code);
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
		g_return_code = execve_path_env(command->cmd, command->args, data->env, data);
		exit(g_return_code);
	}
	waitpid(command->pid, &g_return_code, 0);
	if (WIFEXITED(g_return_code))
		g_return_code = WEXITSTATUS(g_return_code);
	if (WIFSIGNALED(g_return_code))
		g_return_code = 128 + WTERMSIG(g_return_code);
}

void	choose_case(t_data *data)
{
	t_command	*command;
	int			pipe_fd[2];
	int			prev_fd;
	int			status;

	status = 0;
	command = data->command_top;
	prev_fd = STDIN_FILENO;
	if (command && command->next == NULL)
	{
		execute_command(command, data, prev_fd, STDOUT_FILENO);
		return ;
	}
	while (command)
	{
		if (command->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return ;
		}
		if (command->next)
			execute_command_pipe(command, data, prev_fd, pipe_fd[1]);
		else
			execute_command_pipe(command, data, prev_fd, STDOUT_FILENO);
		if (prev_fd != STDIN_FILENO)
			close(prev_fd);
		if (command->next)
		{
			prev_fd = dup(pipe_fd[0]);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
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

void	ft_exit_ctrl_d(t_data *data, t_env *env, char *exit_msg, int exit_code)
{
	int	i;
	
	i = 0;
	if (data->command_top)
	{
		if (data->command_top->args)
		{
			if (data->command_top->args[0] && data->command_top->args[1] && data->command_top->args[2])
			{
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
				g_return_code = 1;
				return ;
			}
			if (data->command_top->args[0] && data->command_top->args[1])
			{
				if ((data->command_top->args[1][0] == '+' || data->command_top->args[1][0] == '-'))
					i++;
			}
			while(data->command_top->args[1] && data->command_top->args[1][i])
			{
				if(!ft_isdigit(data->command_top->args[1][i]))
				{
					ft_putstr_fd("minishell: exit: ", 2);
					ft_putstr_fd(data->command_top->args[1], 2);
					ft_putstr_fd(": numeric argument required\n", 2);
					g_return_code = 2;
					return ;
				}
				i++;
			}
			if(data->command_top->args[1])
				exit_code = ft_atoi(data->command_top->args[1]);
		}
	}
	free_token_lst(data);
	free_env(env);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data);
	if (ft_strlen(exit_msg))
		printf("%s\n", exit_msg);
	if (exit_code >= 0 && exit_code <= 255)
		exit(exit_code);
	if (exit_code < 0)
		exit(256 + exit_code);
	if (exit_code > 255)
		exit(exit_code % 256);
	else
	{
		printf("minishell: exit: %d: numeric argument required\n", exit_code);
		exit(2);
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
		// if (isatty(fileno(stdin)))
		line = readline(data->cmd_prompt);
		// else
		// {
		// 	line = get_next_line(fileno(stdin));
		// 	line = ft_strtrim(line, "\n");
		// 	free(line);
		// }
		if (line == NULL)
			ft_exit_ctrl_d(data, data->env, "exit", g_return_code);
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

