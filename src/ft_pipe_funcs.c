/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:39:14 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 16:40:19 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_close_pipes(int in, int out)
{
	int	i;

	i = 0;
	while (i < g_mini.nbr_of_pipes && g_mini.pipefd[i])
	{
		if (g_mini.pipefd[i][0] != in)
			close(g_mini.pipefd[i][0]);
		if (g_mini.pipefd[i][1] != out)
			close(g_mini.pipefd[i][1]);
		i++;
	}
}

void	ft_close_fds(int in, int out, int nbr)
{
	t_cmd	*iterator;

	(void)nbr;
	iterator = g_mini.cmds;
	while (iterator)
	{
		if (iterator->input_file && iterator->fd_in != in)
			close(iterator->fd_in);
		if (iterator->output_file && iterator->fd_out != out)
			close(iterator->fd_out);
		if (iterator->heredoc_temp)
			unlink(iterator->heredoc_temp);
		iterator = iterator->next;
	}
	if (g_mini.nbr_of_pipes > 0)
		ft_close_pipes(in, out);
}

/*
* initializes the **pipefd in the g_mini struct;
* each *int in **pipefd is initialized with two ints
* that have the default values of 0 at [0] and 1 at [1];
*/
void	ft_init_pipefd(int nbr_of_pipes)
{
	int	i;
	int	*fds;

	i = 0;
	if (g_mini.nbr_of_pipes == 0 || g_mini.nbr_of_pipes < 0 \
			|| g_mini.nbr_of_pipes > 50)
		return ;
	g_mini.pipefd = malloc(sizeof(int *) * nbr_of_pipes + 1);
	if (!g_mini.pipefd)
	{
		perror("malloc: ");
		exit_program(1);
	}
	while (nbr_of_pipes > 0)
	{
		fds = malloc(sizeof(int) * 2);
		ft_check_malloc_int(fds);
		fds[0] = 0;
		fds[1] = 1;
		g_mini.pipefd[i] = fds;
		nbr_of_pipes--;
		i++;
	}
}

/*
* We need to execute the commands simultaneously, 
* so create pipes before forking.
* pipe command stores an int[2] in the given parameter.
* We store it in "int *pipefd[i]";
* counts number of pipes;
* initializes **pipefd in the g_mini struct;
* pipe(): creates the pipe and puts the corresponding
* fd into the **pipefd[i]
* We need in total n_cmds - 1 pipes
*/
void	ft_set_pipes(void)
{
	int	nbr_of_pipes;
	int	pipe_check;
	int	i;

	i = 0;
	pipe_check = 0;
	nbr_of_pipes = ft_lstsize_cmds(g_mini.cmds) - 1;
	g_mini.nbr_of_pipes = nbr_of_pipes;
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
		nbr_of_pipes--;
		i++;
	}
}
