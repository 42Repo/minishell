/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_name_generator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuc <asuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:18:29 by asuc              #+#    #+#             */
/*   Updated: 2024/05/13 21:35:17 by asuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	random_init(t_command *command)
{
	int			urandom_fd;
	int			bytes_read;
	char		buffer[2];
	int			i;

	i = 1;
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		return ;
	while (i < 20)
	{
		bytes_read = read(urandom_fd, buffer, 1);
		if (bytes_read == -1)
			return ;
		while (ft_isalnum(buffer[0]) == 0)
			buffer[0] = (buffer[0] % 93) + 33;
		command->random_name[i] = buffer[0];
		i++;
	}
	command->random_name[0] = '.';
	command->random_name[i] = '\0';
	close(urandom_fd);
}

int	test_open(t_command *command)
{
	if (access(command->random_name, F_OK) == -1)
	{
		perror("Error tmp file");
		return (-1);
	}
	if (access(command->random_name, W_OK) == -1)
	{
		perror("Error tmp file");
		return (-1);
	}
	return (0);
}
