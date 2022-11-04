/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 14:59:49 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/01 19:32:28 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_dong(t_token *token, t_minishell *shell)
{
	char	*variable;
	t_token *tmp;

	if (token->next)
		tmp = token->next;
	// printf ("expanding (%s)[%s] \n", print_token_type(tmp->type) , tmp->data);
	if (tmp->type == COMMAND)
	{
		variable = ms_getenv(tmp->data, shell);
		free_single_token(tmp);
	}
	else
		variable = "";
	free(token->data);
	token->type = COMMAND; //might need other types too?
	token->data = ft_strdup(variable);
}
