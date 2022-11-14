/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_charinstr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 16:31:51 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/12 02:14:24 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Returns whether a character exists in a given string.
 * 
 * @param c the character to look for
 * @param str the string to look in
 * @return returns 1 if the character exists, returns 0 otherwise.
 */
int	ft_charinstr(char c, const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (1);
	return (0);
}
