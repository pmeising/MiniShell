/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:26:53 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 18:54:05 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_remove_quotes_1(char *content, int k)
{
	int		i;
	int		j;
	char	tmp;

	i = 0;
	j = 1;
	while (content[i] && content[j] != k)
	{
		tmp = content[j];
		content[i] = tmp;
		i++;
		j++;
	}
	content[i] = '\0';
	content[j] = '\0';
}

/*
*	removes the quotes from a string;
*	used to remove the quotes in builtin names,
*	arguments, input/outputfiles and heredoc;
*/
void	ft_remove_quotes(char *content)
{
	if (content[0] == 34)
		ft_remove_quotes_1(content, 34);
	else if (content[0] == 39)
		ft_remove_quotes_1(content, 39);
}

void	ft_skip_var_1(int *len, int *start)
{
	*len = *len + 1;
	*start = *start - 1;
}

/*
* skips non existent variables
*/
void	ft_skip_var(char *raw_input, char *var_name, int start, int pos)
{
	int	len;

	len = ft_strlen(var_name);
	if (raw_input[start - 1] == '{')
		ft_skip_var_1(&len, &start);
	ft_skip_var_1(&len, &start);
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

void	ft_find_exit_1(t_cmd *cmd)
{
	g_mini.exit_status = 255;
	printf("42shell: %s: numeric argument required\n", \
	cmd->toks->next->content);
}
