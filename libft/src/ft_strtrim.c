/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/02 20:46:33 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/05 17:22:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	isset(char const c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i] != c && set[i] != '\0')
		i++;
	if (set[i] == c)
		return (1);
	return (0);
}

static int	trim_start(char const *s1, size_t len, char const *set)
{
	size_t	i;

	i = 0;
	while (isset(s1[i], set) && i < len)
		i++;
	return (i);
}

static int	trim_end(char const *s1, size_t len, char const *set)
{
	size_t	i;

	i = len;
	while (isset(s1[i], set) && i != 0)
		i--;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	start;
	size_t	j;
	size_t	end;
	size_t	len;

	if (!s1)
		return (NULL);
	j = 0;
	len = ft_strlen(s1);
	start = trim_start(s1, len, set);
	if (s1[start] == '\0')
		return (ft_strdup(""));
	end = trim_end(s1, len, set);
	str = malloc(sizeof(char) * (end - start + 2));
	if (!str)
		return (NULL);
	while (s1[start] && j < end - start + 1 && start != len)
	{
		str[j] = s1[start + j];
		j++;
	}
	str[j] = '\0';
	return (str);
}
