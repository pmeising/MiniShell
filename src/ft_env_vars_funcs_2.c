/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_vars_funcs_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 17:09:49 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/21 18:32:58 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* checks for invalid input by user: ${};
*/
void	ft_bad_sub(char *raw_input, int *pos)
{
	*pos = *pos + 1;
	if (raw_input[*pos] == '}')
	{
		printf("42shell: ${}: bad substitution\n");
		exit_program(1);
	}
}

/*
* helper function for ft_env_vars;
* used for checking if content is in quotes
* and what type of quotes;
* if quote found it sets flags to the opposite
* int value;
*/
void	ft_quotes(char *raw_input, int *single_quotes, \
					int *double_quotes, int *pos)
{
	if (raw_input[*pos] == 34)
	{
		if (*single_quotes == 0)
			*double_quotes = 1;
		else if (*single_quotes == 1)
			*double_quotes = -1;
		else if (*double_quotes != 0)
			*double_quotes = 0;
	}
	if (raw_input[*pos] == 39)
	{
		if (*double_quotes == 0)
			*single_quotes = 1;
		else if (*double_quotes == 1)
			*single_quotes = -1;
		else if (*double_quotes == -1)
			*single_quotes = 0;
	}
}

char	*ft_check_doll(char *raw_input, int *pos)
{
	int	j;

	j = 0;
	if (raw_input[*pos] == '$' && raw_input[*pos + 1] == '?')
	{
		ft_replace_dollar_question_mark(raw_input, *pos);
		return (NULL);
	}
	else if (raw_input[*pos] == '$' && (raw_input[*pos + 1] == '{' \
			|| ft_isalnum(raw_input[*pos + 1]) == 1))
	{
		ft_put_env_in_input(raw_input, *pos);
		return (NULL);
	}
	else if (raw_input[*pos] == '$')
	{
		j = *pos;
		*pos = *pos + 1;
		while (raw_input[*pos])
			g_mini.raw_input[j++] = raw_input[*pos++];
		g_mini.raw_input[j] = '\0';
		return (NULL);
	}
	return ("HI");
}

/*
*	Receives the Variable name and loops through the 2D array of strings of
*	ENV finding the variable name. Uses ft_strnstr.
*	Return: Takes out its content and returns it to the calling function.
*	In case of an empty variable, returns NULL.
*	2nd if checks, wether the env variable is empty or not
*	by comparing the string lengths
*/
char	*ft_extract_content(char *var_name)
{
	int		var_name_len;
	int		content_len;
	char	*content;
	char	*var_name_and_content;
	t_list	*iterator;

	iterator = g_mini.dup_env;
	var_name_len = ft_strlen(var_name);
	while (iterator != NULL)
	{
		var_name_and_content = iterator->content;
		if (ft_strnstr(iterator->content, var_name, var_name_len) != NULL)
			break ;
		iterator = iterator->next;
	}
	if (iterator == NULL)
		return (NULL);
	if (ft_strlen(var_name_and_content) == (size_t)(var_name_len + 1))
		return (NULL);
	content_len = ft_strlen(&var_name_and_content[var_name_len + 1]);
	content = ft_substr(var_name_and_content, var_name_len + 1, content_len);
	return (content);
}
