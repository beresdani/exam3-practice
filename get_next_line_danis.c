/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdanis <gdanis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 10:26:00 by gdanis            #+#    #+#             */
/*   Updated: 2023/11/27 19:54:58 by gabriel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "get_next_line.h"

/*
void	clear_buffer(char *buffer)
{
	int	i;

	i = 0;
	while (i < BUFFER_SIZE)
	{
		buffer[i] = '\0';
		i++;
	}
}
*/

void	set_zero(char **str)
{
	if (*str)
		*str = NULL;
}

int	str_join(char **str, int len, char *buffer, int *start_str)
{
	int	i;
	int	j;
	char	*tmp;

	i = 0;
	if (*str)
	{
		while ((*str)[i])
			i++;
	}
	tmp = (char *)malloc(sizeof(char) * (len + i + 1));
	if (!tmp)
	{
		if (*str)
		{
			free(*str);
			set_zero(str);
		}
		return (0);
	}
	i = 0;
	if (*str)
	{
		while ((*str)[i])
		{
			tmp[i] = (*str)[i];
			i++;
		}
	}
	j = 0;
	while (j < len)
	{
		tmp[i] = buffer[j + *start_str];
		i++;
		j++;
	}
	tmp[i] = '\0';
	if (*str)
	{
		free(*str);
		set_zero(str);
	}
	*str = tmp;
	*start_str += j;
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	static char	*str;
	static int	start_str;
	static int	rread;
	char		*final_str;
	int		i;

	while (1)
	{
		i = 0;
		if (start_str == 0)
			rread = read(fd, buffer, BUFFER_SIZE);
		if (rread < 0)
		{
			if (str)
			{
				free(str);
				set_zero(&str);
			}
			return (NULL);
		}
		if (rread == 0)
		{
			final_str = str;
			if (str)
				set_zero(&str);
			return (final_str);
		}
		while ((i + start_str) < rread && buffer[i + start_str] != '\n')
			i++;
		if (i + start_str == rread)
		{
			if (!str_join(&str, i, buffer, &start_str))
				return (NULL);
			start_str = 0;
		}
		else if (buffer[i + start_str] == '\n' && start_str + i + 1 == rread)
		{
			if (!str_join(&str, i + 1, buffer, &start_str))
				return (NULL);
			final_str = str;
			set_zero(&str);
			start_str = 0;
			return (final_str);
		}
		else
		{
			if (!str_join(&str, i + 1, buffer, &start_str))
				return (NULL);
			final_str = str;
			set_zero(&str);
			return (final_str);
		}
	}
}
/*
int	main(void)
{
	int	fd;
	int	i;
	char	*str;

	fd = open("41_no_nl", O_RDONLY);
	str = get_next_line(fd);
	i = 1;
	while (str)
	{
		printf("\n === calling getnext %d times ===\n", i);
		printf("%s", str);
		free(str);
		str = get_next_line(fd);
		i++;
	}
	close(fd);
	return (0);
}
*/
