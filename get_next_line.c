/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sqiu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:34:22 by sqiu              #+#    #+#             */
/*   Updated: 2022/10/28 18:32:19 by sqiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

/*
A function that returns a line read from a file descriptor.
Repeated calls (e.g., using a loop) to get_next_line() function reads the text 
file pointed to by the file descriptor, one line at a time. Reads files and from
standard input (keyboard, fd = 0).
Able to read from a different fd per call without losing the reading thread
of each file descriptor or returning a line from another fd.
E.g. able to call get_next_line() to read from fd 3, then
fd 4, then 5, then once again 3, once again 4, and so forth.

Return value:
Read line: correct behavior
NULL: there is nothing else to read, or an error
occurred
Returned line includes the terminating \n character,
except if the end_index of file was reached and does not end_index with a \n character.
*/

int	ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if (c == 0)
		return (i);
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = malloc (len1 + len2 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	return (str);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*str;

	if (!dest && !src)
		return (NULL);
	str = (unsigned char *)dest;
	while (n--)
		*str++ = *((unsigned char *) src++);
	return (dest);
}

static char	*get_line(char *s, int fd);
static int	parse_str(char *s);
static char	*cut_str(char *s, int end_index);
static char	*retain(char *s, int nl_pos);

char	*get_next_line(int fd)
{
	static char	*buf;
	char		*rtrn;
	int			sz;
	int			nl_pos;

	if (!buf)
	{
		buf = (char *) malloc (sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return (NULL);
	}
	nl_pos = parse_str(buf);
	sz = read(fd, buf + nl_pos, BUFFER_SIZE - nl_pos);
	if (sz <= 0)
		return (NULL);
	buf[sz] = '\0';
	nl_pos = parse_str(buf);
	if (nl_pos)
	{
		rtrn = cut_str(buf, nl_pos);
		buf = retain(buf, nl_pos);
	}
	else
		rtrn = get_line(buf, fd);
	return (rtrn);
}

static char	*retain(char *s, int nl_pos)
{
	char	*newstr;
	int		i;

	newstr = (char *) malloc (sizeof(char) * (BUFFER_SIZE + 1));
	if (!newstr)
		return (NULL);
	i = 0;
	while (BUFFER_SIZE - nl_pos - 1)
	{
		newstr[i] = s[nl_pos + i + 1];
		i++;
		nl_pos--;
	}
	return (newstr);
}

static char	*get_line(char *s, int fd)
{
	char	*append;
	char	*rtrn;
	int		sz;
	int		nl_pos;

	nl_pos = 0;
	append = (char *) malloc (sizeof(char) * (BUFFER_SIZE + 1));
	if (!append)
		return (NULL);
	while (nl_pos == 0)
	{
		sz = read(fd, append, BUFFER_SIZE);
		if (sz <= 0)
			return (NULL);
		append[sz] = '\0';
		rtrn = ft_strjoin(s, append);
		nl_pos = parse_str(rtrn);
	}
	s = rtrn;
	rtrn = cut_str(rtrn, nl_pos);
	return (rtrn);
}

/*
End_index: position of \n
Size of new string: end_index position + 1 (starting at 0) + 1 (for \0)
For last loop to run (end_index = 0), condition must be + 1
*/
static char	*cut_str(char *s, int end_index)
{
	char	*newstr;

	newstr = (char *) malloc (sizeof(char) * (end_index + 2));
	if (!newstr)
		return (NULL);
	newstr[end_index + 1] = '\0';
	while (end_index + 1)
	{
		newstr[end_index] = s[end_index];
		end_index--;
	}
	return (newstr);
}

static int	parse_str(char *s)
{
	int	nl_pos;

	nl_pos = ft_strchr(s, '\n');
	return (nl_pos);
}

int	main(void)
{
	int		fd;
	char	*line;
	char	*file;

	file = "test.txt";
	fd = open(file, O_RDONLY);
	line = (char *) calloc (sizeof(char) * BUFFER_SIZE, 1);
	if (!line)
		return (0);
	while (line)
	{
		line = get_next_line(fd);
		printf("%s\n\n", line);
		free (line);
	}
	//free (line);
	return (0);
}
