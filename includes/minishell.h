/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:32:07 by asuc              #+#    #+#             */
/*   Updated: 2024/02/03 14:17:25 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>

typedef struct s_env
{
	char		*name;
	char		*value;
	char		**env;
}				t_env;

#endif
