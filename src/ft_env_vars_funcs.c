/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_vars_funcs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:42:08 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/23 20:42:07 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		ft_skip_var(raw_input, var_name, start, pos);
		if (var_name)
		{
			free(var_name);
			var_name = NULL;
		}
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

/*
	Finds and replaces the "$?" with the exit status value.
	replaces the string content with the exit status.
*/
void	ft_replace_dollar_question_mark(char *raw_input, int pos)
{
	char	*exit_status_str;
	char	*before;
	char	*after;
	char	*half_str;
	int		i;

	i = 0;
	before = malloc(sizeof(char) * ft_strlen(raw_input));
	while (raw_input[i] && i < pos)
	{
		before[i] = raw_input[i];
		i++;
	}
	before[i] = '\0';
	after = strdup(&raw_input[pos + 2]);
	exit_status_str = ft_itoa(g_mini.exit_status);
	half_str = ft_strjoin(before, exit_status_str);
	free (before);
	free (exit_status_str);
	free (g_mini.raw_input);
	g_mini.raw_input = ft_strjoin(half_str, after);
	free (half_str);
	free (after);
}

/*
*	Walks through the input string and looks for a dollar signs ($).
*	Once found, it checks, whether it is followed by...
*	... a questionmark (?), in which case we show the most recently 
*		executed foreground pipeline.
*	... a quotationmark (" or '), in which case we replace 
*		the $ with a blankspace.
*	... a valid environment variable, in which case we insert the content
*		of the env into the string using ft_str_join.
*
*	1 would be outer quote, -1 inner quote and 0 no quote(closed quote);
*	34 = double quotes; 39 = single quotes;
*	all $ that are not interpreted will be deleted by shifting 
*	the string one position to the left
*	(last else-if statement);
*	edge case: $(PATH) should throw an error. Unable to handle it at this point
*	not required by subject pdf. --> see comment in 
*/
void	ft_env_vars(char *raw_input)
{
	int	pos;
	int	j;
	int	single_quotes;
	int	double_quotes;

	pos = 0;
	j = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (raw_input[pos])
	{
		ft_quotes(raw_input, &single_quotes, &double_quotes, &pos);
		if (raw_input[pos] == '$' && single_quotes == 1)
		{
			while (raw_input[pos] != 39)
				pos++;
			single_quotes = 0;
		}
		if (ft_check_doll(raw_input, &pos) == NULL)
			break ;
		j++;
		pos++;
	}
}
		// if (raw_input[pos] == '$' && raw_input[pos + 1] == '?')
		// {
		// 	ft_replace_dollar_question_mark(raw_input, pos);
		// 	break ;
		// }
		// else if (raw_input[pos] == '$' && (raw_input[pos + 1] == '{'
		// 		|| ft_isalnum(raw_input[pos + 1]) == 1))
		// {
		// 	ft_put_env_in_input(raw_input, pos);
		// 	break ;
		// }
		// else if (raw_input[pos] == '$')
		// {
		// 	j = pos;
		// 	pos++;
		// 	while (raw_input[pos])
		// 		g_mini.raw_input[j++] = raw_input[pos++];
		// 	g_mini.raw_input[j] = '\0';
		// 	break ;
		// }

/*
*	checks if there are still interpretable dollar signs left in the string
*	If outer_quotes are single quotes -> do not interpret the $
*	If outer_quotes are double quotes -> do interpret the $
*	Returns 1 if a $ still has to be interpreted.
*	Returns 0 if no $ has to be interpreted anymore.
*
*	34 == double quote; 39 == single quote;
*	1 would be outer quote, -1 inner quote and 0 no quote;

* 	double    single	interpret
*	1			0		1
*	1			-1		1
*	0			0		1
*	-1			1		0
*	0			1		0
*/
void	ft_dollar_sign(void)
{
	int		i;
	int		single_quotes;
	int		double_quotes;
	char	*raw;

	i = 0;
	single_quotes = 0;
	double_quotes = 0;
	while (g_mini.raw_input[i])
	{
		raw = g_mini.raw_input;
		if (raw[i] == '$' && (raw[i + 1] == ' ' || raw[i + 1] == '\0'))
			i++;
		ft_quotes(raw, &single_quotes, &double_quotes, &i);
		if (raw[i] == '$' && single_quotes == 1)
		{
			while (raw[i] != 39)
				i++;
			single_quotes = 0;
		}
		else if (raw[i] == '$')
			ft_env_vars(g_mini.raw_input);
		if (g_mini.raw_input[i])
			i++;
	}
}
