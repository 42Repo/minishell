/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:42:37 by asuc              #+#    #+#             */
/*   Updated: 2024/05/15 17:46:32 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_cmd(char *cmd)
{
	struct stat	buf;

	stat(cmd, &buf);
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
		{
			if (S_ISDIR(buf.st_mode))
				return (126);
			return (0);
		}
		else
			return (126);
	}
	return (1);
}

static int	handle_error_and_free_resources(char ***envp, char **path,
		t_data *data, t_env *env)
{
	free_tab((*envp));
	free((*path));
	free_token_lst(data);
	free_env(env);
	free_command(data);
	rl_clear_history();
	if (data->cmd_prompt)
		free(data->cmd_prompt);
	free(data->term);
	if (data->fd_in > 2)
		close(data->fd_in);
	if (data->fd_out > 2)
		close(data->fd_out);
	free(data);
	close(0);
	close(1);
	close(2);
	return (g_return_code);
}

static void	execute_command_here_doc(char *path, char **args, char ***envp,
		t_data *data)
{
	if (data->fd_in > 2)
		close(data->fd_in);
	if (data->fd_out > 2)
		close(data->fd_out);
	execve(path, args, (*envp));
	close(0);
	close(1);
	close(2);
	rl_clear_history();
	free(data->term);
	free_tab((*envp));
	free(path);
}

static char	*prepare_path(char *cmd, t_env *env)
{
	char	*path;

	path = get_path(env);
	if (has_slash(cmd) != 1)
	{
		g_return_code = check_cmd(cmd);
		if (g_return_code == 0)
			path = ft_strdup(cmd);
		else
			path = find_cmd_path(cmd, path);
		g_return_code = 0;
		if (path == NULL)
			g_return_code = handle_path_error(cmd);
	}
	else
	{
		path = ft_strdup(cmd);
		g_return_code = check_exec_command_path(path);
	}
	return (path);
}

int	execve_path_env(char *cmd, char **args, t_env *env, t_data *data)
{
	char	**envp;
	char	*path;

	if (!cmd || !(*args) || !args || !env || !data)
		return (1);
	g_return_code = 0;
	envp = env_to_tab(env);
	if (!envp)
		return (1);
	path = prepare_path(cmd, env);
	if (g_return_code != 0)
		return (handle_error_and_free_resources(&envp, &path, data, env));
	execute_command_here_doc(path, args, &envp, data);
	return (127);
}
