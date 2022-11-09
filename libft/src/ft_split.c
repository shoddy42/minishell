/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/02 21:22:56 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/09 05:57:12 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	word_len(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static size_t	count_str(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		while (s[i] && s[i] != c)
			i++;
		count++;
		while (s[i] && s[i] == c)
			i++;
	}
	return (count);
}

static void	free_split(char **split, size_t j)
{
	size_t	i;

	i = 0;
	while (i <= j)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	*next_str(char const *s, char c)
{
	while (*s && *s == c)
		s++;
	return ((char *)s);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	i;
	size_t	strcount;

	if (!s)
		return (NULL);
	i = 0;
	strcount = count_str(s, c);
	split = ft_calloc(strcount + 1, sizeof(char *));
	if (!split)
		return (NULL);
	while (i < strcount)
	{
		s = next_str(s, c);
		split[i] = ft_substr(s, 0, word_len(s, c));
		if (!split[i])
		{
			free_split(split, i);
			return (NULL);
		}
		i++;
		s += word_len(s, c);
	}
	split[strcount] = NULL;
	return (split);
}
