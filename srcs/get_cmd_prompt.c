/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:18:39 by asuc              #+#    #+#             */
/*   Updated: 2024/03/11 17:18:39 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_free(char *str)
{
	perror(str);
	return (EXIT_FAILURE);
}

static char	*get_cwd(t_env *env)
{
	char	*tmp;
	char	*tmp2;

	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
		return (NULL);
	if (get_env_value_string(env, "HOME") != NULL && ft_strncmp(tmp,
			get_env_value_string(env, "HOME"),
			ft_strlen(get_env_value_string(env, "HOME"))) == 0)
	{
		tmp2 = tmp;
		tmp = ft_strjoin_free(ft_strdup("~"), tmp2
				+ ft_strlen(get_env_value_string(env, "HOME")));
		free(tmp2);
		if (tmp == NULL)
			return (NULL);
	}
	return (tmp);
}

static int	set_prompt_top(t_data *data, t_env *env)
{
	char	*tmp;

	tmp = get_cwd(env);
	if (tmp == NULL)
		return (EXIT_FAILURE);
	if (data->cmd_prompt)
	{
		free(data->cmd_prompt);
		data->cmd_prompt = NULL;
	}
	data->cmd_prompt = ft_strdup("\001\033[1;32m\002");
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strdup failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, tmp);
	free(tmp);
	tmp = NULL;
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	data->cmd_prompt = ft_strjoin_free(data->cmd_prompt, "\001\033[0m\002$ ");
	if (data->cmd_prompt == NULL)
		return (error_free("ft_strjoin_free failed"));
	return (EXIT_SUCCESS);
}

int	get_cmd_prompt(t_data *data, t_env *env)
{
	return (set_prompt_top(data, env));
}
