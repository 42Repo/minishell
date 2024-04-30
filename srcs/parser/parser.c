/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mbuchs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/04/30 13:00:13 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	free_command(t_data *data)
{
	t_command	*selected;
	t_command	*old;

	selected = data->command_top;
	while (selected)
	{
		if (selected->cmd)
		{
			free(selected->cmd);
			selected->cmd = NULL;
		}
		if (selected->args)
		{
			free_tab(selected->args);
			selected->args = NULL;
		}
		old = selected;
		selected = selected->next;
		if (old)
		{
			free(old);
			old = NULL;
		}
		data->command_top = selected;
	}
}

t_command	*init_command(void)
{
	t_command	*command;

	command = ft_calloc(sizeof(t_command), 1);
	command->cmd = NULL;
	command->args = NULL;
	command->next = NULL;
	command->fd_in = 0;
	command->fd_out = 1;
	command->random_name[0] = '\0';
	command->pid = 0;
	return (command);
}

void select_output(char *file, t_data *data, int mode, t_command *command)
{
	(void) data;
	if (command->fd_out != 1)
		close(command->fd_out);
	if (command->fd_out != 1)
		command->fd_out = 1;
	if (mode == 1)
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		command->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (command->fd_out == -1)
		command->fd_out = 1;
	// gerer erreurs
}

void	select_input(char *file, t_data *data, t_command *command)
{
	(void) data;
	if (command->fd_in != 0)
		close(command->fd_in);
	if (command->fd_in != 0)
		command->fd_in = 0;
	if (access(file, F_OK) == -1)
	{
		put_error("minishell: ", file, ": No such file or directory\n");
		command->cmd = NULL;
		return ;
	}
	if (access(file, R_OK) == -1)
	{
		put_error("minishell: ", file, ": Permission denied\n");
		command->cmd = NULL;
		return ;
	}
	command->fd_in = open(file, O_RDONLY);
	if (command->fd_in == -1)
		command->fd_in = 0;
	// gerer erreurs
}

void	get_redir(t_token *selected, t_data *data, t_command *command)
{
	if (selected->type == REDIR)
	{
		if (selected->next && selected->next->type == WORD)
		{
			if (ft_strlen(selected->value) == 2 && selected->value[1] == '>')
				select_output(selected->next->value, data, 2, command);
			else if (selected->value[0] == '>')
				select_output(selected->next->value, data, 1, command);
			else if (ft_strlen(selected->value) == 2 && selected->value[1] == '<')
				heredoc(selected->next->value, data, command);
			else if (selected->value[0] == '<')
				select_input(selected->next->value, data, command);
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `HEHEHE'\n", 2);
				command->cmd = NULL;
			}
		}
		else
		{
			ft_putstr_fd("minishell: syntax error near unexpected 'HAHAHA'\n", 2);
			command->cmd = NULL;
		}
	}
}

void remove_empty_tokens(t_data *data)
{
	t_token	*selected;
	t_token	*old;

	old = NULL;
	selected = data->prompt_top;
	while (selected)
	{
		if (selected->type == WORD && ft_strlen(selected->value) == 0)
		{
			if (old)
				old->next = selected->next;
			else
				data->prompt_top = selected->next;
			if(selected->value)
				free(selected->value);
			if(selected)
				free(selected);
			if (old)
				selected = old;
		}
		old = selected;
		selected = selected->next;
	}

}

char *parse_line(t_data *data, t_token *selected, t_command *command)
{
	char	**tmp;
	char	*tmp2;
	t_token	*token;

	remove_empty_tokens(data);
	token = data->prompt_top;
	while (token->type != END)
	{
		if (token->type == WORD)
			token->value = remove_quotes(token->value, data);
		// printf("token->value = %s\n", token->value);
		token = token->next;
	}
	selected = data->prompt_top;
	if (selected->type == PIPE)
		return (selected->value);
	while (selected)
	{
		command->args = ft_calloc(sizeof(char **), 1);
		command->next = NULL;
		while (selected && selected->type != PIPE)
		{
			if (selected->type == WORD && !command->cmd)
			{
				selected->value = remove_quotes(selected->value, data);
				tmp2 = ft_strdup(selected->value);
				tmp = ft_split(tmp2, ' ');
				command->cmd = ft_strdup(tmp[0]);
				if (ft_tablen(tmp) > 2)
					command->args = tmp;
				if (ft_tablen(tmp) > 2)
					selected = selected->next;
			}
			if (selected->type == WORD)
				command->args = join_tab(command->args, ft_strdup(selected->value));
			else if (selected->type == REDIR)
			{
				if (selected->next && selected->next->type == END)
					return (selected->next->value);
				if (!selected->next || (selected->next && selected->next->type != WORD))
					return (selected->value);
				get_redir(selected, data, command);
				selected = selected->next;
			}
			selected = selected->next;
		}
		if (selected && selected->type == PIPE)
		{
			if (selected->next && selected->next->type == REDIR && selected->next->value[0] == '>')
				return (selected->next->value);
			if (selected->next->type != WORD)
				return (selected->value);
			command->next = init_command();
			command = command->next;
			selected = selected->next;
		}
		if (selected && selected->type == END)
			return (NULL);
	}
	return (NULL);
}

int	parser(t_data *data)
{
	t_token		*selected;
	t_command	*command;
	char		*error;
	
	data->command_top = init_command();
	selected = data->prompt_top;
	command = data->command_top;
	error = parse_line(data, selected, command);
	if(error)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(error, 2);
		ft_putstr_fd("'\n", 2);
		g_return_code = 2;
		return -1;
	}
	command = data->command_top;
	while (command->next)
	{
		command->cmd = remove_quotes(command->cmd, data);
		int i = 0;
		while (command->args[i])
		{
			command->args[i] = remove_quotes(command->args[i], data);
			i++;
		}
		command = command->next;
	}
	return (0);
}
