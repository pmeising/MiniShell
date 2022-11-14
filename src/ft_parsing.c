/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 19:36:54 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/14 19:28:59 by bde-carv         ###   ########.fr       */
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

/*
* heredoc function that takes in input from user;
* then writes user input into input file,
* until delim is put in by user;
*/
void	ft_read_heredoc(t_cmd *cmd)
{
	char	*content;
	int		fd_input_file;
	char	*temp_name;
	char	*nbr;

	nbr = ft_itoa(g_mini.nbr_heredocs);
	temp_name = ft_strjoin(nbr, "temp.txt");
	if (!temp_name)
	{
		printf("ft_read_heredoc:strjoin failed\n");
		exit_program(1);
	}
	free (nbr);
	cmd->input_file = temp_name;
	cmd->heredoc_temp = temp_name;
	fd_input_file = open(temp_name, O_RDWR | O_CREAT, 0777);
	if (fd_input_file < 0)
		printf("Failed to open file.\n");
	while (1)
	{
		content = readline("heredoc: ");
		if (ft_strncmp(content, cmd->HEREDOC_DELIM, \
			ft_strlen(cmd->HEREDOC_DELIM)) == 0)
		{
			free (content);
			close(fd_input_file);
			break ;
		}
		else
		{
			write(fd_input_file, content, ft_strlen(content));
			write(fd_input_file, "\n", 1);
		}
		free (content);
	}
}

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
	int		i;
	char	*temp;

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
		temp = ft_get_token(raw_input, pos);
		ft_remove_quotes(temp);
		cmd->output_file = temp;
		if (raw_input[i + 1] != '>')
			cmd->open_flag = 0;
	}
	else if (raw_input[i] == '<' && raw_input[i + 1] == '<')
	{
		g_mini.nbr_heredocs++;
		temp = ft_get_token(raw_input, pos);
		ft_remove_quotes(temp);
		cmd->HEREDOC_DELIM = temp;
		ft_read_heredoc(cmd);
	}
	else if (raw_input[i] == '<')
	{
		temp = ft_get_token(raw_input, pos);
		ft_remove_quotes(temp);
		if (cmd->input_file == NULL)
			cmd->input_file = temp;
	}
	pos = ft_get_token_pos(raw_input, pos);
	return (pos);
}

/*
* checks if there is an exit command in
* the userinput;
*/
void	ft_find_exit(void)
{
	t_cmd	*cmd_iterator;

	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		if (ft_is_exit(cmd_iterator->toks->content))
		{
			g_mini.exit = 1;
			break ;
		}
		cmd_iterator = cmd_iterator->next;
	}
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
	t_list	*new_tok;
	t_cmd	*cmd_iterator;

	pos = 0;
	while (raw_input[pos])
	{
		cmd_iterator = ft_lstnew_cmds();
		while (raw_input[pos] && ft_is_cmd_delim(raw_input[pos]) == 0)
		{
			pos = ft_skip_spaces(raw_input, pos);
			if (raw_input[pos] != '>' && raw_input[pos] != '<')
			{
				new_tok = ft_lstnew(ft_get_token(raw_input, pos));
				if (!new_tok)
					exit_program(1);
				ft_lstadd_back(&cmd_iterator->toks, new_tok);
				pos = pos + ft_strlen((char *)new_tok->content);
				pos = ft_skip_spaces(raw_input, pos);
			}
			else if (raw_input[pos] == '>' || raw_input[pos] == '<')
			{
				pos = ft_get_redir_tok(cmd_iterator, raw_input, pos);
				pos = ft_skip_spaces(raw_input, pos);
			}
		}
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
	ft_find_exit();
}
