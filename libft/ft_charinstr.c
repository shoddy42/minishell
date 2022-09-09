/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_charinstr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 16:31:51 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/09 17:11:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	ft_charinstr(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != c && str[i] != '\0')
		i++;
	if (str[i] == c && str[i] != '\0')
		return (1);
	return (0);
}
