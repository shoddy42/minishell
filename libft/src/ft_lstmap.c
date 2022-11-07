/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/18 07:15:59 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/05 17:22:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*node;
	t_list	*start;

	if (!f)
		return (NULL);
	start = NULL;
	while (lst)
	{
		node = ft_lstnew(lst->content);
		if (!node)
		{
			ft_lstclear(&start, del);
			return (NULL);
		}
		node->content = (*f)(node->content);
		ft_lstadd_back(&start, node);
		lst = lst->next;
	}
	return (start);
}
