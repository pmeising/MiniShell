/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:26:26 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/14 17:10:46 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
	Checks whether the command is "EXPORT" or not.
	Returns 1 if it is, 0 if it is not.
*/
int ft_is_export(char *str)
{
	char	export[7];
	char	export_1[9];

	if (!str)
		return (0);
		
	export_1[0] = '"';
	export_1[1] = 'e';
	export_1[2] = 'x';
	export_1[3] = 'p';
	export_1[4] = 'o';
	export_1[5] = 'r';
	export_1[6] = 't';
	export_1[7] = '"';
	export_1[8] = '\0';
	export[0] = 'e';
	export[1] = 'x';
	export[2] = 'p';
	export[3] = 'o';
	export[4] = 'r';
	export[5] = 't';
	export[6] = '\0';
	if (ft_strncmp(str, export, 6) == 0)
		return (1);
	else if (ft_strncmp(str, export_1, 8) == 0)
		return (1);
	else
		return (0);
}

/*
	Checks whether the command is "ECHO" or not.
	Returns 1 if it is, 0 if it is not.
*/
int ft_is_echo(char *str)
{
	char	echo[5];
	char	echo_1[7];

	if (!str)
		return (0);
	echo_1[0] = '"';
	echo_1[1] = 'e';
	echo_1[2] = 'c';
	echo_1[3] = 'h';
	echo_1[4] = 'o';
	echo_1[5] = '"';
	echo_1[6] = '\0';
	echo[0] = 'e';
	echo[1] = 'c';
	echo[2] = 'h';
	echo[3] = 'o';
	echo[4] = '\0';
	if (ft_strncmp(str, echo, 5) == 0)
		return (1);
	else if (ft_strncmp(str, echo_1, 7) == 0)
		return (1);
	else
		return (0);
}

/*
	Checks whether the command is "ENV" or not.
	Returns 1 if it is, 0 if it is not.
*/
int ft_is_env(char *cmd_name)
{
	char	env[4];
	char	env_1[6];
	if (!cmd_name)
		return (0);
	env_1[0] = '"';
	env_1[1] = 'e';
	env_1[2] = 'n';
	env_1[3] = 'v';
	env_1[4] = '"';
	env_1[5] = '\0';
	env[0] = 'e';
	env[1] = 'n';
	env[2] = 'v';
	env[3] = '\0';

	if (ft_strncmp(cmd_name, env, 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_name, env_1, 6) == 0)
		return (1);
	else
		return (0);
}
