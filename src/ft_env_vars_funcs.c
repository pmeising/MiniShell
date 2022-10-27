/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_vars_funcs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:42:08 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/26 20:21:09 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int 	i;
	int		len;
	int		len2;
	char	*content;
	char	*test; // better name content or content_check
	t_list	*iterator;

	i = 0;
	iterator = g_mini.dup_env;
	// printf("var_name: %s\n", var_name);
	len = ft_strlen(var_name);
	// printf("len: %d\n", len);
	while (iterator != NULL)
	{
		test = iterator->content;
		if (ft_strnstr(iterator->content, var_name, len) != NULL)
			break ;
		iterator = iterator->next;
	}
	if (iterator == NULL)
	{
		printf("Couldn't find ENV VAR.\n");
		return (NULL);
	}
	if (ft_strlen(test) == (size_t)(len + 1))
		return (NULL);
	len2 = ft_strlen(&test[len + 1]);
	// printf("len2: %d\n", len2);
	// printf("%s\n", test);
	content = ft_substr(test, len + 1, len2);
	// printf("%s\n", content);
	return (content);
}

/*
* inserts the content of the env-variable into the user-input string;
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
	if (!new_str)
	{
		printf("malloc error ft_insert\n");
		exit_program(EXIT_FAILURE);
	}
	if (raw_input[start - 1] == '{')
		start--;
	start--;
	// printf("i: %d, start: %d, pos: %d\n", i, start, pos);
	while (raw_input[i] && i < start)
	{
		new_str[i] = raw_input[i];
		i++;
	}
	while (dup_var_cont[j])
	{
		new_str[i] = dup_var_cont[j];
		i++;
		j++;
	}
	while (raw_input[pos])
	{
		new_str[i] = raw_input[pos];
		i++;
		pos++;
	}
	free (g_mini.raw_input);
	g_mini.raw_input = new_str;
}

/*
*	Identifies the Variable name and goes on calling functions, 
*	which verify it as well as replace the variable with its content.
*	ft_compare_env verifies, ft_insert replaces it.
*/
void	ft_put_env_in_input(char *raw_input, int pos)
{
	int		start;
	char	*var_name;
	char	*dup_var_content;

	pos++;
	if (raw_input[pos] == '{')
	{
		pos++;
		if (raw_input[pos] == '}')
		{
			printf("${}: bad substitution\n");
			exit_program(EXIT_FAILURE);
		}
	}
	start = pos;
	while (raw_input[pos] && ft_isalnum(raw_input[pos]) == 1)
		pos++;
	var_name = ft_substr(raw_input, start, pos - start);
	dup_var_content = ft_extract_content(var_name); // better call exrtact_content
	if (dup_var_content == NULL)
	{
		ft_skip_var(raw_input, var_name, start, pos);
		return ;
	}
	if (raw_input[pos] == '}')
		pos++;
	ft_insert(raw_input, dup_var_content, pos, start);
}

/*
*	Walks through the input string and looks for a dollar sign ($).
*	Once found, it checks, whether it is followed by...
*	... a questionmark (?), in which case we show the most recently 
*		executed foreground pipeline.
*	... a quotationmark (" or '), in which case we replace the $ with a blankspace.
*	... a valid environment variable, in which case we insert the content
*		of the env into the string using ft_str_join.
*	edge case: $(PATH) should throw an error. Unable to handle it at this point
*	not required by subject pdf.
*/
void ft_env_vars(char *raw_input)
{
	int pos;
	int	j;
	int	ignore;
	int	single_quotes;
	int	double_quotes;

	pos = 0;
	j = 0;
	ignore = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (raw_input[pos])
	{
		if (raw_input[pos] == 34) // double_quotes
		{
			if (single_quotes == 0)
				double_quotes = 1;
			else if (single_quotes == 1)
				double_quotes = -1;
			else if (double_quotes != 0)
				double_quotes = 0;
		}
		if (raw_input[pos] == 39) // single_quotes
		{
			if (double_quotes == 0)
				single_quotes = 1;
			else if (double_quotes == 1)
				single_quotes = -1;
			else if (double_quotes == -1)
				single_quotes = 0;
		}
		if (raw_input[pos] == '$' && single_quotes == 1)
		{
			while (raw_input[pos] != 39)
				pos++;
			single_quotes = 0;
		}
		// printf("HI there, pos is now: %d\n", pos);
		if (raw_input[pos] == '$' && raw_input[pos + 1] == '?')
		{// $? is a variable holding the return value of the last command you ran.
			printf("Need to expand to the exit status of the most recently \
					executed foreground pipeline.\n");
			// printf("%d\n", g_mini.last_exit_status);
			break ;
		}
		else if (raw_input[pos] == '$' && (raw_input[pos + 1] == '{' || ft_isalnum(raw_input[pos + 1]) == 1))
		{
			// printf("want to replace input.\n");
			ft_put_env_in_input(raw_input, pos);
			break ;
		}
		else if (raw_input[pos] == '$')
		{
			j = pos;
			pos++;
			while (raw_input[pos])
			{
				g_mini.raw_input[j] = raw_input[pos];
				j++;
				pos++;
			}
			g_mini.raw_input[j] = '\0';
			break ;
		}
		j++;
		pos++;
	}
}

/*
*	checks if there are still interpretable dollar signs left in the string
*	If outer_quotes are single quotes -> do not interpret the $
*	If outer_quotes are double quotes -> do interpret the $
*	Returns 1 if a $ still has to be interpreted.
*	Returns 0 if no $ has to be interpreted anymore.
*
* 	double    single	interpret
*	1			0		1
*	1			-1		1
*	0			0		1
*	-1			1		0
*	0			1		0
*/
int ft_dollar_sign(char *raw_input)
{
	int i;
	int	single_quotes;
	int	double_quotes;
	int check;
// '$PATH' "$PATH"
	i = 0;
	single_quotes = 0; // 1 would be outer quote, -1 inner quote and 0 no quote
	double_quotes = 0; // 1 would be outer quote, -1 inner quote and 0 no quote
	check = 0;
	while (raw_input[i])
	{
		if (raw_input[i] == '$' && raw_input[i + 1] == ' ')
			i++;
		if (raw_input[i] == 34) // double_quotes
		{
			if (single_quotes == 0)
				double_quotes = 1;
			else if (single_quotes == 1)
				double_quotes = -1;
			else if (double_quotes != 0)
				double_quotes = 0;
		}
		if (raw_input[i] == 39) // single_quotes
		{
			if (double_quotes == 0)
				single_quotes = 1;
			else if (double_quotes == 1)
				single_quotes = -1;
			else if (double_quotes == -1)
				single_quotes = 0;
		}
		if (raw_input[i] == '$' && single_quotes == 1)
		{
			while (raw_input[i] != 39)
				i++;
			single_quotes = 0;
		}
		else if (raw_input[i] == '$')
			check++;
		i++;
	}
	// printf("check: %d\n", check);
	return (check);
}
