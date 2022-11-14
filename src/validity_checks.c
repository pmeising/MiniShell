/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validity_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:34:35 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/14 19:29:00 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* checks if there is a backslash in the raw user-input;
*/
int	ft_check_backslash(char *raw_input)
{
	int	i;

	i = 0;
	while (raw_input[i])
	{
		if (raw_input[i] == 34 || raw_input[i] == 39)
			ft_skip_quotes(raw_input, i);
		if (raw_input[i] == 92)
			return (1);
		i++;
	}
	return (0);
}

/*
* checks if there is a semicolon in the raw user-input;
*/
int	ft_check_semicolon(char *raw_input)
{
	int	i;

	i = 0;
	while (raw_input[i])
	{
		if (raw_input[i] == 34 || raw_input[i] == 39)
			ft_skip_quotes(raw_input, i);
		if (raw_input[i] == 59)
			return (1);
		i++;
	}
	return (0);
}

/*
* checks if the user-input is only white space;
*	returns 1 if only space.
*	returns 0 if at least one not space character.
*/
int	ft_str_only_space(char *raw_input)
{
	int	i;
	int	check;

	i = 0;
	check = 1;
	if (!raw_input)
		return (0);
	while (raw_input[i] && ft_is_space(raw_input[i]) == 1)
		i++;
	if (i == (int)ft_strlen(raw_input))
		check = 1;
	else
		check = 0;
	return (check);
}

/*
* checks if there are only spaces between two same chars;
* there shouldnt be any space because it would make >> || invalid commands;
* does not apply on content in between quotes;
*/
int	ft_space_between_char(char *raw_input, char c)
{
	int	i;

	i = 0;
	if (!raw_input)
		return (0);
	while (raw_input[i] != '\0')
	{
		if (raw_input[i] == 34 || raw_input[i] == 39)
			i = ft_skip_quotes(raw_input, i);
		if (raw_input[i] == c && ft_is_space(raw_input[i + 1]) == 1)
		{
			i++;
			i = ft_skip_spaces(raw_input, i);
			if (raw_input[i] == c)
			{
				printf("only white space between same chars detected\n");
				return (1);
			}
		}
		if (raw_input[i])
			i++;
	}
	return (0);
}

/*
* checks if the illegal_str is contained in user_input but
* only if outside of quotes, because inside of quotes its legal;
* last if: is needed to verify if raw_input is already \0 or not;
* returns 0 if nothing illegal was found or the position where
* illegal_str starts;
*/
char	*ft_strstr_quotes(char *raw_input, char *illegal_str)
{
	int	i;

	i = 0;
	if (!illegal_str)
		return (illegal_str);
	while (raw_input[i])
	{
		if (raw_input[i] == 34 || raw_input[i] == 39)
			i = ft_skip_quotes(&raw_input[i], i);
		if (!ft_strncmp(&raw_input[i], illegal_str, ft_strlen(illegal_str)))
			return (raw_input);
		if (raw_input[i])
			i++;
	}
	return (NULL);
}
