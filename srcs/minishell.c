/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:59:39 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 22:19:47 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_return_code = 0;

void	init_data(t_data *data)
{
	rl_catch_signals = 0;
	data->prompt_top = NULL;
	data->cmd_prompt = NULL;
	data->quote_state = 0;
	data->command_top = NULL;
	data->fd_out = dup(STDOUT_FILENO);
	data->fd_in = dup(STDIN_FILENO);
	data->term = ft_calloc(sizeof(struct termios), 1);
	tcgetattr(STDIN_FILENO, data->term);
}

int	execute_bultin(t_command *command, t_env *env, t_data *data, int fd_out)
{
	(void)fd_out;
	if (data->prompt_top->type == END)
		return (0);
	else if (ft_strcmp(command->cmd, "cd") == 0)
		ft_cd(command, env);
	else if (ft_strcmp(command->cmd, "export") == 0)
		ft_export(env, command->args);
	else if (ft_strcmp(command->cmd, "env") == 0)
		ft_env(command, env);
	else if (ft_strcmp(command->cmd, "unset") == 0)
		ft_unset(env, command);
	else if (ft_strcmp(command->cmd, "echo") == 0)
		ft_echo(command);
	else if (ft_strcmp(command->cmd, "pwd") == 0)
		ft_pwd(env);
	else
		return (0);
	return (1);
}

void	execute_command_pipe(t_command *command, t_data *data, int input_fd,
			int output_fd)
{
	if (data->prompt_top->type == END
		|| command == NULL || command->cmd == NULL)
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
		if (command->pipe[1] == output_fd)
			close(command->pipe[0]);
		if (ft_strcmp(command->cmd, "exit") == 0)
			ft_exit(command, data, "", 0);
		if (execute_bultin(command, data->env, data, output_fd) == 1)
			ft_exit(command, data, "", 1);
		g_return_code = execve_path_env(command->cmd,
				command->args, data->env, data);
		exit(g_return_code);
	}
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_command(t_command *command, t_data *data, int input_fd,
			int output_fd)
{
	int	fd_out;
	int	fd_in;

	if (command == NULL || command->cmd == NULL)
		return ;
	if (is_builtin(command->cmd))
	{
		if (input_fd != STDIN_FILENO)
			dup2(input_fd, STDIN_FILENO);
		if (output_fd != STDOUT_FILENO)
			dup2(output_fd, STDOUT_FILENO);
		if (ft_strcmp(command->cmd, "exit") == 0)
			return (ft_exit(command, data, "exit", 0));
		if (execute_bultin(command, data->env, data, output_fd) == 1)
		{
			fd_in = dup(data->fd_in);
			fd_out = dup(data->fd_out);
			dup2(fd_in, STDIN_FILENO);
			dup2(fd_out, STDOUT_FILENO);
			if (fd_in > 2)
				close(fd_in);
			if (fd_out > 2)
				close(fd_out);
		}
		return ;
	}
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
		g_return_code = execve_path_env(command->cmd, command->args,
				data->env, data);
		exit(g_return_code);
	}
	waitpid(command->pid, &g_return_code, 0);
	if (WIFSIGNALED(g_return_code))
		g_return_code = 128 + WTERMSIG(g_return_code);
	if (WIFEXITED(g_return_code))
		g_return_code = WEXITSTATUS(g_return_code);
	if (g_return_code == 130)
		printf("\n");
}

void useless(int sig)
{
	(void)sig;
}

void	choose_case(t_data *data)
{
	t_command	*command;
	int			prev_fd;
	int			status;

	signal(SIGINT, useless);
	signal(SIGQUIT, useless);
	status = 0;
	command = data->command_top;
	prev_fd = command->fd_in;
	if (command && command->next == NULL)
	{
		execute_command(command, data, prev_fd, command->fd_out);
		return ;
	}
	while (command)
	{
		if (command->fd_in != STDIN_FILENO)
		{
			prev_fd = command->fd_in;
			command->fd_in = STDIN_FILENO;
		}
		if (command->next && pipe(command->pipe) == -1)
		{
			perror("pipe");
			return ;
		}
		if (command->fd_out != -1 && command->fd_in != -1 && command->next)
		{
			if (command->fd_out != STDOUT_FILENO)
				execute_command_pipe(command, data, prev_fd, command->fd_out);
			else
				execute_command_pipe(command, data, prev_fd, command->pipe[1]);
		}
		else if (command->fd_out != -1 && command->fd_in != -1)
			execute_command_pipe(command, data, prev_fd, command->fd_out);
		if (prev_fd != STDIN_FILENO)
		{
			if (prev_fd > 2)
				close(prev_fd);
			prev_fd = STDIN_FILENO;
		}
		if (command->next)
		{
			prev_fd = dup(command->pipe[0]);
			close(command->pipe[0]);
			close(command->pipe[1]);
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
		if (g_return_code == 130)
			printf("\n");
		command = command->next;
	}
	command = data->command_top;
	while (command)
	{
		if (command->next == NULL && (command->fd_out == -1 || command->fd_in == -1))
			g_return_code = 1;
		command = command->next;
	}
}

void	wait_cmd_prompt(t_data *data)
{
	char	*line;

	g_return_code = 0;
	while (1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		if (data->prompt_top)
			free_token_lst(data);
		if (data->command_top)
			free_command(data);
		get_cmd_prompt(data, data->env);
		if (data->cmd_prompt == NULL)
			return ;
		/////////////////////////// in testing
		if (isatty(fileno(stdin)))
			line = ft_strtrim_free(readline(data->cmd_prompt), " ");
		else
		{
			char *line1;
			line1 = get_next_line(fileno(stdin));
			line = ft_strtrim(line1, "\n");
			free(line1);
		}
		////////////////////// in testing
		// line = ft_strtrim_free(readline(data->cmd_prompt), " ");
		if (line == NULL)
			ft_exit(data->command_top, data, "exit", 1);
		if (ft_strlen(line) > 0)
			add_history(line);
		if (lexer(line, data) == 0)
		{
			free(line);
			line = NULL;
			if (parser(data) == 0)
				choose_case(data);
			else
			{
				free_token_lst(data);
				free_command(data);
			}
		}
		else
			free_token_lst(data);
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
	init_data(data);
	get_env(data->env, envp);
	wait_cmd_prompt(data);
	return (0);
}
