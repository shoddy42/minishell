/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 17:43:18 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/21 19:03:27 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * @brief Allocates for (count * size), and sets allocated memory to NULL
 * 
 * @param count The amount of elements to allocate for.
 * @param size	The size of each element.
 * 				intended for use with sizeof()
 * 
 * @returns	A block of memory from malloc fully set to NULL.
 * 			Or NULL incase of malloc failure.
 */
void	*ft_calloc(size_t count, size_t size)
{
	void	*s;
	size_t	i;

	s = malloc(count * size);
	if (!s)
		return (NULL);
	i = -1;
	while (++i < (count * size))
		((char *)s)[i] = '\0';
	return (s);
}
