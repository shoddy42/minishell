/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memccpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/16 01:31:34 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/05 17:22:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	check;
	unsigned char	*d;
	unsigned char	*s;

	check = (unsigned char)c;
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	while (n)
	{
		n--;
		*d = *s;
		if (*d == check)
			return (d + 1);
		d++;
		s++;
	}
	return (NULL);
}
