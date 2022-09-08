/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/08 22:48:49 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	char *command;

	while (strcmp(command, "exit") && strcmp(command, "q"))
	{
		command = readline("> ");
		add_history(command);
	}
	printf("%i, %i, %i\n", COMMAND, OPTION, PIPE);
	return (0);
}