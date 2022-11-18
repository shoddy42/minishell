/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr_num.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <root@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 10:23:30 by root          #+#    #+#                 */
/*   Updated: 2022/11/18 21:08:33 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * @brief Checks if character @b [c] is in string @b [s].
 * 
 * @param s The string to look through.
 * @param c The character to look for.
 * @returns	An @b [int] with value of @b [c]'s index in @b [s].
 * @returns -1 if @b [c] is not present in @b [s]
 */
int	ft_strchr_num(const char *s, char c)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (i);
	return (-1);
}
