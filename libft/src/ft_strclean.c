/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strclean.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: root <root@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/20 15:21:40 by root          #+#    #+#                 */
/*   Updated: 2022/11/24 01:00:34 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>
/**
 * @brief Copies @b [s], but removes all characters from @b [remove].
 * 
 * @param s         The string to return a copied version of.
 * @param remove    All characters to remove.
 * @return char* 
 */
char	*ft_strclean(const char *s, const char *remove)
{
	int		i;
	int		clean;
	char	*ret;

	ret = ft_calloc(ft_strlen(s), sizeof(char));
	if (!ret)
		return (NULL);
	i = -1;
	clean = 0;
	while (s[++i + clean])
	{
		while (ft_charinstr(s[i + clean], remove))
			clean++;
		ret[i] = s[i + clean];
	}
	return (ret);
}
