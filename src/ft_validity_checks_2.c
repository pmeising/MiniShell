/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validity_checks_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:53:12 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/21 17:32:13 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* checks for invalid char-sequences in user-input;
* returns 1 if invalid sequences detected or 0 if not;
*/
int	ft_not_supported(char *raw_input)
{
	int	check;

	if (ft_contains_not_supported(raw_input, "*") || \
		ft_contains_not_supported(raw_input, "\\") || \
		ft_contains_not_supported(raw_input, "&"))
		check = 1;
	else
		check = 0;
	return (check);
}

/*
* checks for unclosed quotes and forbidden chars
* in the user input;
*/
int	ft_qsb(void)
{
	if (ft_check_quotes(g_mini.raw_input) == 1)
	{
		printf("42shell: quotes not closed\n");
		g_mini.exit_status = 1;
		return (1);
	}
	if (ft_check_backslash(g_mini.raw_input) == 1)
	{
		printf("42shell: ´/' forbidden\n");
		g_mini.exit_status = 1;
		return (1);
	}
	if (ft_check_semicolon(g_mini.raw_input) == 1)
	{
		printf("42shell: ´;' forbidden\n");
		g_mini.exit_status = 1;
		ft_free_input();
		return (1);
	}
	return (0);
}

/*
* checks if input from user is valid;
* if not the respective checking function
* prints out an error message;
* returns a value back to the calling main which
* indicates continuation or stopping of shell loop ?
* returns 0 if string is only whitespace or contains invalid input
* returns 1 if no invalid input was detected
*/
int	ft_check_input_validity(char *raw_input)
{
	int	check;

	if (!raw_input || ft_str_only_space(raw_input) == 1)
		return (0);
	else if (ft_qsb() == 1)
		check = 0;
	else if (ft_wrong_tokens(raw_input) == 1 \
			|| ft_not_supported(raw_input) == 1)
		check = 0;
	else
		check = 1;
	return (check);
}

/*
* checks if the illegal_str/sequence is in the user-input;
*/
int	ft_contains_not_supported(char *raw_input, char *illegal_str)
{
	int	check;

	if (ft_strstr_quotes(raw_input, illegal_str) != 0)
	{
		check = 1;
		printf("%s is not supported\n", illegal_str);
	}
	else
		check = 0;
	return (check);
}
