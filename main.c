/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 16:16:20 by wkonings      #+#    #+#                 */
/*   Updated: 2022/09/09 19:23:43 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	get_token_type()

void	new_token(t_minishell *shell, char *data, int len)
{
	int		i;
	t_token	*new;
	t_token *last;

	i = 0;
	new = malloc(sizeof(t_token));
	new->data = malloc(sizeof(char) * len + 1);
	while (--len)
		new->data[i++] = data[len];
	new->data[i] = '\0';
	last = ft_lstlast(shell->tokens);
	last->next = new;
	new->prev = last;
	// get_token_type
}

int	ft_tokenize(t_minishell *shell, char *command)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (command[i])
	{
		while (ft_charinstr(DELIMITER, command[i]) == 0 && command[i])
			i++;
		if (ft_charinstr(DELIMITER, command[i]) == 1)
			new_token(shell, command, i);
		command + 5;
	}
	return (0);
}

int	init_minishell(t_minishell *shell)
{
	//todo: add shit to initialize?
	return (0);
}

int	main(void)
{
	char		*command;
	t_minishell	shell;

	init_minishell(&shell);
	while (strcmp(command, "exit") && strcmp(command, "q"))
	{
		command = readline("> ");
		ft_tokenize(&shell, command);
		if (ft_strlen(command) > 0)
			add_history(command);
		free(command);
	}
	return (0);
}
