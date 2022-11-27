/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 10:15:38 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/26 11:09:35 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_find_command_3(t_cmd *cmd, t_list *tok, char **paths)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], tok->content);
		if (tok->content[0] == '/' && access(tok->content, F_OK | X_OK) == 0)
			cmd->command_path = tok->content;
		else if (ft_is_program(cmd, tok->content) == 1)
			break ;
		else if (access(temp, F_OK | X_OK) == 0 && cmd->is_built_in == 0)
		{
			cmd->command_path = ft_strdup(temp);
			break ;
		}
		else
			cmd->command_path = NULL;
		ft_free_str(&temp);
		i++;
	}
	if (temp)
		ft_free_str(&temp);
	return (ft_find_command_5(paths, temp, i));
}

int	ft_find_command_5(char **paths, char *temp, int i)
{
	int	j;

	j = 0;
	if (!paths || paths[i] == NULL)
	{
		if (temp)
			free(temp);
		temp = NULL;
		j = 1;
	}
	return (j);
}
