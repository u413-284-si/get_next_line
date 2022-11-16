/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sqiu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:34:22 by sqiu              #+#    #+#             */
/*   Updated: 2022/11/16 11:38:57 by sqiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
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
**********************************Uncomment for testing*******************
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
***************************************************************************
*/

char	*init_mult(char *ret);
char	*get_line_mult(int fd, char **ret);
char	*cut_str_mult(char *s);
char	*retain_mult(char *s);

char	*get_next_line(int fd)
{
	static char	*ret[4096];
	char		*rtrn;
	char		*tmp;

	if (read(fd, 0, 0) < 0 || BUFFER_SIZE <= 0 || fd < 0)
	{
		if (ret[fd])
			free (ret[fd]);
		return (ret[fd] = NULL);
	}
	tmp = get_line_mult(fd, ret);
	if (!tmp)
		return (ret[fd] = NULL);
	rtrn = cut_str_mult(tmp);
	if (!rtrn)
		return (NULL);
	ret[fd] = retain_mult(tmp);
	free(tmp);
	return (rtrn);
}

char	*get_line_mult(int fd, char **ret)
{
	char	*tmp;
	int		sz;
	char	*buf;

	ret[fd] = init_mult(ret[fd]);
	sz = 1;
	while (!ft_strchr(ret[fd], '\n') && sz > 0)
	{
		buf = malloc (sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return (NULL);
		sz = read(fd, buf, BUFFER_SIZE);
		buf[sz] = '\0';
		if (sz == 0 && !*ret[fd])
		{
			monster_free(buf, ret[fd]);
			return (NULL);
		}
		tmp = ret[fd];
		ret[fd] = ft_strjoin(tmp, buf);
		if (!ret[fd])
			return (monster_free(tmp, buf));
		monster_free(tmp, buf);
	}
	return (ret[fd]);
}

/* 
Check if \n is found or if nothing is found after the \n.
In that case nothing is to be retained, NULL is returned
which results in gnl returning NULL.
*/
char	*retain_mult(char *s)
{
	char	*newstr;
	int		j;
	int		len;
	char	*end;

	end = ft_strchr(s, '\n');
	if (!end || !*(end))
		return (NULL);
	len = 0;
	j = 1;
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
*/
char	*cut_str_mult(char *s)
{
	char	*newstr;
	int		s_len;
	char	*end;

	end = ft_strchr(s, '\n');
	if (!end)
		s_len = ft_strlen(s);
	else
		s_len = end - s + 1;
	newstr = malloc (sizeof(char) * (s_len + 1));
	if (!newstr)
		return (NULL);
	newstr[s_len] = '\0';
	ft_memcpy(newstr, s, s_len);
	return (newstr);
}

char	*init_mult(char *ret)
{
	if (!ret)
	{
		ret = malloc (1);
		if (!ret)
			return (NULL);
		ret[0] = '\0';
	}
	return (ret);
}

/* int	main(void)
{
	int		fd;
	char	*line;
	char	*file;
	//char	*file_empty;
	int		i;

	file = "nl"; //42_with_nl
	//file_empty = "empty.txt";
	fd = open(file, O_RDONLY);
	//fd = open(file_empty, O_RDONLY);
	i = 0;
	while (i < 3)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		i++;
	}
	return (0);
} */
