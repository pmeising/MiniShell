/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 17:25:54 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 17:41:06 by bde-carv         ###   ########.fr       */
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
* inserts the content of the env-variable into the user-input string;
* start = Beginn of variable name; pos = is the first char after the
* environmentvariable name;
*/
void	ft_insert(char *raw_input, char *dup_var_cont, int pos, int start)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_str = ft_calloc(((ft_strlen(dup_var_cont) + \
			ft_strlen(raw_input))), sizeof(char));
	ft_check_malloc(new_str);
	if (raw_input[start - 1] == '{')
		start--;
	start--;
	while (raw_input[i] && i < start)
	{
		new_str[i] = raw_input[i];
		i++;
	}
	while (dup_var_cont[j])
		new_str[i++] = dup_var_cont[j++];
	free(dup_var_cont);
	while (raw_input[pos])
		new_str[i++] = raw_input[pos++];
	free (g_mini.raw_input);
	g_mini.raw_input = new_str;
}

void	help_put_env_in_input(char **raw_input, \
		char**var_name, int *start, int *pos)
{
	ft_skip_var(*raw_input, *var_name, *start, *pos);
	if (*var_name)
	{
		free(*var_name);
		*var_name = NULL;
	}
}

/*
*	Identifies the Variable name and calls functions, 
*	which verify it as well as replace the variable with its content.
*	ft_compare_env verifies, ft_insert replaces it.
*	pos = position of the $
*/
void	ft_put_env_in_input(char *raw_input, int pos)
{
	int		start;
	char	*var_name;
	char	*dup_var_content;

	pos++;
	if (raw_input[pos] == '{')
		ft_bad_sub(raw_input, &pos);
	start = pos;
	while (raw_input[pos] && ft_isalnum(raw_input[pos]) == 1)
		pos++;
	var_name = ft_substr(raw_input, start, pos - start);
	dup_var_content = ft_extract_content(var_name);
	if (dup_var_content == NULL)
	{
		help_put_env_in_input(&raw_input, &var_name, &start, &pos);
		return ;
	}
	if (raw_input[pos] == '}')
		pos++;
	if (var_name)
	{
		free(var_name);
		var_name = NULL;
	}
	ft_insert(raw_input, dup_var_content, pos, start);
}
