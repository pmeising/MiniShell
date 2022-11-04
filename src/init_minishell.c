/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:30:36 by bde-carv          #+#    #+#             */
/*   Updated: 2022/11/04 11:54:28 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* ft_oldpwd: checks if the env.var OLDPWD exist, if yes its content is deleted
* if no a new envvar OLDPWD is created as a node and added to the dup_env list;
* ft_env_exist: returns 0 if env_var does NOT exist;
* ft_del_env_cont: deletes content of an env.var and leaves only its name
*/
void	ft_oldpwd(void)
{
	if (ft_env_exist("OLDPWD", &g_mini.dup_env) == 0)
	{
		ft_lstadd_back(&g_mini.dup_env, ft_lstnew("OLDPWD"));
	}
	else
	{
		ft_del_env_cont("OLDPWD", &g_mini.dup_env);
	}
}

/*
*	replaces the content of an env_var;
*	take the name of an env adds a '=' and then joins with the new_value;
*/
char	*ft_replace_env_cont(char *env_name, char *new_value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(env_name, "=");
	if (!tmp)
	{
		printf("ft_replace_env_cont:strings not joined\n");
		exit_program(1);
	}
	res = ft_strjoin(tmp, new_value);
	if (!res)
	{
		printf("ft_replace_env_cont:strings not joined\n");
		exit_program(1);
	}
	free(tmp);
	// tmp = 0; // is needed for some reason or there will be no value in SHLVL
	// free(*curr_content);// should be freed but gives compiling error ?
	// curr_content = 0; // is needed for some reason or there will be no value in SHLVL
	return(res);
}

/*
*	updates the content of a node/env_var in our dup_env list;
*/
void	ft_update_env_list(char *env_name, char *new_value, t_list *dup_env)
{
	char	*curr_content;
	int		len;

	len = ft_strlen(env_name);
	while (dup_env)
	{
		curr_content = (char *)dup_env->content;
		if (ft_strncmp(env_name, curr_content, len) == 0)
			dup_env->content = ft_replace_env_cont(env_name, new_value);
		dup_env = dup_env->next;
	}
}

/*
* updates the SHLVL for every ./minishell call +1;
* checks if SHLVL env.var exists, if not creates one
* if existent it reads out the SHLVL content, converts it
* to int, increases it by 1, converts back to char* and 
* puts it back into the env list;
*/
void	ft_raise_shlvl(void)
{
	char	*cur_lvl;
	int		int_cur_lvl;

	if (ft_env_exist("SHLVL", &g_mini.dup_env) == 0)
		ft_create_env("SHLVL=1", &g_mini.dup_env);
	else
	{
		cur_lvl = ft_get_env_cont("SHLVL");
		int_cur_lvl = ft_atoi(cur_lvl);
		int_cur_lvl++;
		free(cur_lvl);
		cur_lvl = ft_itoa(int_cur_lvl);
		if (!cur_lvl)
		{
			printf("ft_raise_shlvl: raise shlvl\n");
			exit_program(1);
		}
		ft_update_env_list("SHLVL", cur_lvl, g_mini.dup_env);
		free(cur_lvl);
	}
}

/*
* ft_bzero: initializes g_mini struct with 0's
* ft_copy_env: copy environment variables into list, for later use in export;
* ft_oldpwd: checks if the env.var OLDPWD exist, if yes its content is deleted
* if no a new envvar OLDPWD is created as a node and added to the dup_env list;
*/
void	ft_init_minishell(t_mini *g_mini, char **env)
{
	g_mini->env = env;
	g_mini->fdin = 0;
	g_mini->fdout = 1;
	g_mini->exit_status = 0;
	ft_bzero(g_mini, sizeof(t_mini));
	ft_copy_env(g_mini, env);
	ft_oldpwd();
	ft_raise_shlvl();
}
