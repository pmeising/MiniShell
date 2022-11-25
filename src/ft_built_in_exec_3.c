/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:45:26 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/22 17:30:40 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_export_exec_helper_1(t_list *toks)
{
	if (!toks->next)
	{
		ft_print_sorted_env(g_mini.dup_env);
		return (NULL);
	}
	else
		return (toks->next);
}

void	ft_export_exec_helper_2(t_list *iterator, char **env_cont, int i, int j)
{
	while (iterator && iterator->content && iterator->content[i])
	{
		env_cont[0][j] = iterator->content[i];
		i++;
		j++;
	}
	env_cont[0][j] = '\0';
}

int	ft_export_exec_helper_3(t_list *iterator, char **env_name)
{
	int	i;

	i = 0;
	while (iterator && iterator->content[i] && iterator->content[i] != '=')
	{
		env_name[0][i] = iterator->content[i];
		i++;
	}
	env_name[0][i] = '\0';
	i++;
	return (i);
}

void	ft_cd_exec_helper_1(char *content)
{
	int	check;

	check = chdir(content);
	if (check == -1)
		printf("42_shell: cd: %s: No such file or directory\n", \
		content);
}
