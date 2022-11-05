/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:28:28 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/05 19:02:37 by pmeising         ###   ########.fr       */
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

void	ft_free_env_vars(void)
{
	t_list	*iterator;
	int		i;
	size_t		len;
	char	**env;

	i = 0;
	env = g_mini.env;
	iterator = g_mini.dup_env;
	while(iterator)
	{
		printf("g_mini. %s\n", g_mini.env[i]);
		len = ft_strlen(g_mini.env[i]) - ft_strlen(ft_strchr((const char *)g_mini.env[i], '='));
		while (env[i] && ft_strncmp(iterator->content, env[i], len) != 0)
			i++;
		if (ft_strncmp(iterator->content, env[i], len) == 0)
		{
			i = 0;
			iterator = iterator->next;
		}
		else
		{
			free(iterator->content);
			i = 0;
			iterator = iterator->next;
		}
	}
}

// HOME						PATH
// PATH 						USER
// USER						HOME
// testhihgsdklfje				NULL
							
void ft_free()
{
	t_cmd	*cmd_iterator;
	t_cmd	*temp;
	int		i;
	
	i = 0;
	cmd_iterator = g_mini.cmds;
	// ft_free_files();
	ft_free_env_vars();
	ft_free_lst_cont(g_mini.dup_env);
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
	free (g_mini.raw_input);
	// free (g_mini);
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
	g_mini.exit_status = status;
	if (status  == 2)
	{
		ft_free();
		exit (EXIT_FAILURE);
	}
	if (status == 0) // exit success
	
	if (status == 130) // crtl+c
	
	if (status == 1) // clear shell but continue shell running and give prompt back liek at begining
	{
		ft_free();
	}
	
	printf("exit status:%d\n", status);
	// free everything
	exit(0);
}
