/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/03 23:28:50 by wkonings       #+#    #+#                */
/*   Updated: 2019/12/14 19:13:07 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dst && !src)
		return (NULL);
	if (dst < src)
		ft_memcpy(dst, src, n);
	else
	{
		while (i < n)
		{
			i++;
			((unsigned char *)dst)[n - i] = ((unsigned char *)src)[n - i];
		}
	}
	return (dst);
}
