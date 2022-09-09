/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 17:43:18 by wkonings      #+#    #+#                 */
/*   Updated: 2019/12/05 19:58:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void		*s;
	size_t		i;

	i = 0;
	s = malloc(count * size);
	if (!s)
		return (NULL);
	while (i < (count * size))
	{
		((char *)s)[i] = '\0';
		i++;
	}
	return (s);
}
