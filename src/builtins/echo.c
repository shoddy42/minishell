/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 18:42:30 by auzochuk      #+#    #+#                 */
/*   Updated: 2023/01/17 17:17:28 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//removed != 0 from the strcmp. should be good? thx norme
static bool	dash_n(const char *flag)
{
	int	i;

	i = 0;
	if (flag && flag[0] == '-')
	{
		while (flag[++i])
			if (flag[i] != 'n')
				return (false);
		if (i == 1)
			return (false);
	}
	else
		return (false);
	return (true);
}

/**
 * @brief Mimics echo command. Writes given string
 * 
 * @param command The commands to read the str from.
 * @returns 0 upon success. 1 on failure.
 * @bug		Due to no mutex/pthreads, multiple appending writes to 1 file
 * 			can desync with the newline write.
 */
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
	write(command->outfile, "\n", 1);
	return (0);
}
