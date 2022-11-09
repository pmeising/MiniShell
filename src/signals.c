/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 17:45:53 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/09 19:16:26 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* handler function for sigint (ctrl+c);
* rl_replace_line("", 0): replaces the contents in readline_buffer
* (prompt and user input) with "";
* rl_on_new_line(): moves to a new line;
* rl_redisplay(): displays whats stored in readline_buffer;
* readline buffer contains our prompt;
*/
void ft_sigint(int sig)
{
	(void)sig;
	//write(1, "\n", 1);
	ft_putstr_fd("\b\b\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();

	//exit_program (130);
}

/*
* sa.sa_handler = functoin to be called when signal is caught;
* sa.sa_flags = if SA_RESTART is set we continue at the calling function
* if not set we will get a function fail;
* sigemptyset: initializes a signal set
* sa_mask: is a signal set containing signals to be blocked during handling
* of the incoming signal;
* sigaction(trigger signal, struct containing infos, previous signal):
* is a ptr to a fct., if the signal comes in then sa_handler
*/
void ft_handle_sigint(void)
{
	struct sigaction sa;

	sa.sa_handler = &ft_sigint;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

/*
* exit programm for ctrl + d (sigquit)
* sigquit/ctrl + d means end of file and has 0 value
* so we check after the readline function if the line
* that was read in is empty. if yes exit_shell_quit
* is being called to terminate the program;
*/
void exit_shell_quit(int sig)
{
	if (sig == 0)
	{
		printf("exit\n");
		exit_program(2);
	}
}
