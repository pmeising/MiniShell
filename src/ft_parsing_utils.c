/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:26:53 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/01 16:06:39 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
*	removes the quotes from a string;
*	used to remove the quotes in builtin names,
*	arguments, input/outputfiles and heredoc;
*/
void	ft_remove_quotes(char *content)
{
	int	i;
	int	j;
	char tmp;
	
	i = 0;
	j = 1;
	// printf("con: %s\nlen: %d\n", content, len);
	if (content[0] == 34 || content[0] == 39)
	{
		while (content[i] && (content[j] != 34 && content[j] != 39))
		{
			tmp = content[j];
			content[i] = tmp;
			i++;
			j++;
		}
		content[i] = '\0';
		content[j] = '\0';
	}
}

/*
* skips non existent variables
*/
void	ft_skip_var(char *raw_input, char *var_name, int start, int pos)
{
	int	len;

	len = ft_strlen(var_name);
	if (raw_input[start - 1] == '{')
	{
		len++;
		start--;
	}
	len++;
	start--;
	if (raw_input[pos] == '}')
	{
		len++;
		pos++;
	}
	while (raw_input[start] && raw_input[pos])
	{
		g_mini.raw_input[start] = raw_input[pos];
		pos++;
		start++;
		len--;
	}
	while (len > 0)
	{
		g_mini.raw_input[start] = '\0';
		len--;
		start++;
	}
}
