/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:16:17 by asuc              #+#    #+#             */
/*   Updated: 2024/03/23 14:20:14 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_return_code;

int	execve_error(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (1);
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
		envp[i] = ft_strjoin_free(envp[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*find_cmd_path(char *cmd, char *path_env)
{
	char	**path_tab;
	int		i;
	char	*path;

	path_tab = ft_split(path_env, ':');
	if (!path_tab)
		return (NULL);
	i = 0;
	while (path_tab[i])
	{
		path = ft_strjoin(path_tab[i], "/");
		path = ft_strjoin_free(path, cmd);
		if (!access(path, F_OK))
		{
			if (!access(path, X_OK))
			{
				free_tab(path_tab);
				return (path);
			}
		}
		free(path);
		i++;
	}
	free_tab(path_tab);
	return (ft_strdup(cmd));
}

char	*get_path(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PATH"))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	check_exec_command(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
		return (execve_error(path));
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (126);
	}
	if (!(buf.st_mode & S_IXUSR))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (0);
}

int	execve_path_env(char *cmd, char **args, t_env *env, t_data *data)
{
	char	**envp;
	int		i;
	pid_t	pid;
	char	*path;

	if (!cmd || !(*args) || !args || !env || !data)
		return (1);
	envp = env_to_tab(env);
	path = get_path(env);
	if (!path || !envp)
		return (1);
	path = find_cmd_path(cmd, path);
	if (!path)
		return (1);
	pid = fork();
	if (pid == -1)
	{
		free_tab(envp);
		return (1);
	}
	if (pid == 0)
	{
		if (check_exec_command(path))
		{
			free_tab(envp);
			free(path);
			ft_exit(data, env, "", 0);
		}
		i = execve(path, args, envp);
		perror("minishell");
		free_tab(envp);
		free(path);
		ft_exit(data, env, "", 0);
	}
	waitpid(pid, &i, 0);
	free_tab(envp);
	free(path);
	return (i);
}

// char **env_to_array(t_env *env)
// {
// 	t_env	*tmp;
// 	int		i;
// 	char	**envp;

// 	tmp = env;
// 	i = 0;
// 	while (tmp)
// 	{
// 		i++;
// 		tmp = tmp->next;
// 	}
// 	envp = malloc(sizeof(char *) * (i + 1));
// 	if (envp == NULL)
// 		return (NULL);
// 	tmp = env;
// 	i = 0;
// 	while (tmp)
// 	{
// 		envp[i] = ft_strjoin(tmp->name, "=");
// 		envp[i] = ft_strjoin_free(envp[i], tmp->value);
// 		tmp = tmp->next;
// 		i++;
// 	}
// 	envp[i] = NULL;
// 	return (envp);
// }

// void execute_command(t_command *command, t_env *env)
// {
// 	// Créer un tableau pour stocker les arguments de la commande
// 	int args_count = ft_tablen(command->args);
// 	char **args = malloc(sizeof(char *) * (args_count + 2));
// 	args[0] = command->cmd;
// 	for (int i = 0; i < args_count; i++)
// 	{
// 		args[i + 1] = command->args[i];
// 	}
// 	args[args_count + 1] = NULL;

// 	// Convertir la liste chaînée d'environnement en tableau de chaînes
// 	char **envp = env_to_array(env);

// 	// Exécuter la commande dans un nouveau processus
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		// Processus fils
// 		if (execve(command->cmd, args, envp) == -1)
// 		{
// 			perror("execve");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else if (pid < 0)
// 	{
// 		// Erreur lors de la création du processus fils
// 		perror("fork");
// 	}
// 	else
// 	{
// 		// Processus parent
// 		wait(NULL);
// 	}

// 	// Libérer la mémoire allouée
// 	free(args);
// 	free_array(envp);
// }

// void execute_commands(t_data *data)
// {
// 	t_command	*current;

// 	current = data->command_top;
// 	while (current != NULL)
// 	{
// 		// Rediriger les entrées/sorties si nécessaire
// 		if (current->input_file != NULL)
// 		{
// 			data->fd_in = open(current->input_file, O_RDONLY);
// 			dup2(data->fd_in, STDIN_FILENO);
// 		}
// 		if (current->output_file != NULL)
// 		{
// 			data->fd_out = open(current->output_file,
//					O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			dup2(data->fd_out, STDOUT_FILENO);
// 		}

// 		execute_command(current, data->env);

// 		// Restaurer les descripteurs de fichiers standard
// 		if (data->fd_in != STDIN_FILENO)
// 		{
// 			close(data->fd_in);
// 			dup2(STDIN_FILENO, STDIN_FILENO);
// 		}
// 		if (data->fd_out != STDOUT_FILENO)
// 		{
// 			close(data->fd_out);
// 			dup2(STDOUT_FILENO, STDOUT_FILENO);
// 		}

// 		current = current->next;
// 	}
// }
