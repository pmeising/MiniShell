/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 19:36:54 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 15:41:22 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
*	This function does the exact same thing as the ft_get_token,
*	but instead of the string, we return the int position.
*	Returns the int after the token has ended.
*/
int	ft_get_token_pos(char *raw_input, int pos)
{
	while (raw_input[pos] && ft_is_tok_delim(raw_input[pos]) == 0)
	{
		if (raw_input[pos] == 34 || raw_input[pos] == 39)
			pos = ft_skip_quotes(raw_input, pos);
		else
			pos++;
	}
	return (pos);
}

char	*ft_get_token_1(char *raw_input, int pos)
{
	int		begin_pos;
	char	*token;

	begin_pos = pos;
	while (raw_input[pos] && raw_input[pos] != '|' && raw_input[pos] != ' ')
	{
		if (raw_input[pos] == 34 || raw_input[pos] == 39)
			pos = ft_skip_quotes(raw_input, pos);
		else
			pos++;
	}
	token = ft_substr(raw_input, begin_pos, (pos - begin_pos));
	if (!token)
	{
		perror("Substring: ");
		exit_program(1);
	}
	return (token);
}

/*
*	Identifies each token of the string.
*	Examples string: > test.txt echo Hello World, what's up?
*	token1: test.txt
*	token2: echo
*	token3: Hello
*	token4: World,
*	token5: what's
*	token6: up
*	These tokens are then interpreted depending ontheir redirects 
*	and whether or not they are commands or arguments etc.
*	pos = first char in user_input thats not a space;
*/
char	*ft_get_token(char *raw_input, int pos)
{
	int		begin_pos;
	char	*token;

	begin_pos = pos;
	while (raw_input[pos] && ft_is_tok_delim(raw_input[pos]) == 0)
	{
		if (raw_input[pos] == 34 || raw_input[pos] == 39)
			pos = ft_skip_quotes(raw_input, pos);
		else
			pos++;
	}
	token = ft_substr(raw_input, begin_pos, (pos - begin_pos));
	if (!token)
	{
		perror("Substring: ");
		exit_program(1);
	}
	return (token);
}

void	ft_1(void)
{
	if (g_mini.exit_status == 0)
	{
		printf("42shell: syntax error near unexpected token `|'\n");
		g_mini.exit_status = 258;
		g_mini.exit = 0;
	}
}

void	ft_2(void)
{
	t_cmd	*cmd;

	cmd = g_mini.cmds;
	while (cmd)
	{
		if (cmd->heredoc_delim)
			ft_read_heredoc(cmd);
		cmd = cmd->next;
	}
}
