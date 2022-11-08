/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:39:14 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/08 17:21:58 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_close_fds(int in, int out, int nbr) //in = 0, out = 1, nbr = 0
{
	int 	i;
	t_cmd	*iterator;

	(void)nbr;
	i = 0;
	iterator = g_mini.cmds;
	printf("hi there. :) \n");
	while (iterator) // close open files that are not used.
	{
		if (iterator->input_file && iterator->fd_in != in)
		{
			printf("Closing fd: %d\n", iterator->fd_in);
			close(iterator->fd_in);
		}
		if (iterator->output_file && iterator->fd_out != out)
		{
			printf("Closing fd: %d\n", iterator->fd_out);
			close(iterator->fd_out);
		}
		iterator = iterator->next;
	}
	printf("After opened files.\n");
	if (g_mini.nbr_of_pipes > 0)
	{
		while (i < g_mini.nbr_of_pipes && g_mini.pipefd[i])
		{
			if (g_mini.pipefd[i][0] != in)
			{
				printf("Closing fd: %d\n", g_mini.pipefd[i][0]);
				close(g_mini.pipefd[i][0]);
			}
			if (g_mini.pipefd[i][1] != out)
			{
				printf("Closing fd: %d\n", g_mini.pipefd[i][1]);
				close(g_mini.pipefd[i][1]);
			}
			i++;
		}
	}
	printf("After pipe fds closed.\n");
	// if (nbr == 0 && iterator->input_file != NULL)
	// {
	// 	printf("Closing fd: %d\n", STDIN_FILENO);
	// 	close(STDIN_FILENO);
	// }
	// if (nbr == g_mini.nbr_of_pipes && iterator->output_file != NULL)
	// {
	// 	printf("Closing fd: %d\n", STDOUT_FILENO);
	// 	close(STDOUT_FILENO);
	// }
	printf("leaving ft_close_fds.\n");
}

/*
* i = cmd counter;
*/
void	ft_execute_process(t_cmd *cmd_iterator, int i)
{
	printf("Before input redirection.\n");
	// INPUT FILES REDIRECT
	if (cmd_iterator->input_file != NULL) // 3 scenarios for input reads. if file read from file.
		dup2(cmd_iterator->fd_in, STDIN_FILENO);
	else if (cmd_iterator->input_file == NULL && i > 0) // if NULL && second+ command take from pipe.
		dup2(g_mini.pipefd[i - 1][0], STDIN_FILENO);
	// else if (cmd_iterator->input_file == NULL && i == 0) // else read from STDIN
	// 	dup2(cmd_iterator->fd_in, STDIN_FILENO);
	printf("before output redir\n");
	// OUTPUT FILES REDIRECT
	printf("it_out:%s output fd: %d\n", cmd_iterator->output_file, cmd_iterator->fd_out);
	if (cmd_iterator->output_file != NULL)
		dup2(cmd_iterator->fd_out, STDOUT_FILENO);
	else if (cmd_iterator->output_file == NULL && i < g_mini.nbr_of_pipes)
		dup2(g_mini.pipefd[i][1], STDOUT_FILENO);
	// else if (cmd_iterator->output_file == NULL && i == g_mini.nbr_of_pipes)
	// 	dup2(cmd_iterator->fd_out, STDOUT_FILENO);
	printf("i == %d. before close fds.\n", i);
	ft_close_fds(cmd_iterator->fd_in, cmd_iterator->fd_out, i);
	printf("before execve is built in.\n");
	if (cmd_iterator->is_built_in == 1)
	{
		ft_execute_built_in(cmd_iterator, cmd_iterator->toks);
		if (ft_is_exit(cmd_iterator->toks->content) == 1)
		{
			printf("exit found.\n");
			exit(130);
			printf("SHouldn't be here.\n");
		}
		exit(0);
	}
	printf("before execve\n");
	execve(cmd_iterator->command_path, cmd_iterator->arguments, g_mini.env);
	dup2(1, STDOUT_FILENO);
	printf("EXECVE failure.\n");
	perror("execve: ");
}

/*
* initializes the **pipefd in the g_mini struct;
* each *int in **pipefd is initialized with two ints
* that have the default values of 0 at [0] and 1 at [1];
* 
*/
void	ft_init_pipefd(int nbr_of_pipes)
{
	int	i;
	int	*fds;

	i = 0;
	printf("init_pipefd\n");
	g_mini.pipefd = malloc(sizeof(int *) * nbr_of_pipes + 1);
	if (!g_mini.pipefd)
	{
		printf("ft_init_pipe:malloc failed.\n");
		exit_program(1);
	}
	while (nbr_of_pipes > 0)
	{
		fds = malloc(sizeof(int) * 2);
		if (!fds)
		{
			printf("ft_init_pipe:malloc failed.\n");
			exit_program(1);
		}
		fds[0] = 0;
		fds[1] = 1;
		printf("fds[0:1]: %d, %d\n", fds[0], fds[1]);
		g_mini.pipefd[i] = fds;
		printf("pipefd[%d][0]/[%d][1]: %d/%d\n", i, i, g_mini.pipefd[i][0], g_mini.pipefd[i][1]);
		nbr_of_pipes--;
		i++;
	}
}

/*
* We need to execute the commands simultaneously, so create pipes before forking.
* pipe command stores an int[2] in the given parameter. We store it in "int *pipefd[i]";

* counts number of pipes;
* initializes **pipefd in the g_mini struct;
* pipe(): creates the pipe and puts the corresponding
* fd into the **pipefd[i]
*/
void	ft_set_pipes(void)
{
	int	nbr_of_pipes;
	int	pipe_check;
	int	i;

	i = 0;
	pipe_check = 0;
	nbr_of_pipes = ft_lstsize_cmds(g_mini.cmds) - 1; // We need in total n_cmds - 1 pipes
	g_mini.nbr_of_pipes = nbr_of_pipes;
	printf("nbr of pipes: %d\n", nbr_of_pipes);
	ft_init_pipefd(nbr_of_pipes);
	while (nbr_of_pipes > 0)
	{
		pipe_check = pipe(g_mini.pipefd[i]);
		if (pipe_check == -1)
		{
			printf("Pipe couldn't be opened.\n");
			perror("pipe: ");
			exit_program(1);
		}
		printf("pipefd: %d\n", g_mini.pipefd[i][0]); // printing reading end of pipe.
		printf("pipefd: %d\n", g_mini.pipefd[i][1]); // printing writing end of pipe.
		nbr_of_pipes--;
		i++;
	}
}
