#include "libft.h"
#define BUFFER_SIZE 100

char	*gnl_store_rest(char *rest, char **line)
{
	int		size_rest;
	char	*tmp;
	char	*chr;

	if (!rest)
		size_rest = 0;
	else
	{
		chr = (ft_strchr(rest, '\n') != NULL) ?
			ft_strchr(rest, '\n') : rest + ft_strlen(rest);
		size_rest = (int)(chr - rest);
	}
	free(*line);
	if (!(*line = ft_substr(rest, 0, size_rest)))
		return (NULL);
	if (!(tmp = ft_substr(rest, size_rest + 1, BUFFER_SIZE)))
		return (NULL);
	free(rest);
	return (tmp);
}

char	*gnl_new_line(char **line, char *buffer)
{
	int		size_buffer;
	int		size_line;
	char	*tmp;
	char	*chr;

	if (!buffer)
		size_buffer = 0;
	else
	{
		chr = (ft_strchr(buffer, '\n') != NULL) ?
			ft_strchr(buffer, '\n') : buffer + ft_strlen(buffer);
		size_buffer = (int)(chr - buffer);
	}
	size_line = ft_strlen(*line);
	if (!(tmp = malloc(sizeof(char) * (size_line + size_buffer + 1))))
		return (NULL);
	tmp[0] = '\0';
	ft_strlcat(tmp, *line, size_line + 1);
	ft_strlcat(tmp, buffer, size_line + size_buffer + 1);
	free(*line);
	*line = tmp;
	return (ft_substr(buffer, size_buffer + 1, BUFFER_SIZE));
}

int		gnl_care_rest(char **rest, char **line)
{
	if (*rest == NULL)
	{
		if (!(*rest = ft_strdup("")))
			return (-1);
	}
	if (ft_strlen(*rest) > 0)
	{
		if (ft_strchr(*rest, '\n'))
		{
			if (!(*rest = gnl_store_rest(*rest, line)))
				return (-1);
			return (1);
		}
		else
		{
			if (!(*rest = gnl_store_rest(*rest, line)))
				return (-1);
		}
	}
	return (0);
}

int		get_next_line(int fd, char **line)
{
	static char	*rest;
	int			ret;
	char		buffer[BUFFER_SIZE + 1];

	if (!line || (read(fd, buffer, 0) != 0))
		return (-1);
	*line = ft_strdup("");
	buffer[0] = '\0';
	if ((ret = gnl_care_rest(&rest, line)) != 0)
		return (ret);
	while (!ft_strchr(buffer, '\n'))
	{
		free(rest);
		rest = NULL;
		ret = read(fd, buffer, BUFFER_SIZE);
		if (ret == 0)
			return (0);
		if (ret < 0)
			return (-1);
		buffer[ret] = '\0';
		if (!(rest = gnl_new_line(line, buffer)))
			return (-1);
	}
	buffer[0] = '\0';
	return (1);
}
