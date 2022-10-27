/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-carv <bde-carv@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 18:16:46 by bde-carv          #+#    #+#             */
/*   Updated: 2022/10/27 18:03:02 by bde-carv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*output(char *line_a)
{
	int		i;
	char	*str;

	i = 0;
	if (line_a[i] == '\0')
		return (NULL);
	while (line_a[i] != '\0' && line_a[i] != '\n')
		i++;
	str = malloc((i + 2) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (line_a[i] != '\0' && line_a[i] != '\n')
	{
		str[i] = line_a[i];
		i++;
	}
	if (line_a[i] == '\n')
	{
		str[i] = line_a[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*new_line(char *line_a)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (line_a[i] != '\0' && line_a[i] != '\n')
		i++;
	if (line_a[i] == '\0')
	{
		free(line_a);
		return (NULL);
	}
	str = malloc((ft_strlen_gnl(line_a) - i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i++;
	j = 0;
	while (line_a[i] != '\0')
	{
		str[j++] = line_a[i++];
	}
	str[j] = '\0';
	free(line_a);
	return (str);
}

char	*read_line(int fd, char *line_a)
{
	char	*buffer;
	int		i;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
	{
		return (NULL);
	}
	i = 1;
	while (!ft_strchr_gnl(line_a, '\n') && i != 0)
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[i] = '\0';
		line_a = ft_strjoin_gnl(line_a, buffer);
	}
	free(buffer);
	return (line_a);
}

char	*get_next_line(int fd)
{
	static char	*line_a;
	char		*line_b;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		return (0);
	}
	line_a = read_line(fd, line_a);
	if (!line_a)
	{
		return (NULL);
	}
	line_b = output(line_a);
	line_a = new_line(line_a);
	return (line_b);
}

// #include <fcntl.h>
// #include <stdio.h>
// int main ()
// {
// 	int fd;
// 	char *line;
	
// 	fd = open("/Users/bde-carv/Core/safezone/gnl/text.txt", O_RDONLY);
	
// 	if (fd == -1)
// 	{
// 		printf("opening file not possible\n");
// 	}
// line = get_next_line(fd);
// printf("text is:%s\n", line);
// 	while (1) // ?
// 	{
// 		line = get_next_line(fd);
// 		printf("text is:%s\n", line);
		
// 		if (line == NULL)
// 		{
// 			break;
// 		}
// 		free(line);
// 	}
// 	close(fd);
// 	return(0);
// 	//printf("text is:%s\n", line_a);
// 	// cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
// }