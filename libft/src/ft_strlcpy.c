/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 12:38:27 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/16 19:12:36 by wkonings      ########   odam.nl         */
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
