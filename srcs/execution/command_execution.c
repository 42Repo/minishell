/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:42:37 by asuc              #+#    #+#             */
/*   Updated: 2024/05/21 00:01:26 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_error_and_free_resources(char ***envp, char **path,
		t_data *data, t_env *env)
{
	int	ret;

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
	ret = data->g_return_code;
	free(data);
	close(0);
	close(1);
	close(2);
	return (ret);
}

static void	free_all(char **path, char ***envp, t_data *data)
{
	close(0);
	close(1);
	close(2);
	free_tab((*envp));
	free((*path));
	ft_exit(data->command_top, data, (*path), 1);
}

static void	execute_command_here_doc(char *path, char **args, char ***envp,
		t_data *data)
{
	struct stat	buf;

	if (data->fd_in > 2)
		close(data->fd_in);
	if (data->fd_out > 2)
		close(data->fd_out);
	execve(path, args, (*envp));
	if (errno == ENOENT)
	{
		if (stat(path, &buf) == 0)
		{
			if (S_ISDIR(buf.st_mode))
			{
				data->g_return_code = 126;
				printf("minishell: %s: is a directory\n", path);
				free_all(&path, envp, data);
			}
			data->g_return_code = 127;
			printf("minishell: %s: no such file or directory\n", path);
			free_all(&path, envp, data);
		}
	}
	else if (errno == EACCES)
	{
		if (stat(path, &buf) == 0)
		{
			if (S_ISDIR(buf.st_mode))
			{
				data->g_return_code = 126;
				printf("minishell: %s: is a directory\n", path);
				free_all(&path, envp, data);
			}
		}
		data->g_return_code = 126;
		printf("minishell: %s: permission denied\n", path);
		free_all(&path, envp, data);
	}
	data->g_return_code = 1;
	printf("minishell: %s: %s\n", path, strerror(errno));
	free_all(&path, envp, data);
}

static char	*prepare_path(char *cmd, t_env *env, t_data *data)
{
	char	*path;

	path = get_path(env);
	if (has_slash(cmd) != 1)
	{
		if (path == NULL || ft_strlen(path) == 0)
			return (ft_strdup(cmd));
		path = find_cmd_path(cmd, path);
		data->g_return_code = 0;
		if (path == NULL)
			data->g_return_code = handle_path_error(cmd);
	}
	return (ft_strdup(cmd));
}

int	execve_path_env(char *cmd, char **args, t_env *env, t_data *data)
{
	char	**envp;
	char	*path;

	if (!cmd || !(*args) || !args || !env || !data)
		return (1);
	data->g_return_code = 0;
	envp = env_to_tab(env);
	if (!envp)
		return (1);
	path = prepare_path(cmd, env, data);
	if (data->g_return_code != 0)
		return (handle_error_and_free_resources(&envp, &path, data, env));
	execute_command_here_doc(path, args, &envp, data);
	return (127);
}
