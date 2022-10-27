/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validity_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 19:42:02 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/26 18:51:58 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

/*
* called in ft_strstr_quotes to skip enquoted content;
* goes through an enquoted string until the quote is over;
*/
int ft_skip_quotes(char *raw_input, int pos)
{
	int single_q_open;
	int double_q_open;
	
	single_q_open = 0;
	double_q_open = 0;
	while (raw_input[pos])
	{
		if(raw_input[pos] == 34 && single_q_open == 0)
			double_q_open = !double_q_open;
		else if (raw_input[pos] == 39 && double_q_open == 0)
			single_q_open = !single_q_open;
		pos++;
		if ((double_q_open == 0 && single_q_open == 0) && (raw_input[pos] != 34 && raw_input[pos] != 39))
			break;
	}
	return (pos);
}

/*
* skips any blank space characters in a string starting at pos;
*/
int ft_skip_spaces(char *raw_input, int pos)
{
	while (raw_input[pos] && ft_is_space(raw_input[pos]) == 1)
			pos++;
	return (pos);
}

/*
* checks if single- and double quotes are closed;
* is done by counting them then taking the modulo 2;
* Returns 0 for closed quotes and 1 when open.
*/
int ft_check_quotes(char *raw_input)
{
	int i;
	int single_q;
	int double_q;
	
	i = 0;
	single_q = 0;
	double_q = 0;
	while (raw_input[i])
	{
		if (raw_input[i] == 34)
		{
			double_q++;
			while (raw_input[i + 1] && raw_input[i + 1] != 34)
				i++;
			if (raw_input[i + 1] == 34)
				double_q++;
			i++;
		}
		if (raw_input[i] == 39)
		{
			single_q++;
			while (raw_input[i + 1] && raw_input[i + 1] != 39)
				i++;
			if (raw_input[i + 1] == 39)
				single_q++;
			i++;
		}
		i++;
	}
	if ((single_q % 2 != 0) || (double_q % 2 != 0))
		return (1);
	return (0);
}
