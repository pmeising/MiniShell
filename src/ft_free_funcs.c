/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:28:28 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/10 20:00:16 by bde-carv         ###   ########.fr       */
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
// void ft_free_files(void)
// {
// 	int file_fd_in;
// 	int file_fd_out;

// 	file_fd_in = open("mull/Input_file.txt", O_RDWR | O_TRUNC, 0777);
// 	if (file_fd_in < 0)
// 		printf("couldnt open inputfile. ft_free_files.\n");
// 	write(file_fd_in, (int *)EOF, 1);
// 	close(file_fd_in);

// 	file_fd_out = open("mull/Output_file.txt", O_RDWR | O_TRUNC, 0777);
// 	if (file_fd_out < 0)
// 		printf("couldnt open outputfile. ft_free_files.\n");
// 	write(file_fd_out, (int *)EOF, 1);
// 	close(file_fd_out);
// }

void	ft_free_fds(void)
{
	int	i;

	i = 0;
	while (i < g_mini.nbr_of_pipes)
	{
		free(g_mini.pipefd[i]);
		i++;
	}
	free (g_mini.pipefd);
}

void	ft_free_input(void)
{
	t_cmd	*cmd_iterator;
	// int		i;
	t_cmd	*temp;

	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		ft_free_lst_cont(cmd_iterator->toks);
		printf("here: %s, input: %s\n", cmd_iterator->heredoc_temp, cmd_iterator->input_file);
		if (cmd_iterator->heredoc_temp && cmd_iterator->input_file)
		{
			printf("entered if1\n");
			if (ft_strncmp(cmd_iterator->heredoc_temp, cmd_iterator->input_file, ft_strlen(cmd_iterator->heredoc_temp)) == 0)
			{
				printf("entered if2\n");
				printf("unlink: %d\n", unlink(cmd_iterator->heredoc_temp));
			}
		}
		if (cmd_iterator->command_path) 
			free(cmd_iterator->command_path);
		if (cmd_iterator->input_file)
			free(cmd_iterator->input_file);
		if (cmd_iterator->output_file)
			free(cmd_iterator->output_file);
		if (cmd_iterator->HEREDOC_DELIM)
			free(cmd_iterator->HEREDOC_DELIM);
		if (cmd_iterator->arguments)
			free (cmd_iterator->arguments);
		temp = cmd_iterator->next;
		free (cmd_iterator);
		cmd_iterator = temp;
	}
	free (g_mini.raw_input);
	g_mini.cmds = NULL;
	g_mini.nbr_heredocs = 0;
	ft_free_fds();
	if (g_mini.exit_status == 2 || g_mini.exit == 1)
	{
		ft_free_lst_cont(g_mini.dup_env);
		exit(g_mini.exit_status);	
	}
	// unlink?
}
						
void ft_free_lst_cont(t_list *iterator)
{
	t_list	*temp;

	while (iterator)
	{
		// free (iterator->content);
		temp = iterator->next;
		// free (iterator->next);
		free (iterator);
		iterator = temp;
	}
}

void exit_program(int status)
{ 
	// if 2 terminate minishell
	g_mini.exit = 1;
	g_mini.exit_status = status;
	if (status  == 2)
	{
		ft_free_input();
		printf("before exit\n");
		exit (EXIT_FAILURE);
	}
	if (status == 0) // exit success
	{
		printf("exit staus 0\n");
	}
	
	if (status == 130) // crtl+c
	{
		ft_sigint(status);
	}
	
	if (status == 1) // clear shell but continue shell running and give prompt back liek at begining
	{
		ft_free_input();
	}
	
	printf("exit status:%d\n", status);
	// free everything
	//exit(0);
}
