/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/03 21:46:59 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/12 05:05:23 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	ft_numlen(long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

/**
 * @brief Takes an integer and turns it into a string.
 * 
 * @param n Number to be converted.
 * @return Allocated string containing the number. Or NULL incase of faillure
 */
char	*ft_itoa(int n)
{
	char	*res;
	long	nb;
	size_t	len;

	nb = n;
	if (n < 0)
		n *= -1;
	len = ft_numlen(nb) + (n < 0);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res[len] = '\0';
	while (len > 0)
	{
		len--;
		res[len] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
}
