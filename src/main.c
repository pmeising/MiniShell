/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:34:58 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/16 18:03:443 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_mini	g_mini;

int	ft_is_special_built(char *arguments)
{
	if (ft_is_export(arguments) == 1 || ft_is_cd(arguments) == 1 || \
		ft_is_unset(arguments) == 1)
		return (1);
	return (0);
}

/*
*	creates individual processes for each command;
*	1 == true;
*/
void	ft_test(void)
{
	int		i;
	int		*pid;
	int		status;
	t_cmd	*cmd_iterator;

	i = 0;
	status = 0;
	pid = ft_calloc(sizeof(int), g_mini.nbr_of_pipes + 1);
	if (!pid)
	{
		printf("ft_test: calloc error\n");
		exit_program(1);
	}
	cmd_iterator = g_mini.cmds;
	while (cmd_iterator)
	{
		if (cmd_iterator->toks && cmd_iterator->toks->content && \
			(ft_is_export(cmd_iterator->toks->content) == 1 || \
			ft_is_cd(cmd_iterator->toks->content) == 1 || \
			ft_is_unset(cmd_iterator->toks->content) == 1) && \
			g_mini.nbr_of_pipes == 0)
		{
			ft_execute_built_in(cmd_iterator, cmd_iterator->toks);
			ft_overwrite_env();
			g_mini.special_flag++;
		}
		else
		{
			pid[i] = fork();
			if (pid[i] == -1)
			{
				printf("Forking failed.\n");
				perror("Forking: ");
				exit_program(1);
			}
			if (pid[i] == 0)
			{
				ft_execute_process(cmd_iterator, i);
			}
		}
		i++;
		cmd_iterator = cmd_iterator->next;
	}
	i = 0;
	ft_close_fds(-1, -1, -1);
	while (i < g_mini.nbr_of_pipes + 1)
	{
		(waitpid(pid[i], &status, 0));
		if (status == 6)
			g_mini.exit_status = 0;
		else
		{
			if (WIFSIGNALED(status))
				g_mini.exit_status = 130;
			else
				g_mini.exit_status = status / 256;
		}
		i++;
	}
	free (pid);
	if (g_mini.exit_status != 0 && g_mini.exit == 1)
	{
		ft_free_fds();
		exit(g_mini.exit_status);
	}
}

void	ft_execute(void)
{
	ft_set_pipes();
	ft_set_files();
	if (g_mini.exit != 7)
		ft_test();
}

/*
* displays a prompt and reads in user input;
* adds input to the history for later acces by user;
* checks for open quotes, backslash and semicolon which 
* are all not allowed in this project
*/
int	ft_get_input(void)
{
	char	*prompt;
	char	*temp;

	prompt = "42shell > ";
	temp = readline(prompt);
	if (temp)
		g_mini.raw_input = ft_strdup(temp);
	if (temp == NULL)
	{
		ft_free_lst_cont(g_mini.dup_env);
		if (g_mini.raw_input)
			free (g_mini.raw_input);
		g_mini.raw_input = NULL;
		exit(EXIT_SUCCESS);
	}
	free (temp);
	temp = NULL; // 22.11
	if (ft_str_only_space(g_mini.raw_input) != 1 && g_mini.raw_input)
		add_history(g_mini.raw_input);
	// if (ft_check_quotes(g_mini.raw_input) == 1)
	// {
	// 	printf("quotes not closed\n");
	// 	free(g_mini.raw_input);
	// 	exit_program(1);
	// }
	// if (ft_check_backslash(g_mini.raw_input) == 1)
	// {
	// 	printf("backslash forbidden\n");
	// 	free(g_mini.raw_input);
	// 	exit_program(1);
	// }
	// if (ft_check_semicolon(g_mini.raw_input) == 1)
	// {
	// 	printf("semicolon forbidden\n");
	// 	free(g_mini.raw_input);
	// 	exit_program(1);
	// }
	return (0);
}

/*
* deletes empty quotes like "" or '' out of the input string;
*/
void	ft_del_sus_quotes(void)
{
	char	*raw_input;
	int		i;
	int		j;

	i = 0;
	raw_input = g_mini.raw_input;
	while (raw_input[i])
	{
		if ((raw_input[i] == 34 && raw_input[i + 1] == 34) || \
			(raw_input[i] == 39 && raw_input[i + 1] == 39))
		{
			j = i + 1;
			raw_input[j] = 3;
			while (raw_input[j])
			{
				raw_input[i] = raw_input[j];
				i++;
				j++;
			}
			raw_input[i] = '\0';
			i = 0;
		}
		i++;
	}
}

void ft_put_prompt(int sig)
{
	if (SIGINT == sig)
	{
		g_mini.exit_status = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

/*
* When launching the shell, we don't launch it with any input()
* so argv should be empty. If it isn't we need to return an error.
*/
int	main(int argc, char **argv, char **env)
{
	int	input_check;

	(void)argv;
	(void)argc;
	if (argc != 1)
	{
		printf("too many commands only ./minishell\n");
		exit_program(EXIT_FAILURE);
	}
	ft_init_minishell(&g_mini, env);
	while (1)
	{
		//ft_handle_sigint();
		signal(SIGINT, ft_put_prompt);
		signal(SIGQUIT, SIG_IGN);
		input_check = ft_get_input();
		if (input_check == 2)
			printf("invalid syntax\n");
		else
		{
			if (ft_check_input_validity(g_mini.raw_input) == 1)
			{
				ft_del_sus_quotes();
				ft_dollar_sign();
				ft_parsing(g_mini.raw_input);
				if (g_mini.exit == 0)
					ft_execute();
			}
			ft_free_input();
		}
	}
	return (0);
}
