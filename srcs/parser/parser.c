/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbuchs <mael@buchs.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 18:06:59 by mbuchs            #+#    #+#             */
/*   Updated: 2024/02/23 18:17:23 by mbuchs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void parser(t_data *data)
{
// 	parser

// if ->top node == word
// 	command_top == word
// while -> node == word
// 	join_tab to command->top->args
	
// if << -> heredoc

// if < -> if next == word
// 	cat "word" in args

// if pipe
// 	pipexS

// if >> OR >
// 	change output to file
}