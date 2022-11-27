/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 19:22:21 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/25 19:39:38 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_chars(char *cur_cwd, char *new_cwd)
{
	free (cur_cwd);
	free (new_cwd);
}

void	ft_check_malloc(char *str)
{
	if (!str)
	{
		perror("malloc :");
		free (str);
		exit_program(1);
	}
}

void	ft_check_malloc_int(int *fd)
{
	if (!fd)
	{
		perror("malloc :");
		free (fd);
		exit_program(1);
	}
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

void	ft_put_prompt(int sig)
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
