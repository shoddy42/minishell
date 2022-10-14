/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: auzochuk <auzochuk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 18:42:30 by auzochuk      #+#    #+#                 */
/*   Updated: 2022/10/14 18:48:23 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ms_echo(t_command *command)
{
	int	i;

	i = 1;
	if(!(command->command))
		return(1);
	if (!command->command[1])
		return (0);
	if (command->command[1] && ft_strcmp(command->command[1], "-n") == 0)
		i++;
	while(command->command[i])
	{
		ft_putstr_fd(command->command[i], command->outfile);
		write(command->outfile, " ", 1);
		i++;
	}
	if (ft_strcmp(command->command[1], "-n") == 0)
		return (0);
	ft_putchar_fd('\n', command->outfile);
	return(0);
}
