/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delim_funcs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 18:39:49 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/25 19:34:26 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* checks if char c (position on raw_input string) is a delimitor for tokens;
*/
int ft_is_tok_delim(char c)
{
	int check;

	if (ft_strchr(BLANK_SPACE, c))
		check = 1;
	else if (c == '<')
		check = 1;
	else if (c == '>')
		check = 1;
	else if (c == '|')
		check = 1;
	else if (c == '&')
		check = 1;
	else
		check = 0;
	return (check);
}

/*
** Checks whether the input line has reached the command delimiter.
** In our project the only allowed command delimiter is '|'.
*/
int	ft_is_cmd_delim(char c)
{
	int	check;

	check = 0;
	if (c == '|')
		check = 1;
	return (check);
}
