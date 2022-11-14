/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/04/01 21:01:46 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/12 02:40:55 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

static int	ft_isspace(char n)
{
	if (n == ' ' || n == '\t' || n == '\v'
		|| n == '\f' || n == '\r' || n == '\n')
		return (1);
	return (0);
}

int	ft_atoi(const char *nb)
{
	int				i;
	unsigned long	r;
	int				sign;

	i = 0;
	r = 0;
	sign = 1;
	while (ft_isspace(nb[i]))
		i++;
	if (nb[i] == '-')
		sign = -1;
	if (nb[i] == '-' || nb[i] == '+')
		i++;
	while (nb[i] >= '0' && nb[i] <= '9')
	{
		r = (r * 10) + (nb[i] - '0');
		i++;
	}
	return ((int)(r * sign));
}
