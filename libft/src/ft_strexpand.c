/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strexpand.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/15 03:34:09 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/07 19:45:14 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strexpand(char *str, char const *expand)
{
	char	*s;
	size_t	i;
	size_t	j;

	if (!str && !expand)
		return (NULL);
	if (!str)
		return (NULL);
	if (!expand)
		return (str);
	s = ft_calloc((ft_strlen(str) + ft_strlen(expand) + 1), sizeof(char));
	if (!s)
		return (NULL);
	i = -1;
	while (str[++i])
		s[i] = str[i];
	j = -1;
	while (expand[++j])
		s[i + j] = expand[j];
	s[i + j] = '\0';
	free(str);
	return (s);
}
