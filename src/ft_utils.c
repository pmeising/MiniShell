/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 19:22:21 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/14 19:34:13 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_chars(char *cur_cwd, char *new_cwd)
{
	free (cur_cwd);
	free (new_cwd);
}

void	ft_check(char *cwd)
{
	if (!cwd)
	{
		perror("malloc :");
		free (cwd);
		exit_program(1);
	}
}
