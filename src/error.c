/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 05:59:42 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/14 12:41:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//later: evaluate whether this is even needed at all.
void	ms_error(char *msg, int code, bool terminate, t_minishell *shell)
{
	if (code != 0)
		errno = code;
	perror(msg);
	if (terminate == true)
		exit(code);
}
