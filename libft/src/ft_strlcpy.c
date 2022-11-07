/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 12:38:27 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/05 17:22:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcpy(char *dst, char const *src, size_t dstsize)
{
	char	*tmp;

	if (!dst && !src)
		return (0);
	tmp = ft_memccpy(dst, src, '\0', dstsize);
	if (!tmp && dstsize)
		dst[dstsize - 1] = '\0';
	return (ft_strlen(src));
}

/* #include <unistd.h>
#include <stdlib.h>
#include <get_next_line.h>
#include <fcntl.h>
#include <stdio.h>

t_list	*check_node(t_list **head, int fd)
{
	t_list	*ptr;
	t_list	*new;

	ptr = *head;
	while (ptr)
	{
		if (ptr->fd == fd)
			return (ptr);
		ptr = ptr->next;
	}
	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!new->buff)
	{
		free(new);
		return (NULL);
	}
	new->first_call_check = 1;
	new->fd = fd;
	new->next = *head;
	*head = new;
	return (new);
}

int		handle_newline(t_list *node, char *content, int	nl_bool, char **line)
{
	char *copy;

	copy = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!copy)
		return (-1);
	ft_strncpy(copy, node->buff, node->read, 1);
	if (nl_bool == 1)
	{
		*line = ft_strjoin(node->buff, content);
		ft_strncpy(node->buff, content, node->read, 0);
	//	printf("BUFF C: %s\n", node->buff);
		return (1);
	}
	if (ft_strchr(node->buff, '\n') == 1)
	{
	
		*line = ft_strdup(node->buff, 0);
		ft_strncpy(node->buff, copy, node->read, 0);
//		printf("BUFF A: %s\n", node->buff);
		//printf("LINE: %s\n", *line);
		return (1);
	}
	return (1);
}

int		gnl_read(t_list *node, char **line, char *content)
{
	char	*copy;
	ssize_t	bytes_read;
	int		ret;

	bytes_read = read(node->fd, content, BUFFER_SIZE);
	content[BUFFER_SIZE] = '\0';
	node->read = bytes_read;
	ret = handle_newline(node, content, ft_strchr(content, '\n'), line);
	printf("CONTENT: %s\n", content);
//	printf("BUFF: %s\n", node->buff);
//	printf("COPY: %s\n", copy);
//	printf("LINE: %s\n", *line);
	return (ret);
}

int		get_next_line(int fd, char **line)
{
	static t_list	*head;
	t_list			*node;
	int				state;
	char			*content;
	
	content = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!content)
		return (-1);
	state = 2;
	node = check_node(&head, fd);
	if (!node)
		return (-1);
	while (state == 2)
		state = gnl_read(node, line, content);
	printf("END STATE: %i\n", state);
	return (1);
}

int		main(void)
{
	int file = open("abc.txt", O_RDONLY);
	char *line;
	int loop;

	loop = 8;
	while (loop < 10)
	{
		get_next_line(file, &line);
		loop++;
		printf("LINE FINISHED: %s\n", line);
		free(line);
	}
//	sleep(500);
	return (0);
}*/

/*
#include <unistd.h>
#include <stdlib.h>
#include <get_next_line.h>
#include <fcntl.h>
#include <stdio.h>

t_list	*check_node(t_list **head, int fd)
{
	t_list	*ptr;
	t_list	*new;

	ptr = *head;
	while (ptr)
	{
		if (ptr->fd == fd)
			return (ptr);
		ptr = ptr->next;
	}
	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!new->buff)
	{
		free(new);
		return (NULL);
	}
	new->first_call_check = 1;
	new->fd = fd;
	new->size = -1;
	new->next = *head;
	*head = new;
	return (new);
}

int		handle_newline(t_list *node, char *content, int nl, char **line)
{
	char *copy;
	int ret;
	int i;
	ret = 0;
	i = 0;

	copy = ft_strdup(node->buff, 1);
	if (!copy)
		return (-1);
	if (nl == -1) //NEWLINE
	{
		*line = ft_strdup(node->buff, 0);
		free(node->buff);
		node->buff = ft_strdup(copy, 2);
		if (!node->buff)
			return (-1);
		ret = 1;
	}
	if (nl != -1 && content[0] != '\0') //NO NEWLINE
	{
		free(node->buff);
		node->buff = ft_strjoin(copy, content);
		if (!node->buff)
			return (-1);
		while (content[i])
		{
			content[i] = '\0';
			i++;
		}
		ret = 2;
	}
	free(copy);
	return (ret);
}

int		gnl_read(t_list *node, char **line, char *content)
{
	ssize_t	size;
	int		ret;

	if (ft_strchr(node->buff, '\n') != -1)
	{
		size = read(node->fd, content, BUFFER_SIZE);
		if (size == 0 || size == -1)
			return (size);
		node->size += size;
		content[BUFFER_SIZE] = '\0';
	}
//	printf("content: |%s|\n", content);
	ret = handle_newline(node, content, ft_strchr(node->buff, '\n'), line);
	if (ret == 1 && content[0] == '\0' && node->buff[0] == '\0')
	{
		return (0);
	}
//	printf("buff: |%s|\n", node->buff);
	return (ret);
}

int		get_next_line(int fd, char **line)
{
	static t_list	*head;
	t_list			*node;
	int				state;
	char			*content;
	
	content = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!content)
		return (-1);
	state = 2;
	node = check_node(&head, fd);
	if (!node)
		return (-1);
	while (state == 2)
		state = gnl_read(node, line, content);
	free(content);
	// printf("END STATE: %i\n", state);
	return (state);
}

int		main(void)
{
	int file = open("abc2.txt", O_RDONLY);
	char *line;
	int loop;
	int ret = 1;

	loop = 0;
	while (ret)
	{
		ret = get_next_line(file, &line);
		// loop++;
		printf("[%d] LINE FINISHED: |%s|\n", ret, line);
		// free(line);
	}
	// sleep(500);
	return (0);
} */