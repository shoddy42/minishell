/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/16 03:57:38 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/05 17:22:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	check;
	char	*str;
	size_t	i;
	size_t	len;

	check = (char)c;
	str = (char *)s;
	i = 0;
	len = ft_strlen(str);
	while (i <= len)
	{
		if (str[i] == check)
			return (&str[i]);
		i++;
	}
	return (NULL);
}
