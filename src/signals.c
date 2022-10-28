/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 17:45:53 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/28 17:39:45 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	ft_sigint(int sig)
// {
// 	(void)sig;
// 	printf("\n");
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

// void	ft_sigquit(int sig)
// {
// 	(void)sig;
// 	write(1, "exit", 4);
// 	exit_program(EXIT_SUCCESS);
// }

/* ********************************* */

// andi links
// siqquit not a signal?
// ctrl+\ -> need us keyboard nothing happens by default already

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
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
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

// void ft_sigquit(int sig)
// {
// 	(void)sig;

// 	printf("quit\n");
// 	exit_program(EXIT_SUCCESS);
	
	
// }

// void ft_handle_sigquit(void)
// {
// 	struct sigaction	sa;

// 	sa.sa_handler = SIG_IGN;
// 	sa.sa_flags = SA_RESTART;
// 	sigemptyset(&sa.sa_mask);
// 	sigaction(SIGQUIT, &sa, NULL);
// }

void exit_shell(int sig)
{
	printf("quit\n");
	if (SIGQUIT == sig)
	{
		printf("quit\n");
		//exit(EXIT_SUCCESS);
	}
}

void ft_handle_sigs(void)
{
	ft_handle_sigint();
	//ft_handle_sigquit();
}
