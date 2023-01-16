/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 11:23:21 by wkonings      #+#    #+#                 */
/*   Updated: 2023/01/16 22:57:17 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include  "../../include/minishell.h"

int	ms_pwd(t_command *cmd, t_minishell *shell)
{
	int		i;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path || ft_strcmp(path, ""))
	{
		if (path)
			free(path);
		path = ft_strdup(ms_getenv("PWD", shell));
	}
	if (!path)
		return (1);
	ft_putstr_fd(path, 1);
	write(1, "\n", 1);
	if (path)
		free(path);
	return (0);
}
