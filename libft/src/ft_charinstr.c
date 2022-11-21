/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_charinstr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 16:31:51 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/20 16:09:38 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * @brief Returns whether a character exists in a given string.
 * 
 * @param c the character to look for
 * @param s	the string to look in
 * @returns	returns @b [true] if the character exists in @b [s],
 * @returns	returns @b [false] otherwise.
 */
bool	ft_charinstr(char c, const char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (true);
	return (false);
}
