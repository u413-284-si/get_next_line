/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sqiu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:34:22 by sqiu              #+#    #+#             */
/*   Updated: 2022/11/08 18:02:12 by sqiu             ###   ########.fr       */
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

/*
***************************************Uncomment for testing*******************************
*/
/* int	ft_strchr(const char *s, int c)
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
} */

static char	*get_line(int fd);
static int	parse_str(char *s);
static char	*cut_str(char *s, int end_index);
static char	*retain(char *s, int nl_pos);

char	*get_next_line(int fd)
{
	static char	*ret;
	char		*buf;
	char		*rtrn;
	char		*tmp;
	int			nl_pos;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	nl_pos = 0;
	buf = "";
	while (nl_pos == 0)
	{
		buf = get_line(fd);
		if (buf == NULL)
			return (NULL);
		nl_pos = parse_str(buf);
		tmp = ft_strdup(ret);
		free(ret);
		ret = ft_strjoin(tmp, buf);
		free(tmp);
		free(buf);
		rtrn = cut_str(ret, parse_str(ret));
		ret = retain(ret, parse_str(ret));
		if (nl_pos == 0 && ft_strlen(rtrn) == 1)
			break ;
	}
	return (rtrn);
}

static char	*retain(char *s, int nl_pos)
{
	char	*newstr;
	int		j;
	int		len;

	if (!s || (nl_pos == 0 && *s == '\0'))
	{
		free (s);
		return (NULL);
	}
	len = -1;
	j = 0;
	while (s[nl_pos + j++])
		len++;
	newstr = (char *) malloc (sizeof(char) * (len + 1));
	if (!newstr)
		return (NULL);
	j = 0;
	while (j < len)
	{
		newstr[j] = s[nl_pos + 1 + j];
		j++;
	}
	newstr[len] = '\0';
	free(s);
	return (newstr);
}

static char	*get_line(int fd)
{
	char	*append;
	int		sz;

	append = (char *) malloc (sizeof(char) * (BUFFER_SIZE + 1));
	if (!append)
		return (NULL);
	sz = read(fd, append, BUFFER_SIZE);
	if (sz <= 0)
	{
		free(append);
		return (NULL);
	}
	append[BUFFER_SIZE] = '\0';
	return (append);
}

/*
End_index: position of \n
Size of new string: end_index position + 1 (starting at 0) + 1 (for \0)
For last loop to run (end_index = 0), condition must be + 1
*/
static char	*cut_str(char *s, int end_index)
{
	char	*newstr;

	if (!s)
		return (NULL);
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

	if (!s)
		return (0);
	nl_pos = ft_strchr(s, '\n');
	return (nl_pos);
}

/* int	main(void)
{
	int		fd;
	char	*line;
	char	*file;
	//char	*file_empty;
	int		i;

	file = "41_no_nl";
	//file_empty = "empty.txt";
	fd = open(file, O_RDONLY);
	//fd = open(file_empty, O_RDONLY);
	i = 0;
	while (i < 9)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		i++;
	}
	return (0);
} */
