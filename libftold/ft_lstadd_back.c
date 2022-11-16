/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_back.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/12 12:02:26 by wkonings      #+#    #+#                 */
/*   Updated: 2022/02/24 15:19:37 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*back;

	if (!new)
		return ;
	back = ft_lstlast(*alst);
	if (!back)
		*alst = new;
	else
		back->next = new;
}
