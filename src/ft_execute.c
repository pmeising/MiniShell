/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:37:51 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/10 19:00:18 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/*
*	Takes a pointer to a command node (t_cmd) and creates a child process.
*	The command node contains information about where to read from
*	and where to write to. These variables are initialized to NULL
*	, 0 or their corresponding "initial state". (i.e. the fd_in variable
*	in the t_cmd node stands for the filedescriptor of the standard
*	input = 0; fd_out for Standard output = 1.) This initialization 
*	happens in ft_lstnew_cmds (file "ft_lst_funcs.c").
*	if the input_file var is not empty, that means we have an input
*	redirect from a file. In which case we need to check, if the file
*	exists (i.e. access -> returns 0 for success). After, we open the file
*	with open() and store the file descriptor it returns in fd_in;
*	(returns -1 on error).
*	Now we use the dup2() function to make a copy of the standard input fd=0
*	and assign it to the file we want to use as an input file. It takes the file
*	descriptor of the dedicated input file and the FD of which to associate it 
*	with. In this case we associate it with STDIN = 0.
*	This happens whether the input_file string is empty or not. In case it
*	isn't empty, we need to close that previously opened file again.
*	The same logic applies to the output file. However, if the output file
*	should not exist, we would simply create it (using the open(O_CREATE) flag.).
*	Additionally, we are using the O_APPEND flag if the open_flag is 1, the O_TRUNC
*	flag, if the open_flag is 0. Then we dup2() again and close any open file.
*	In the last part, we call the execve() function to execute the command
*	currently stored in the t_cmd node.
*	The execve() function terminates a process upon successful execution, 
*	which is why the following lines including perror etc. would only be
*	executed in case of an execve() failure.
*	using the dup2() STDOUT allows us to receive that error message in
*	the standard output rather than any potential output file.
*/
// void	ft_fork_process(t_cmd *iterator)
// {
// 	pid_t	fork_check;
// 	int		file_check;

// 	fork_check = fork();
// 	if (fork_check == 0)
// 	{
// 		if (iterator->input_file != NULL)
// 		{
// 			file_check = access(iterator->input_file, R_OK | F_OK);
// 			if (file_check != 0)
// 			{
// 				printf("bash: %s: No such file or directory\n", iterator->input_file);
// 				exit_program(1);
// 			}
// 			iterator->fd_in = open(iterator->input_file, O_RDWR, 0777);
// 			if (iterator->fd_in == -1)
// 			{
// 				printf("bash: %s: Permission denied\n", iterator->input_file);
// 				exit_program(1);
// 			}
// 			// printf("fd_in in child: %d\n", iterator->fd_in);
// 		}
// 		dup2(iterator->fd_in, STDIN_FILENO);
// 		if (iterator->fd_in != 0)
// 			close(iterator->fd_in); // [0] -> Output_file.txt [1] 
// 		if (iterator->output_file[0] != NULL)
// 		{
// 			// if (iterator->open_flag[] == 0) // Output_file.txt
// 			iterator->fd_out = open(iterator->output_file[0], O_CREAT | O_RDWR | O_TRUNC, 0777);
// 			// else if (iterator->open_flag == 1)
// 			// 	iterator->fd_out = open(iterator->output_file[0], O_CREAT | O_RDWR | O_APPEND, 0777);
// 			// remember to change open_flag to it's value when changing the outputfile content.
// 			if (iterator->fd_out == -1)
// 				exit_program(1); // error message for internal trouble shooting.
// 		}
// 		dup2(iterator->fd_out, STDOUT_FILENO); // fd_out defaults to 1
// 		if (iterator->fd_out != 1)
// 			close(iterator->fd_out);
// 		// printf("KDJFIAEHJLSIEHGIEHJ \n");
// 		// printf("input: %s\n output: %s\n", iterator->input_file, iterator->output_file[0]);
// 		// printf("args[0]: %s\n", iterator->arguments[0]);
// 		if (iterator->is_built_in == 0)
// 		{
// 			execve(iterator->command_path, iterator->arguments, g_mini.env);
// 			dup2(1, STDOUT_FILENO);
// 			printf("EXECVE failure.\n");
// 			perror("execve: ");
// 		}
// 		else if (iterator->is_built_in == 1)
// 		{
// 			ft_execute_built_in(iterator, iterator->toks);
// 			execve(iterator->command_path, iterator->arguments, g_mini.env);
// 			dup2(1, STDOUT_FILENO);
// 			printf("EXECVE failure.\n");
// 			perror("execve: ");
// 		}
// 	}
// 	if (fork_check != 0)
// 	{
// 		// sleep(1);
// 		waitpid(0, NULL, 0);
// 		printf("I am parent.\n");
// 	}
// }

/*
* copies content of file_1 into file_2;
* file_1 and file_2 are outpufiles[0]/[i]
*/
// void	ft_copy_content(char *file_1, char *file_2, int open_flag)
// {
// 	int 	fd_file_1;
// 	int 	fd_file_2;
// 	char	*temp;

// 	fd_file_1 = open(file_1, O_CREAT | O_RDWR, 0777);
// 	if (fd_file_1 == -1)
// 	{
// 		printf("ft_copy_content:error fd_file_1\n");
// 		close(fd_file_1);
// 		exit_program(1);
// 	}
// 	if (open_flag == 0)
// 	{
// 		fd_file_2 = open(file_2, O_CREAT | O_RDWR | O_TRUNC, 0777);
// 		if (fd_file_2 == -1)
// 		{
// 			printf("ft_copy_content:fd_file_2\n");
// 			close(fd_file_2);
// 			exit_program(1);
// 		}
// 	}
// 	else if (open_flag == 1)
// 	{
// 		fd_file_2 = open(file_2, O_CREAT | O_RDWR | O_APPEND, 0777);
// 		if (fd_file_2 == -1)
// 		{
// 			printf("ft_copy_content:error fd_file_2\n");
// 			close(fd_file_2);
// 			exit_program(1);
// 		}
// 	}
// 	while (1)
// 	{
// 		temp = get_next_line(fd_file_1);
// 		if (temp)
// 		{
// 			write(fd_file_2, temp, ft_strlen(temp));
// 			free (temp);
// 		}
// 		else
// 		{
// 			free (temp);
// 			break ;
// 		}
// 	}
// 	close(fd_file_1);
// 	close(fd_file_2);
// }

/*
*	input_file is empty if no input redirection was found.
*	need expl.
*/
// void	ft_redirect(t_cmd *iterator)
// {
// 	int		i;

// 	i = 0;
// 	while (iterator->output_file[i])
// 	{
// 		if (i > 0)
// 		{
// 			printf("iterator->output_file[%d]: %s. Flag: %d\n", i, iterator->output_file[i], iterator->open_flag[i]);
// 			ft_copy_content(iterator->output_file[0], iterator->output_file[i], iterator->open_flag[i]);
// 		}
// 		else if (i == 0 && iterator->next && iterator->next->input_file == NULL)
// 		{
// 			ft_copy_content(iterator->output_file[0], "mull/Input_file.txt", 0);
// 			iterator->next->input_file = "mull/Input_file.txt";
// 		}
// 		else if (i == 0 && iterator->next && iterator->next->input_file != NULL)
// 			ft_copy_content(iterator->output_file[0], iterator->next->input_file, 1);
// 		i++;
// 	}
// 	// printf("it_input: %s\n", iterator->next->input_file);
// 	// if (iterator->output_file[0] == NULL)
// 	// 	ft_copy_content(iterator->output_file[0], "Input_file.txt", 0);
// }

/*
* output_file[1] = name of file where to redirect;
* output_file[0] = has "output_file.txt" as default;
*/
// void	ft_output_file(t_cmd *iterator)
// {
// 	t_cmd	*last;

// 	last = ft_lstlast_cmds(iterator);
// 	if ((ft_strncmp(last->output_file[0], "mull/Output_file.txt", 21) == 0) && last->output_file[1] == NULL)
// 	{
// 		last->output_file[0] = NULL;
// 		last->fd_out = 1;
// 	}
// }

/*
*	Walks through the t_cmd list and calls the ft_fork_process function
*	which creates a child process executing the command in the cmd_path.
*	ft_output_file; checks if there is an outputfile, if not deletes the 
*	default "output_file.txt"
*/
// void	ft_execute(void)
// {
// 	t_cmd	*iterator;

// 	iterator = g_mini.cmds;
// 	ft_output_file(iterator);
// 	while (iterator) //  && (iterator->command_path || iterator->is_built_in == 1)
// 	{
// 		ft_fork_process(iterator);
// 		ft_redirect(iterator);
// 		if (iterator->next == NULL)
// 			break ;
// 		iterator = iterator->next;
// 	}
// }
