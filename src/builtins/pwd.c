/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 11:23:21 by wkonings      #+#    #+#                 */
/*   Updated: 2022/11/16 14:30:05 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include  "include/minishell.h"

int	ms_pwd(t_command	*cmd)
{
	int		i;
	char	*path;

	path = getcwd(NULL, 0);
	ft_putstr_fd(path, 1);
	write(1, "\n", 1);
	if (path)
		free(path);
	return (0);
}
