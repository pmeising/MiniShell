/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:28:28 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/29 17:50:08 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* deletes content of our default input and output file
* these files are contained in: mull/Input_file.txt, mull/Output_file.txt
* O_TRUNC = means truncate, so that the option to overwrite its content is
* enabled;
* EOF = End of File
*/
void ft_free_files(void)
{
	int file_fd_in;
	int file_fd_out;

	file_fd_in = open("mull/Input_file.txt", O_RDWR | O_TRUNC, 0777);
	if (file_fd_in < 0)
		printf("couldnt open inputfile. ft_free_files.\n");
	write(file_fd_in, (int *)EOF, 1);
	close(file_fd_in);

	file_fd_out = open("mull/Output_file.txt", O_RDWR | O_TRUNC, 0777);
	if (file_fd_out < 0)
		printf("couldnt open outputfile. ft_free_files.\n");
	write(file_fd_out, (int *)EOF, 1);
	close(file_fd_out);
}

void ft_free()
{
	t_cmd	*cmd_iterator;
	t_cmd	*temp;
	int		i;
	
	i = 0;
	cmd_iterator = g_mini.cmds;
	ft_free_files();
	while (cmd_iterator)
	{
		ft_free_lst_cont(cmd_iterator->toks);
		free(cmd_iterator->command_path);
		free(cmd_iterator->input_file);
		free(cmd_iterator->output_file);
		free(cmd_iterator->HEREDOC_DELIM);
		while (cmd_iterator->arguments[i])
		{
			free(cmd_iterator->arguments[i]);
			i++;
		}
		free (cmd_iterator->arguments);
		temp = cmd_iterator->next;
		free (cmd_iterator);
		cmd_iterator = temp;
	}
	free (g_mini.cmds);
	ft_free_lst_cont(g_mini.dup_env);
	free (g_mini.raw_input);
	free (&g_mini);
}

void ft_free_lst_cont(t_list *cmd_iterator)
{
	t_list	*temp;

	while (cmd_iterator)
	{
		free (cmd_iterator->content);
		temp = cmd_iterator->next;
		free (cmd_iterator->next);
		free (cmd_iterator);
		cmd_iterator = temp;
	}
}

void exit_program(int status)
{
	printf("exit status:%d\n", status);
	// free everything
	exit(0);
}
