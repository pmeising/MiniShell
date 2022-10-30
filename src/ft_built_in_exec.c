/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_in_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 17:51:01 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/30 18:01:13 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* execution function for builtin pwd;
* prints out the current working directory;
* pwd is obtained by calling getcwd();
*/
void ft_pwd_exec(void)
{
	char *cur_path;

	cur_path = malloc(sizeof(char) * 1024);
	if (!cur_path)
	{
		printf("pwd: malloc error");
		free(cur_path);
		exit(EXIT_FAILURE);
	}
	if (getcwd(cur_path, 1024))
			printf("%s\n", cur_path);
	free(cur_path);
}

/*
* execute function for builtin command env;
* prints out the list of environment variables;
*/
void ft_env_exec(void)
{
	while (g_mini.dup_env)
	{
		printf("%s\n", g_mini.dup_env->content);
		g_mini.dup_env = g_mini.dup_env->next;
	}
}

