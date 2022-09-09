/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/09 17:42:11 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//int init_minishell()

char *ft_strtok(char *s, const char *delim)
{
	int i;

	i = 0;
	while (s[i])

}

int ft_tokenize(t_minishell *data, char *command, int start)
{
	int i;

	i = 0;
	printf("cmd: [%s] \n", command);
	while(ft_charinstr(DELIMITER, command[i + start]) == 0 && command[i + start])
		i++;
	if (ft_charinstr(DELIMITER, command[i + start]) == 1)
		printf("FOUND ONE\n");
	return (0);
}

int	init_minishell(t_minishell *data)
{
	//todo: add shit to initialize?
	return (0);
}

int main(void)
{
	char 		*command;
	t_minishell data;

	init_minishell(&data);
	while (strcmp(command, "exit") && strcmp(command, "q"))
	{
		command = readline("> ");
		ft_tokenize(&data, command, 0);
		if (ft_strlen(command) > 0)
			add_history(command);
		free(command);
	}
	// printf("%i, %i, %i\n", COMMAND, OPTION, PIPE);
	return (0);
}