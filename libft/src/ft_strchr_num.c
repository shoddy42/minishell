/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr_num.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <root@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 10:23:30 by root          #+#    #+#                 */
/*   Updated: 2022/11/09 10:29:48 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

//temporary file, need to push the real one from codam.
int ft_strchr_num(const char *s, int c)
{
    int i;

    i = -1;
    while (s[++i])
        if (s[i] == (char)c)
            return (i);
    return (-1);
}