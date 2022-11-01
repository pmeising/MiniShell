/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:03:38 by pmeising          #+#    #+#             */
/*   Updated: 2022/11/01 16:07:46 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* sorts the env variables based on ascii value;
* based on sortlist function from ex02;
*/
void	ft_print_sorted_env(t_list *dup_env)
{

	t_list	*iterator;
	char	*temp_cont;

	iterator = dup_env;
	while (iterator && iterator->next)
	{
		if (strcmp(iterator->content, iterator->next->content) > 0 /*&& iterator == *dup_env*/)
		{
			temp_cont = iterator->content;
			iterator->content = iterator->next->content;
			iterator->next->content = temp_cont;
			iterator = dup_env;
		}
		else
			iterator = iterator->next;
	}
	iterator = dup_env;
	while (iterator)
	{
		printf("%s\n", iterator->content);
		iterator = iterator->next;
	}
}

void	ft_export_exec(t_list *toks)
{
	if (!toks->next)
		ft_print_sorted_env(g_mini.dup_env);
	else
	{
		if (ft_isalpha(toks->next->content[0]) == 0)
			printf("bash: export: `%s´: not a valid identifier\n", toks->next->content);
		else
			ft_lstadd_back(&g_mini.dup_env, ft_lstnew(toks->next->content));
	}
}