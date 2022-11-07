/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_charinstr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 16:31:51 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/20 11:48:18 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
