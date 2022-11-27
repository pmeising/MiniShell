/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_funcs_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 18:39:36 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/27 20:52:17 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_input_built_in(void)
{
	t_cmd	*cmd_iterator;
	t_cmd	*temp;

	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		ft_free_helper(cmd_iterator);
		temp = cmd_iterator->next;
		free (cmd_iterator);
		cmd_iterator = temp;
	}
	ft_free_fds();
	if (g_mini.dup_env)
		ft_free_lst_cont(g_mini.dup_env);
	if (g_mini.raw_input)
		ft_free_str(&g_mini.raw_input);
	if (g_mini.env && g_mini.special_flag > 0)
		free (g_mini.env);
	exit (0);
}

void	ft_free_helper_1(void)
{
	if (g_mini.raw_input)
		free (g_mini.raw_input);
	g_mini.raw_input = NULL;
	g_mini.cmds = NULL;
	g_mini.nbr_heredocs = 0;
	ft_free_fds();
	g_mini.nbr_of_pipes = 0;
}
