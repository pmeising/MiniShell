/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 19:03:32 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/29 19:56:222 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_is_space(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || \
		c == '\r')
		return (1);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	int	i;
	int	strlen;

	i = 0;
	strlen = 0;
	while (str[i] != '\0')
	{
		strlen++;
		i++;
	}
	return (strlen);
}

void ft_cd_empty(void)
{
	chdir(getenv("HOME"));
}
// changed the argv numbers
void  ft_cd(char **argv)
{
	char	*curr_path;
	char	*buffer;
	int		len_curr;
	//int		error;
	
	buffer = malloc(sizeof(char) * 100);
	curr_path = malloc(sizeof(char) * 100);
	curr_path = getcwd(curr_path, 1024);
	// printf("%s\n", argv[1]);
	// printf("%s\n", argv[2] );
	if (argv[2][0] == '.' && argv[2][1] != '.')
		write(1, "\n", 1);
	else if (argv[2][0] == '.' && argv[2][1] == '.' && (ft_is_space(argv[1][2]) == 1 || argv[1][2] == '\0'))
	{
		len_curr = ft_strlen(curr_path);
		//printf("len: %d\n", len_curr);
		while (curr_path[len_curr] != '/'/* && len_curr > 0*/)
		{
			curr_path[len_curr] = '\0';
			len_curr--;
		}
		printf("%s\n", curr_path);
		/*error = */ chdir(curr_path);
		// printf("%d\n", error);
	}
	else
	{
		if (chdir(argv[1]) == -1)
			printf("minishell: %s: path doesnt exist\n", argv[1]);
	}
	free (buffer);
	free (curr_path);
}

int	main(int argc, char **argv)
{
	if (argc == 1)
		ft_cd_empty();
	else if (argc == 3)
		ft_cd(argv);
	else if (argc > 3)
		printf("minishell : cd: too many arguments\n");
}
