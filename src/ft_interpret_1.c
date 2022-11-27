/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:08:16 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/26 10:43:06 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
*	Here we find the correct variable from the "env" command and store it in a 
*	single string.
*/
char	*ft_find_path(void)
{
	char	*str;
	t_list	*env_iterator;

	env_iterator = g_mini.dup_env;
	while (env_iterator && env_iterator->content)
	{
		str = ft_strnstr(env_iterator->content, "PATH=", 5);
		if (str != NULL)
			break ;
		env_iterator = env_iterator->next;
	}
	if (!env_iterator)
		return (NULL);
	return (env_iterator->content);
}

/*
*	In this function we split the string into several directories by the de-
*	limiter ":". This is then stored in a 2D array of strings.
*/
char	**ft_find_paths(char *path)
{
	char	**paths;
	char	*path_temp;
	int		i;

	i = 0;
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	while (paths[i] != NULL)
	{
		path_temp = ft_strjoin(paths[i], "/");
		if (paths[i])
		{
			free(paths[i]);
			paths[i] = NULL;
		}
		paths[i] = path_temp;
		i++;
	}
	return (paths);
}

void	ft_cut_path(t_list *toks)
{
	int		len;
	char	*temp;

	len = ft_strlen(toks->content);
	len--;
	while (toks->content[len] && toks->content[len] != '/' && len > 0)
		len--;
	temp = ft_strdup(&toks->content[len + 1]);
	free (toks->content);
	toks->content = temp;
}

int	ft_find_command_1(t_cmd *cmd, t_list *tok, char **paths)
{
	if (ft_is_built_in(tok->content) == 0)
	{
		cmd->command_path = tok->content;
		return (1);
	}
	else if (ft_is_built_in(tok->content) == 1)
	{
		ft_remove_quotes(tok->content);
		cmd->is_built_in = 1;
		if (access(tok->content, F_OK | X_OK) == 0)
		{
			ft_free_path_at_i(paths);
			cmd->command_path = tok->content;
		}
		return (0);
	}
	return (1);
}

int	ft_find_command_2(t_cmd *cmd, t_list *tok, char **paths)
{
	ft_free_path_at_i(paths);
	cmd->command_path = tok->content;
	return (0);
}
