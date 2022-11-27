/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 19:36:54 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 19:05:26 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
*	If we have only one >, we need to overwrite the output file content,
*	else, we need to add the content to the output. APPEND additionally,
*	we skip one pos.
*	then we skip all spaces. save the start pos of the file name,
*	and pull out the file_name of where to redirect the output to.
*	pos = the first < or >
*	open_flag[j] (which is 1) because at [0] is the open_flag for our
*	default output_file.txt
*/
int	ft_get_redir_tok(t_cmd *cmd, char *raw_input, int pos)
{
	int	i;

	i = pos;
	cmd->open_flag = 0;
	if (raw_input[pos] == '>' && raw_input[pos + 1] == '>')
	{
		cmd->open_flag = 1;
		pos++;
	}
	else if (raw_input[pos] == '<' && raw_input[pos + 1] == '<')
		pos++;
	pos++;
	pos = ft_skip_spaces(raw_input, pos);
	if (raw_input[i] == '>')
	{
		if (ft_get_redir_tok_1(cmd, raw_input, &pos, &i) == -1)
			return (-1);
	}
	else if (raw_input[i] == '<' && raw_input[i + 1] == '<')
		ft_get_redir_tok_2(cmd, raw_input, &pos);
	else if (raw_input[i] == '<')
		ft_get_redir_tok_3(cmd, raw_input, &pos);
	pos = ft_get_token_pos(raw_input, pos);
	return (pos);
}

/*
* returns 1 if string contains digit only;
*/
int	ft_is_only_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}

/*
* checks if there is an exit command in
* the userinput;
*/
void	ft_find_exit(void)
{
	t_cmd	*cmd;

	cmd = g_mini.cmds;
	while (cmd)
	{
		if (cmd->toks && ft_is_exit(cmd->toks->content))
		{
			g_mini.exit = 1;
			if (cmd->toks->next != NULL)
			{
				if (cmd->toks->next && \
					ft_is_only_digit(cmd->toks->next->content) == 1)
					g_mini.exit_status = ft_atoi(cmd->toks->next->content);
				else
					ft_find_exit_1(cmd);
			}
			else
				g_mini.exit_status = 0;
			break ;
		}
		cmd = cmd->next;
	}
}

int	ft_parsing_1(t_cmd *cmd, char *raw_input, int *pos)
{
	t_list	*new_tok;

	while (raw_input[*pos] && ft_is_cmd_delim(raw_input[*pos]) == 0)
	{
		*pos = ft_skip_spaces(raw_input, *pos);
		if (raw_input[*pos] != '>' && raw_input[*pos] != '<')
		{
			new_tok = ft_lstnew(ft_get_token(raw_input, *pos));
			if (!new_tok)
				exit_program(1);
			ft_lstadd_back(&cmd->toks, new_tok);
			*pos = *pos + ft_strlen((char *)new_tok->content);
			*pos = ft_skip_spaces(raw_input, *pos);
		}
		else if (raw_input[*pos] == '>' || raw_input[*pos] == '<')
		{
			*pos = ft_get_redir_tok(cmd, raw_input, *pos);
			if (*pos == -1)
				return (1);
			*pos = ft_skip_spaces(raw_input, *pos);
		}
	}
	return (0);
}

/*
*	splits the user-input into commands by separating them by pipe;
*	then splits the commands into smaller units of execution called tokens;
*	a list for commands containing a lists of their
*	corresponding tokens is created;
*	last if is to verify that we are not at '\0' yet;
*/
void	ft_parsing(char *raw_input)
{
	int		pos;
	t_cmd	*cmd_iterator;

	pos = 0;
	while (raw_input[pos])
	{
		cmd_iterator = ft_lstnew_cmds();
		if (ft_parsing_1(cmd_iterator, raw_input, &pos) == 1)
			return ;
		ft_lstadd_back_cmds(&g_mini.cmds, cmd_iterator);
		if (raw_input[pos] == '|')
		{
			pos ++;
			if (cmd_iterator->open_flag == -1)
				cmd_iterator->open_flag = 0;
		}
		else if (!raw_input[pos])
			break ;
	}
	if (g_mini.exit_status == 0)
		ft_2();
	ft_find_exit();
}
