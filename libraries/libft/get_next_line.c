/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 18:21:05 by aelkheta          #+#    #+#             */
/*   Updated: 2023/12/08 17:14:14 by aelkheta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static char	*s;
	char		*line_next;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	s = ft_read_creat(fd, s);
	if (!s)
		return (NULL);
	line_next = ft_extract_line(s);
	s = ft_return_s(s);
	return (line_next);
}

char	*ft_read_creat(int fd, char *s)
{
	char	*buf;
	ssize_t	read_char;

	read_char = 1;
	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	while (!ft_strchr(s, '\n') && read_char)
	{
		read_char = read(fd, buf, BUFFER_SIZE);
		if (read_char == -1)
		{
			free (buf);
			free (s);
			return (NULL);
		}
		buf[read_char] = '\0';
		s = ft_strjoin(s, buf);
	}
	free (buf);
	return (s);
}

char	*ft_extract_line(char *s)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!ft_strlen(s))
		return (NULL);
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		new_line = malloc(sizeof(char) * (i + 2));
	else
		new_line = malloc(sizeof(char) * (i + 1));
	if (!new_line)
		return (NULL);
	while (j <= i && s[j])
	{	
		new_line[j] = s[j];
		j++;
	}
	new_line[j] = '\0';
	return (new_line);
}

char	*ft_return_s(char *s)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (!s[i])
	{
		free(s);
		return (NULL);
	}
	i++;
	new_str = malloc(sizeof(char) * (ft_strlen(s) - i + 1));
	if (!new_str)
	{
		free (s);
		return (NULL);
	}
	while (s[i])
		new_str[j++] = s[i++];
	new_str[j] = '\0';
	free (s);
	return (new_str);
}


// char	*get_the_rest(char *buffer)
// {
// 	int		i;
// 	int		j;
// 	char	*str;

// 	i = 0;
// 	while (buffer[i] && buffer[i] != '\n')
// 		i++;
// 	if (!buffer[i])
// 		return (ft_free(buffer));
// 	str = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
// 	if (!str)
// 		return (ft_free(buffer));
// 	i++;
// 	j = 0;
// 	while (buffer[i])
// 		str[j++] = buffer[i++];
// 	str[j] = '\0';
// 	free(buffer);
// 	return (str);
// }

// char	*check_next_line(char *buffer)
// {
// 	int		i;
// 	int		len;
// 	char	*i_am_the_line;

// 	i = 0;
// 	len = 0;
// 	if (!buffer)
// 		return (NULL);
// 	while (buffer[len] && buffer[len] != '\n')
// 		len++;
// 	i_am_the_line = (char *)malloc(sizeof(char) * len + 2);
// 	if (!i_am_the_line)
// 		return (NULL);
// 	while (buffer[i] && buffer[i] != '\n')
// 	{
// 		i_am_the_line[i] = buffer[i];
// 		i++;
// 	}
// 	if (buffer[i] == '\n')
// 		i_am_the_line[i++] = '\n';
// 	i_am_the_line[i] = '\0';
// 	return (i_am_the_line);
// }

// char	*reach_the_line(int fd, char *buffer)
// {
// 	char	*line;
// 	int		bytes_read;

// 	line = malloc(sizeof(char) * BUFFER_SIZE + 1);
// 	if (!line)
// 		return (NULL);
// 	bytes_read = 1;
// 	while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
// 	{
// 		bytes_read = read(fd, line, BUFFER_SIZE);
// 		if (bytes_read < 0)
// 		{
// 			free(line);
// 			free(buffer);
// 			return (NULL);
// 		}
// 		line[bytes_read] = '\0';
// 		buffer = ft_strjoin(buffer, line);
// 		if (!buffer)
// 			return (NULL);
// 	}
// 	free(line);
// 	return (buffer);
// }

// char	*get_next_line(int fd)
// {
// 	char	*line;
// 	int		bytes_read;

// 	if (fd < 0 || BUFFER_SIZE < 1 || BUFFER_SIZE > INT_MAX)
// 		return (NULL);
// 	line = malloc(BUFFER_SIZE * sizeof(char));
// 	if (!line)
// 		return (NULL);
// 	bytes_read = read(fd, line, BUFFER_SIZE);
// 	if (bytes_read < 0)
// 		return (NULL);
// 	line[bytes_read] = '\0';
// 	return (line);
// }
