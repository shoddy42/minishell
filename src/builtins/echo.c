/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 18:42:30 by auzochuk      #+#    #+#                 */
/*   Updated: 2022/11/22 19:17:12 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//removed != 0 from the strcmp. should be good? thx norme
bool	dash_n(const char *flag)
{
	int	i;

	i = 0;
	if (flag && flag[0] == '-')
	{
		while (flag[++i])
			if (flag[i] != 'n')
				return (false);
	}
	else
		return (false);
	return (true);
}

int	ms_echo(t_command *command)
{
	int		i;
	bool	no_nl;

	if (!(command->command))
		return (1);
	i = 1;
	no_nl = false;
	while (dash_n(command->command[i]))
	{
		no_nl = true;
		i++;
	}
	while (command->command[i])
	{
		ft_putstr_fd(command->command[i], command->outfile);
		if (command->command[i + 1] && ft_strcmp(command->command[i + 1], ""))
			write(command->outfile, " ", 1);
		i++;
	}
	if (no_nl)
		return (0);
	ft_putchar_fd('\n', command->outfile);
	return (0);
}
