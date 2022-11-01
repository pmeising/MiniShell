/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_funcs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 18:49:28 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/01 14:38:23 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_print_list(t_cmd *cmds)
{
	t_list	*iterator;
	int		i;
	
	i = 1;
	iterator = cmds->toks;
	while (iterator)
	{
		printf("%d. node: %s\n", i, cmds->toks->content);
		i++;
		iterator = iterator->next;
	}
}

void	ft_print_args(t_cmd *iterator)
{
	int	i;

	i = 0;
	if (iterator->command_path != NULL)
	{	
		while (iterator->arguments[i])
		{
			printf("argument %d: %s\n", i, iterator->arguments[i]);
			i++;
		}
	}
	i = 0;
	while (iterator->output_file[i] && (iterator->open_flag[i] == -1 || iterator->open_flag[i] == 0 || iterator->open_flag[i] == 1))
	{
		printf("outputfile:%s\n", iterator->output_file[i]);
		printf("open_flag: %d\n", iterator->open_flag[i]);
		i++;
	}
	// i = 0;
	// while (iterator->open_flag[i] == -1 || iterator->open_flag[i] == 0 || iterator->open_flag[i] == 1)
	// {
	// 	printf("open_flag: %d\n", iterator->open_flag[i]);
	// 	i++;
	// }
}

void	ft_print_cmds(t_cmd *cmd)
{
	t_cmd	*iterator;
	int		i;

	i = 0;
	iterator = cmd;
	while (iterator)
	{
		i++;
		ft_print_list(iterator);
		printf("command_path: %s\n", iterator->command_path);
		ft_print_args(iterator);
		printf("input_file: %s\n", iterator->input_file);
		// printf("output_file: %s\n", iterator->output_file[]);
		printf("HEREDOC_DELIM: %s\n", iterator->HEREDOC_DELIM);
		printf("fd_in: %d\n", iterator->fd_in);
		printf("fd_out: %d\n", iterator->fd_out);
		printf("is_built_in: %d\n", iterator->is_built_in);
		// printf("open_flag: %d\n", iterator->open_flag);
		iterator = iterator->next;
	}
}
