/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:37:44 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/25 17:01:57 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_cd_exec_helper_1(char *content)
{
	int	check;

	check = chdir(content);
	if (check == -1)
		printf("42_shell: cd: %s: No such file or directory\n", \
		content);
}

void	ft_cd_empty(void)
{
	int		check;
	char	*cwd;

	cwd = malloc(sizeof(char) * 2048);
	if (!cwd)
	{
		printf("ft_cd_empty: malloc error\n");
		exit(EXIT_FAILURE);
	}
	cwd = getcwd(cwd, 1024);
	ft_update_env_list("OLDPWD", cwd, g_mini.dup_env);
	free(cwd);
	check = chdir(getenv("HOME"));
	if (check < 0)
		perror("chdir: ");
	cwd = malloc(sizeof(char) * 1024);
	if (!cwd)
	{
		printf("ft_cd_empty: malloc error\n");
		exit(EXIT_FAILURE);
	}
	cwd = getcwd(cwd, 1024);
	ft_update_env_list("PWD", cwd, g_mini.dup_env);
	free(cwd);
}

/*
	built in execution function for "cd ..";
*/
void	ft_cd_two_dots(void)
{
	char	*cur_cwd;
	char	*new_cwd;
	int		len;
	int		j;

	j = 0;
	cur_cwd = malloc(sizeof(char) * 2048);
	ft_check_malloc(cur_cwd);
	cur_cwd = getcwd(cur_cwd, 1024);
	ft_update_env_list("OLDPWD", cur_cwd, g_mini.dup_env);
	len = ft_strlen(cur_cwd);
	while (cur_cwd[len] != '/')
		len--;
	cur_cwd[len] = '\0';
	new_cwd = malloc(sizeof(char) * len + 1);
	ft_check_malloc(new_cwd);
	while (cur_cwd[j])
	{
		new_cwd[j] = cur_cwd[j];
		j++;
	}
	new_cwd[j] = '\0';
	chdir(new_cwd);
	ft_update_env_list("PWD", new_cwd, g_mini.dup_env);
	ft_free_chars(cur_cwd, new_cwd);
}

void	ft_cd_exec(t_list *toks)
{
	char	*cwd;
	int		i;

	i = 0;
	cwd = malloc(sizeof(char) * 1024);
	ft_check_malloc(cwd);
	cwd = getcwd(cwd, 1024);
	ft_check_malloc(cwd);
	ft_update_env_list("OLDPWD", cwd, g_mini.dup_env);
	free (cwd);
	if (!toks->next)
		ft_cd_empty();
	else if (toks->next->content[0] == '.' && toks->next->content[1] == '.')
	{
		while (i < (int)ft_strlen(toks->next->content) && \
			toks->next->content[i] == '.' && toks->next->content[i + 1] == '.')
		{
			ft_cd_two_dots();
			i = i + 3;
		}
	}
	else if (toks->next)
		ft_cd_exec_helper_1(toks->next->content);
}
