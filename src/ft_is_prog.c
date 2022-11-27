/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_prog.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:24:25 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/27 19:34:23 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_prog_help_1(t_cmd *cmd, char *prog_name)
{
	char	*name;
	char	*cwd;

	cwd = malloc(sizeof(char) * 1024);
	name = ft_strdup(&prog_name[1]);
	cmd->command_path = ft_strjoin(getcwd(cwd, 1024), name);
	ft_free_str(&name);
	ft_free_str(&cwd);
	if (access(name, F_OK | X_OK) == 0)
		return (1);
	else
		ft_free_str(&cmd->command_path);
	return (0);
}

void	ft_is_prog_help_2(t_cmd *cmd, char *prog_name, int i)
{
	char	*name;
	char	*cwd;
	int		len;

	name = ft_strdup(prog_name);
	while (name[0] == '.')
	{
		cwd = malloc(sizeof(char) * 1024);
		getcwd(cwd, 1024);
		len = ft_strlen(cwd);
		while (cwd[len] != '/')
			len--;
		cwd[len] = '\0';
		chdir(cwd);
		ft_free_str(&cwd);
		ft_free_str(&name);
		name = ft_strdup(&prog_name[i]);
		i = i + 3;
	}
	ft_free_str(&cwd);
	cwd = malloc(sizeof(char) * 1024);
	cmd->command_path = ft_strjoin(getcwd(cwd, 1024), name);
	ft_free_str(&cwd);
	ft_free_str(&name);
}

void	ft_free_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	str = NULL;
}
