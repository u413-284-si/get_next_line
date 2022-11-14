/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sqiu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:34:22 by sqiu              #+#    #+#             */
/*   Updated: 2022/11/14 17:27:37 by sqiu             ###   ########.fr       */
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
except if the end of file was reached and does not end with a \n character.
*/

/*
***************************************Uncomment for testing*******************************
*/

/* char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((void *)&s[i]);
		i++;
	}
	if (c == 0)
		return ((void *)&s[i]);
	return (NULL);
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

	if (!s)
		return (0);
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


/*
***************************************************************************************************************************************
*/

static char	*get_line(int fd, char *ret, char *buf);
static char	*monster_free(char *uno, char *due);
static char	*cut_str(char *s);
static char	*retain(char *s);

char	*get_next_line(int fd)
{
	static char	*ret = "";
	char		*buf;
	char		*rtrn;
	char		*tmp;
	char		*str;

	if (!ret)
		return (NULL);
	str = "";
	if (read(fd, str, 0) < 0 || BUFFER_SIZE <= 0)
		return (ret = NULL);
	buf = malloc (sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	tmp = get_line(fd, ret, buf);
	if (!tmp)
		return (ret = NULL);
	rtrn = cut_str(tmp);
	if (!rtrn)
		return (NULL);
	ret = retain(tmp);
	free(tmp);
	return (rtrn);
}

static char	*get_line(int fd, char *ret, char *buf)
{
	char	*tmp;
	int		sz;
	int		len;

	sz = read(fd, buf, BUFFER_SIZE);
	buf[sz] = '\0';
	if (sz == 0 && !*ret)
	{
		free(buf);
		return (NULL);
	}
	while (!ft_strchr(ret, '\n') && sz > 0)
	{
		len = ft_strlen(ret);
		tmp = malloc (sizeof(char) * (len + 1));
		tmp[len] = '\0';
		ft_memcpy(tmp, ret, len);
		if (*ret != '\0')
			free(ret);
		ret = ft_strjoin(tmp, buf);
		if (!ret)
			return (monster_free(tmp, buf));
		monster_free(tmp, buf);
		buf = malloc (sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return (NULL);
		sz = read(fd, buf, BUFFER_SIZE);
		buf[sz] = '\0';
	}
	free(buf);
	return (ret);
}

/* 
Check if \n is found or if nothing is found after the \n.
In that case nothing is to be retained, NULL is returned
which results in gnl returning NULL.
*/
static char	*retain(char *s)
{
	char	*newstr;
	int		j;
	int		len;
	char	*end;

	end = ft_strchr(s, '\n');
	if (!end || !*(end + 1))
		return (NULL);
	len = -1;
	j = 0;
	while (end[j++])
		len++;
	newstr = (char *) malloc (sizeof(char) * (len + 1));
	if (!newstr)
		return (NULL);
	j = 0;
	while (j < len)
	{
		newstr[j] = end[j + 1];
		j++;
	}
	newstr[len] = '\0';
	return (newstr);
}

/*
End_index: position of \n
Size of new string: end position + 1 (starting at 0) + 1 (for \0)
For last loop to run (end = 0), condition must be + 1
*/
static char	*cut_str(char *s)
{
	char	*newstr;
	int		s_len;
	char	*end;

	end = ft_strchr(s, '\n');
	if (!end)
		s_len = ft_strlen(s) + 1;
	else
		s_len = end - s + 2;
	newstr = malloc (sizeof(char) * s_len);
	if (!newstr)
		return (NULL);
	newstr[s_len - 1] = '\0';
	ft_memcpy(newstr, s, s_len - 1);
	return (newstr);
}

static char	*monster_free(char *uno, char *due)
{
	if (uno)
		free(uno);
	if (due)
		free (due);
	return (NULL);
}

/* int	main(void)
{
	int		fd;
	char	*line;
	char	*file;
	//char	*file_empty;
	int		i;

	file = "41_with_nl";
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
