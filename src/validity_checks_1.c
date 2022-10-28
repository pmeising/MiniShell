/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validity_checks_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:38:09 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/28 17:57:09 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

/*
* checks if the raw user-input starts with a certain character;
* before checking it makes a copy in which it trims whitespaces
* from beginning and end;
*/
int ft_is_char_front(char *raw_input, char c)
{
	int		check;
	char	*trim_input;

	trim_input = ft_strtrim(raw_input, BLANK_SPACE);
	if (!trim_input)
	{
		printf("no input after trimming in is_char_front\n");
		exit_program(EXIT_FAILURE);
	}
	if (trim_input[0] == c)
	{
		printf("error:1st char illegal\n");
		check = 1;
	}
	else
		check = 0;
	free(trim_input);
	return (check);
}

/*
* checks if the raw user-input has a certain character at the last position;
* before checking it makes a copy in which it trims whitespaces
* from beginning and end;
*/
int	ft_is_char_end(char *raw_input, char c)
{
	int check;
	char *trim_input;
	int len;

	trim_input = ft_strtrim(raw_input, &c);
	if (!trim_input)
	{
		printf("no input after trimming in is_char_front\n");
		exit_program(EXIT_FAILURE);
	}
	len = ft_strlen(trim_input);
	if (trim_input[len - 1] == c)
	{
		printf("error:last char illegal\n");
		check = 1;
	}
	else
		check = 0;
	free(trim_input);
	return(check);
}

/*
* detects illegal sequences in user-input;
*/
int ft_illegal_sequence(char *raw_input, char *sequence)
{
	int check;
	char *trim_input;

	trim_input = ft_strtrim_all(raw_input, BLANK_SPACE);
	if (!trim_input)
	{
		printf("no trim_input\n");
		exit_program(EXIT_FAILURE);
	}
	if (ft_strstr_quotes(trim_input, sequence))
	{
		check = 1;
		printf("forbidden sequence detected\n");
	}
	else
	{
		check = 0;	
	}
	free(trim_input);
	return (check);
}

/*
* checks if there are wrong sequences of tokens and/or
* or tokens at the wrong place;
* quotes are already checked in ft_get_input
* returns 1 if invalid tokens detected or 0 if not;
*/
int	ft_wrong_tokens(char *raw_input)
{
	int check;

	if (ft_is_char_front(raw_input, '|') || ft_is_char_end(raw_input, '|') || \
		ft_is_char_front(raw_input, ';') || ft_is_char_end(raw_input, '<') || \
		ft_is_char_end(raw_input, '>') || ft_is_char_end(raw_input, '&') || \
		ft_space_between_char(raw_input, '|') || \
		ft_space_between_char(raw_input, '>') || \
		ft_illegal_sequence(raw_input, ";;") || \
		ft_illegal_sequence(raw_input, "|;") || \
		ft_illegal_sequence(raw_input, "&;") || \
		ft_illegal_sequence(raw_input, ";|") || \
		ft_illegal_sequence(raw_input, ";&") || \
		ft_illegal_sequence(raw_input, ">>>") || \
		ft_illegal_sequence(raw_input, ";;"))
	{
		printf("invalid tokens or sequence\n");
		check = 1;
	}
	else
		check = 0;
	return (check);
}

/*
* checks if the illegal_str/sequence is in the user-input;
*/
int ft_contains_not_supported(char *raw_input, char *illegal_str)
{
	int check;
	
	if (ft_strstr_quotes(raw_input, illegal_str) != 0)
	{
		check = 1;
		printf("%s is not supported\n", illegal_str);
	}
	else
	{
		check = 0;
	}
	return (check);
}
