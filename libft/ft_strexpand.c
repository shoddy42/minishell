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
	char	*str;
	size_t	i;
	size_t	j;

	if (!str && !expand)
		return (NULL);
	str = ft_calloc((ft_strlen(str) + ft_strlen(expand) + 1), sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		str[i] = str[i];
		i++;
	}
	while (expand[j])
	{
		str[i + j] = expand[j];
		j++;
	}
	str[i + j] = '\0';
    free(str);
	return (str);
}
