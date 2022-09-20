/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strexpand.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/15 03:34:09 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/15 03:41:40 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strexpand(char *str, char const *expand)
{
	char	*s;
	size_t	i;
	size_t	j;

	if (!str && !expand)
		return (NULL);
	s = ft_calloc((ft_strlen(str) + ft_strlen(expand) + 1), sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		s[i] = str[i];
		i++;
	}
	while (expand[j])
	{
		s[i + j] = expand[j];
		j++;
	}
	s[i + j] = '\0';
	free(str);
	return (s);
}
