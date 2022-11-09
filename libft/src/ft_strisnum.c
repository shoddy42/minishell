/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strisnum.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 19:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/09 06:03:02 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
	@brief	Determines whether a string is made of only digits.
	@param 	str the string to be assessed.
	@return @b [true] if it IS all digits.
	@return @b [false] if it's NOT all digits.
**/
int	ft_strisnum(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (ft_isdigit(str[i]) == false)
			return (false);
	return (true);
}
