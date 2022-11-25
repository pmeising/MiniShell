/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 19:22:21 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/15 16:55:28 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_chars(char *cur_cwd, char *new_cwd)
{
	free (cur_cwd);
	free (new_cwd);
}

void	ft_check_malloc(char *str)
{
	if (!str)
	{
		perror("malloc :");
		free (str);
		exit_program(1);
	}
}
