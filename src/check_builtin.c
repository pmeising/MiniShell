/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:27:58 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/03 18:43:06 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_is_pwd(char *str)
{
	char	pwd[4];
	char	pwd_1[6];

	pwd_1[0] = '"';
	pwd_1[1] = 'p';
	pwd_1[2] = 'w';
	pwd_1[3] = 'd';
	pwd_1[4] = '"';
	pwd[0] = 'p';
	pwd[1] = 'w';
	pwd[2] = 'd';
	pwd[3] = '\0';
	if (ft_strncmp(str, pwd, 4) == 0)
		return (1);
	else if (ft_strncmp(str, pwd_1, 6) == 0)
		return (1);
	else
		return (0);
}

int ft_is_unset(char *str)
{
	char	unset[6];
	char	unset_1[8];

	unset_1[0] = '"';
	unset_1[1] = 'u';
	unset_1[2] = 'n';
	unset_1[3] = 's';
	unset_1[4] = 'e';
	unset_1[5] = 't';
	unset_1[6] = '"';
	unset_1[7] = '\0';
	unset[0] = 'u';
	unset[1] = 'n';
	unset[2] = 's';
	unset[3] = 'e';
	unset[4] = 't';
	unset[5] = '\0';
	if (ft_strncmp(str, unset, 6) == 0)
		return (1);
	else if (ft_strncmp(str, unset_1, 8) == 0)
		return (1);
	else
		return (0);
}

int ft_is_exit(char *cmd_name)
{
	char	exit[5];
	char	exit_1[7];

	exit_1[0] = '"';
	exit_1[1] = 'e';
	exit_1[2] = 'x';
	exit_1[3] = 'i';
	exit_1[4] = 't';
	exit_1[5] = '"';
	exit_1[6] = '\0';
	exit[0] = 'e';
	exit[1] = 'x';
	exit[2] = 'i';
	exit[3] = 't';
	exit[4] = '\0';

	if (ft_strncmp(cmd_name, exit, 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_name, exit_1, 6) == 0)
		return (1);
	else
		return (0);

}

int ft_is_cd(char *str)
{
	char	cd[3];
	char	cd_1[5];

	cd_1[0] = '"';
	cd_1[1] = 'c';
	cd_1[2] = 'd';
	cd_1[3] = '"';
	cd_1[4] = '\0';
	cd[0] = 'c';
	cd[1] = 'd';
	cd[2] = '\0';
	if (ft_strncmp(str, cd, 3) == 0)
		return (1);
	else if (ft_strncmp(str, cd_1, 5) == 0)
		return (1);
	else
		return (0);
}

int ft_is_built_in(char *cmd_name)
{
	int check;
	
	check = 0;
	if (ft_is_echo(cmd_name) || ft_is_env(cmd_name) || ft_is_export(cmd_name) || \
		ft_is_exit(cmd_name) || ft_is_pwd(cmd_name) || ft_is_unset(cmd_name) || \
		ft_is_cd(cmd_name))
		check = 1;
	return (check);
}
