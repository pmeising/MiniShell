/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:14:00 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/27 15:41:45 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_read_heredoc_2(t_cmd *cmd, int fd_input_file)
{
	char	*content;

	while (1)
	{
		content = readline("heredoc: ");
		if (ft_strncmp(content, cmd->heredoc_delim, \
			ft_strlen(cmd->heredoc_delim)) == 0)
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
* heredoc function that takes in input from user;
* then writes user input into input file,
* until delim is put in by user;
*/
void	ft_read_heredoc(t_cmd *cmd)
{
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
	ft_read_heredoc_2(cmd, fd_input_file);
}

int	ft_get_redir_tok_1(t_cmd *cmd, char *raw_input, int *pos, int *i)
{
	char	*temp;

	temp = ft_get_token_1(raw_input, *pos);
	if (!temp || temp[0] == '\0')
	{
		ft_1();
		return (-1);
	}
	else
	{
		ft_remove_quotes(temp);
		cmd->output_file = temp;
		if (raw_input[*i + 1] != '>')
			cmd->open_flag = 0;
		ft_open_file_2(temp, &cmd->fd_out, 1, cmd->open_flag);
	}
	return (0);
}

void	ft_get_redir_tok_2(t_cmd *cmd, char *raw_input, int *pos)
{
	char	*temp;

	g_mini.nbr_heredocs++;
	temp = ft_get_token_1(raw_input, *pos);
	if (!temp || temp[0] == '\0')
		ft_1();
	else
	{
		ft_remove_quotes(temp);
		cmd->heredoc_delim = temp;
	}
}

void	ft_get_redir_tok_3(t_cmd *cmd, char *raw_input, int *pos)
{
	char	*temp;

	temp = ft_get_token_1(raw_input, *pos);
	if (!temp || temp[0] == '\0')
		ft_1();
	else
	{
		ft_remove_quotes(temp);
		if (cmd->input_file == NULL)
			cmd->input_file = temp;
	}	
}
