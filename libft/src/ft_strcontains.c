/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcontains.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/24 19:05:25 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/24 19:18:09 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * @brief Checks if any characters of @b delims are in string @b s
 * 
 * @param s 		The string to look in.
 * @param delims	The delimiters to look for.
 * @returns returns @b [true] if any of the delim are in s,
 * @returns	returns	@b [false] otherwise. 
 */
bool	ft_strcontains(char *s, char *delims)
{
	int	i;

	if (!s || !delims)
		return (false);
	i = -1;
	while (s[++i])
		if (ft_charinstr(s[i], delims) == true)
			return (true);
	return (false);
}
