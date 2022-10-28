/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validity_checks_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:53:12 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/28 18:00:49 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

/*
* checks for invalid char-sequences in user-input;
* returns 1 if invalid sequences detected or 0 if not;
*/
int ft_not_supported(char *raw_input)
{
	int check;

	if (ft_contains_not_supported(raw_input, "*") || \
		ft_contains_not_supported(raw_input, "\\") || \
		ft_contains_not_supported(raw_input, "&"))
		check = 1;
	else
		check = 0;
	return (check);
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
int ft_check_input_validity(char *raw_input)
{
	int check;

	if (!raw_input || ft_str_only_space(raw_input) == 1)
	{
		check = 0;
	}
	else if (ft_wrong_tokens(raw_input) == 1 || ft_not_supported(raw_input) == 1)
	{
		check = 0;
		// set g_mini exit status
		printf("invalid or not supported input detected\n");
	}
	else
	{
		check = 1;
	}
	return (check);
}
